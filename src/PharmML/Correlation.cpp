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

namespace PharmML
{
    Correlation::Correlation(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void Correlation::parse(xml::Node node) {
        // Get variability reference
        xml::Node var_ref_node = this->context->getSingleElement(node, "./ct:VariabilityReference");
        this->variabilityReference = new PharmML::VariabilityReference(this->context, var_ref_node);
        
        // Get pairwise/matrix specification of correlation
        xml::Node pairwise_node = this->context->getSingleElement(node, "./mdef:Pairwise");
        xml::Node matrix_node = this->context->getSingleElement(node, "./ct:Matrix");
        if (pairwise_node.exists()) {
            // Get the two correlated random variables
            xml::Node var1_node = this->context->getSingleElement(pairwise_node, "./mdef:RandomVariable1");
            this->pairwiseSymbRefs.push_back(new PharmML::SymbRef(var1_node));
            xml::Node var2_node = this->context->getSingleElement(pairwise_node, "./mdef:RandomVariable2");
            this->pairwiseSymbRefs.push_back(new PharmML::SymbRef(var2_node));
            
            // Get correlation type
            xml::Node corr_node = this->context->getSingleElement(pairwise_node, "./mdef:CorrelationCoefficient");
            xml::Node cov_node = this->context->getSingleElement(pairwise_node, "./mdef:Covariance");
            if (corr_node.exists()) {
                this->pairwiseType = "CorrelationCoefficient";
            } else if (cov_node.exists()) {
                this->pairwiseType = "Covariance";
            }
            
            // Get the assignment (coefficient/covariance) itself
            xml::Node assign = this->context->getSingleElement(pairwise_node, ".//ct:Assign");
            xml::Node tree = assign.getChild();
            this->pairwiseAssignment = this->context->factory.create(tree, &deps);
        } else if (matrix_node.exists()) {
            // TODO: Implement MatrixType support
            this->matrixType = node.getAttribute("deviationMatrixType").getValue();
        }
    } 

    PharmML::VariabilityReference *Correlation::getVariabilityReference() {
        return this->variabilityReference;
    }
    
    bool Correlation::isPairwise() {
        return (pairwiseSymbRefs.size() == 2);
    }
    
    std::vector<PharmML::SymbRef *> Correlation::getPairwiseSymbRefs(){
        return this->pairwiseSymbRefs;
    }
    
    std::string Correlation::getPairwiseType() {
        return this->pairwiseType;
    }
    
    PharmML::AstNode *Correlation::getPairwiseAssignment() {
        return this->pairwiseAssignment;
    }
    
    Dependencies &Correlation::getDependencies() {
        return this->deps;
    }
    
    void Correlation::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
