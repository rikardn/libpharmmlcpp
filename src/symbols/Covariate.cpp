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
#include <symbols/Covariate.h>
#include <PharmML/PharmMLContext.h>
#include <AST/AstNodeFactory.h>
#include <AST/AstNode.h>
#include <PharmML/Distribution.h>

namespace PharmML
{
    Covariate::Covariate(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    // Constructor for transformed covariates
    Covariate::Covariate(PharmMLContext *context, xml::Node name_node, xml::Node assign_node) {
        this->context = context;
        this->Symbol::parse(name_node);
        xml::Node tree = assign_node.getChild();
        this->assignment = this->context->factory.create(tree);
        this->transformed = true;
    }

    void Covariate::parse(xml::Node node) {
        this->Symbol::parse(node);

        // Get type (timeDependent, occasionDependent or constant)
        this->type = node.getAttribute("type").getValue();

        // Get continuous/categorical type
        xml::Node cont_node = this->context->getSingleElement(node, "./mdef:Continuous");
        if (cont_node.exists()) {
            this->continuous = true;

            // Get distribution/realization
            xml::Node dist_node = this->context->getSingleElement(cont_node, "./mdef:Distribution");
            if (dist_node.exists()) {
                this->distribution = new PharmML::Distribution(this->context, dist_node);
            }
            // TODO: Support realization of distribution (also, in general)
            //xml::Node real_node = this->context->getSingleElement(cont_node, "./mdef:Realization");

            // Get transformations
            std::vector<xml::Node> trans_nodes = this->context->getElements(cont_node, "./mdef:Transformation");
            for (xml::Node trans_node : trans_nodes) {
                // Create new covariate for each transformation
                xml::Node name_node = this->context->getSingleElement(trans_node, "./mdef:TransformedCovariate");
                xml::Node assign_node = this->context->getSingleElement(trans_node, "./ct:Assign");
                Covariate *new_cov = new Covariate(this->context, name_node, assign_node);
                this->transformations.push_back(new_cov);
            }

            // TODO: Get interpolation
            //xml::Node int_node = this->context->getSingleElement(cont_node, "./ct:Interpolation");

            // Get assign (likely for constants)
            xml::Node assign = this->context->getSingleElement(cont_node, "./ct:Assign");
            if (assign.exists()) {
                xml::Node tree = assign.getChild();
                this->assignment = this->context->factory.create(tree);
            }
        } else {
            this->continuous = false;

            // TODO: Categorical covariate support
        }
    }

    bool Covariate::isTransformed() {
        return this->transformed;
    }

    bool Covariate::isContinuous() {
        return this->continuous;
    }

    std::string Covariate::getType() {
        return this->type;
    }

    PharmML::Distribution *Covariate::getDistribution() {
        return this->distribution;
    }

    std::vector<Covariate *> Covariate::getTransformations() {
        return this->transformations;
    }

    AstNode *Covariate::getAssignment() {
        return this->assignment;
    }

    void Covariate::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        if (this->assignment) {
            std::unordered_set<Symbol *> found_symbols = this->symbRefsFromAst(this->assignment, symbolMap);
            this->addReferences(found_symbols);
        }
        for (Covariate *cov : this->transformations) {
            cov->gatherSymbRefs(symbolMap);
        }
    }

    void Covariate::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void Covariate::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }
}
