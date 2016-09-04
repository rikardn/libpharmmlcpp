/* libpharmmlcpp - Library to handle PharmML
 * Copyright (C) 2016 Rikard Nordgren and Gunnar Yngman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * his library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <PharmML/Correlation.h>
#include <PharmML/PharmMLContext.h>
#include <AST/AstNodeFactory.h>
#include <AST/AstNode.h>

namespace pharmmlcpp
{
    Correlation::Correlation(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->parse(reader, node);
    }

    void Correlation::parse(PharmMLReader &reader, xml::Node node) {
        // Get variability reference
        xml::Node var_ref_node = reader.getSingleElement(node, "./ct:VariabilityReference");
        this->variabilityReference = new VariabilityReference(reader, var_ref_node);

        // Get pairwise/matrix specification of correlation
        xml::Node pairwise_node = reader.getSingleElement(node, "./mdef:Pairwise");
        xml::Node matrix_node = reader.getSingleElement(node, "./ct:Matrix");
        if (pairwise_node.exists()) {
            // Get the two correlated random variables
            xml::Node var1_node = reader.getSingleElement(pairwise_node, "./mdef:RandomVariable1/ct:SymbRef");
            this->pairwiseSymbRefs.push_back(new SymbRef(var1_node));
            xml::Node var2_node = reader.getSingleElement(pairwise_node, "./mdef:RandomVariable2/ct:SymbRef");
            this->pairwiseSymbRefs.push_back(new SymbRef(var2_node));

            // Get correlation type
            xml::Node corr_node = reader.getSingleElement(pairwise_node, "./mdef:CorrelationCoefficient");
            xml::Node cov_node = reader.getSingleElement(pairwise_node, "./mdef:Covariance");
            if (corr_node.exists()) {
                this->pairwiseType = "CorrelationCoefficient";
            } else if (cov_node.exists()) {
                this->pairwiseType = "Covariance";
            }

            // Get the assignment (coefficient/covariance) itself
            xml::Node assign = reader.getSingleElement(pairwise_node, ".//ct:Assign");
            xml::Node tree = assign.getChild();
            if (tree.getName() == "SymbRef") {
                this->pure_symbref_assignment = true;
            } else {
                this->pure_symbref_assignment = false;
            }
            this->pairwiseAssignment = reader.factory.create(reader, tree);
        } else if (matrix_node.exists()) {
            // TODO: Implement MatrixType support
            xml::Attribute dev_matrix_type = node.getAttribute("deviationMatrixType");
            if (dev_matrix_type.exists()) {
                this->matrixType = dev_matrix_type.getValue();
            }
        }
    }

    pharmmlcpp::VariabilityReference *Correlation::getVariabilityReference() {
        return this->variabilityReference;
    }

    bool Correlation::isPairwise() {
        return (pairwiseSymbRefs.size() == 2);
    }

    std::vector<SymbRef *> Correlation::getPairwiseSymbRefs(){
        return this->pairwiseSymbRefs;
    }

    std::string Correlation::getPairwiseType() {
        return this->pairwiseType;
    }

    std::shared_ptr<AstNode> Correlation::getPairwiseAssignment() {
        return this->pairwiseAssignment;
    }

    // True if pairwise assignment is a pure SymbRef (i.e. correlation is "named" via population parameter)
    bool Correlation::hasPureSymbRefAssigment() {
        return this->pure_symbref_assignment;
    }

    void Correlation::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        this->variabilityReference->setupSymbRefs(gathering, blkId);
        if (this->pairwiseAssignment) {
            this->setupAstSymbRefs(this->pairwiseAssignment.get(), gathering, blkId);
            for (pharmmlcpp::SymbRef *symbRef : this->pairwiseSymbRefs) {
                // Separation of correlated random variables seem like a good idea
                this->correlatedSymbols.addSymbol(this->addSymbRef(symbRef, gathering, blkId));
            }
        } else {
            // TODO: Matrix support
        }
    }

    void Correlation::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
