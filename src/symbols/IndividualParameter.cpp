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

#include "IndividualParameter.h"
#include <iostream>

namespace PharmML
{
    IndividualParameter::IndividualParameter(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->IndividualParameter::parse(node);
        this->context->symbols[this->symbId] = this;
    }

    void IndividualParameter::parse(xml::Node node) {
        this->Symbol::parse(node);

        xml::Node structured_model = this->context->getSingleElement(node, ".//mdef:StructuredModel");
        if (structured_model.exists()) {
            xml::Node trans = this->context->getSingleElement(node, ".//mdef:StructuredModel/mdef:Transformation");
            if (trans.exists()) {
                this->transformation = trans.getAttribute("type").getValue();
            }

            xml::Node pop = this->context->getSingleElement(node, ".//mdef:StructuredModel/mdef:LinearCovariate/mdef:PopulationValue/ct:Assign");
            if (!pop.exists()) {
                pop = this->context->getSingleElement(node, ".//mdef:StructuredModel/mdef:PopulationValue/ct:Assign");
            }
            if (pop.exists()) {
                this->PopulationValue = this->context->factory.create(pop.getChild(), &deps);
            }

            xml::Node rand = this->context->getSingleElement(node, ".//mdef:StructuredModel/mdef:RandomEffects/ct:SymbRef");
            if (rand.exists()) {
                this->RandomEffects = this->context->factory.create(rand, &deps);
            }

            xml::Node fixed = this->context->getSingleElement(node, ".//mdef:StructuredModel/mdef:LinearCovariate/mdef:Covariate/mdef:FixedEffect/ct:SymbRef");
            if (fixed.exists()) {
                this->FixedEffect = this->context->factory.create(fixed, &deps);
            }

            xml::Node cov = this->context->getSingleElement(node, ".//mdef:StructuredModel/mdef:LinearCovariate/mdef:Covariate/ct:SymbRef");
            if (cov.exists()) {
                this->Covariate = this->context->factory.create(cov, &deps);
            }
            this->is_structured = true;
        } else {
            this->assignment = this->context->factory.create(node.getChild().getChild(), &deps);
            this->is_structured = false;
        }
    }

    std::string IndividualParameter::getTransformation() {
        return this->transformation;
    }

    AstNode *IndividualParameter::getPopulationValue() {
        return this->PopulationValue;
    }
    
    AstNode *IndividualParameter::getCovariate() {
        return this->Covariate;
    }

    AstNode *IndividualParameter::getRandomEffects() {
        return this->RandomEffects;
    }

    AstNode *IndividualParameter::getFixedEffect() {
        return this->FixedEffect;
    }
    
    // FIXME: This could in the future create an assignment for the full expression given a structured model
    AstNode *IndividualParameter::getAssignment() {
        return this->assignment;
    }

    bool IndividualParameter::isStructured() {
        return this->is_structured;
    }
    
    PharmML::Dependencies &IndividualParameter::getDependencies() {
        return this->deps;
    }


    void IndividualParameter::gatherSymbRefs(std::unordered_map<std::string, Symbol *> symbolMap) {
        if (is_structured) {

        } else {

        }
    }

    void IndividualParameter::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void IndividualParameter::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }
}
