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
    FixedEffect::FixedEffect(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->FixedEffect::parse(node);
    }

    void FixedEffect::parse(xml::Node node) {
        // Get either SymbRef or Scalar
        xml::Node symbref_node = this->context->getSingleElement(node, "./ct:SymbRef");
        if (symbref_node.exists()) {
            PharmML::SymbRef *symbRef = new PharmML::SymbRef(symbref_node);
            this->symbRef = symbRef;
        } else {
            xml::Node scalar_node = this->context->getSingleElement(node, "./ct:Scalar");
            PharmML::AstNode *scalar = this->context->factory.create(scalar_node);
            this->scalar = scalar;
        }

        // Get category (for categorical covariates)
        xml::Node cat_node = this->context->getSingleElement(node, "./mdef:Category");
        if (cat_node.exists()) {
            this->catId = cat_node.getAttribute("catId").getValue();
        }
    }

    PharmML::SymbRef *FixedEffect::getReference() {
        return this->symbRef;
    }

    PharmML::AstNode *FixedEffect::getScalar() {
        return this->scalar;
    }

    std::string FixedEffect::getCategory() {
        return this->catId;
    }

    Symbol *FixedEffect::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        if (this->symbRef) {
            Symbol *found_symbol = this->addSymbRef(symbRef, symbolMap);
            this->addReference(found_symbol);
            return found_symbol;
        }
        return nullptr;
    }

    IndividualParameter::IndividualParameter(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->IndividualParameter::parse(node);
        this->context->symbols[this->symbId] = this;
    }

    void IndividualParameter::parse(xml::Node node) {
        this->Symbol::parse(node);

        xml::Node structm_node = this->context->getSingleElement(node, "./mdef:StructuredModel");
        if (structm_node.exists()) {
            // Individual parameter is structured (type 2 or 3; general or linear)
            this->is_structured = true;

            // Get transformation if available
            xml::Node trans = this->context->getSingleElement(structm_node, "./mdef:Transformation");
            if (trans.exists()) {
                // Get transformation type (Box-Cox, identity, log, logit or probit)
                this->transformation = trans.getAttribute("type").getValue();

                // Get transformation parameters (if available)
                std::vector<xml::Node> trans_params = this->context->getElements(trans, "./ct:Parameter");
                for (xml::Node trans_param : trans_params) {
                    PharmML::AstNode *param = this->context->factory.create(trans_param.getChild());
                    this->transformationParameters.push_back(param);
                }
            }

            // Get structured type
            xml::Node pop = this->context->getSingleElement(structm_node, "./mdef:PopulationValue");
            xml::Node lin = this->context->getSingleElement(structm_node, "./mdef:LinearCovariate");
            xml::Node gen = this->context->getSingleElement(structm_node, "./mdef:GeneralCovariate");
            if (pop.exists()) {
                // Only a population value -- type 3 (without fixed effects)
                this->is_linear_cov = true;

                // Get population value
                xml::Node assign = this->context->getSingleElement(pop, "./ct:Assign");
                this->populationValue = this->context->factory.create(assign.getChild());
            } else if (lin.exists()) {
                // Linear covariate model -- type 3
                this->is_linear_cov = true;

                // Get population value
                xml::Node assign = this->context->getSingleElement(lin, "./mdef:PopulationValue/ct:Assign");
                this->populationValue = this->context->factory.create(assign.getChild());

                // Get covariate terms
                std::vector<xml::Node> cov_nodes = this->context->getElements(lin, "./mdef:Covariate");
                for (xml::Node cov_node : cov_nodes) {
                    // Get SymbRef (to the covariate)
                    xml::Node symbref_node = this->context->getSingleElement(cov_node, "./ct:SymbRef");
                    PharmML::SymbRef *cov_symbref = new PharmML::SymbRef(symbref_node);
                    this->covariates.push_back(cov_symbref);

                    // Get fixed effects (unlimited amount per covariate)
                    std::vector<xml::Node> fixed_nodes = this->context->getElements(cov_node, "./mdef:FixedEffect");
                    this->fixedEffects.emplace(cov_symbref, std::vector<FixedEffect *>()); // Need to initialize vector first (insert and std::make_pair if not C++11)
                    for (xml::Node fixed_node : fixed_nodes) {
                        // Create fixed effect object
                        FixedEffect *fixed_effect = new FixedEffect(this->context, fixed_node);
                        // Store fixed effect object in vector under associative array (of covariate SymbRef)
                        this->fixedEffects[cov_symbref].push_back(fixed_effect);
                    }
                }
            } else if (gen.exists()) {
                // General covariate model -- type 2
                this->is_general_cov = true;

                // Get population value
                xml::Node assign = this->context->getSingleElement(gen, "./mdef:PopulationValue/ct:Assign");
                this->populationValue = this->context->factory.create(assign.getChild());

                // Get general covariate definition (should not contain any random variable references)
                assign = this->context->getSingleElement(gen, "./ct:Assign");
                this->generalAssignment = this->context->factory.create(assign);
            }

            // Finally, get the random effects
            std::vector<xml::Node> rand_nodes = this->context->getElements(structm_node, "./mdef:RandomEffects");
            for (xml::Node rand_node : rand_nodes) {
                // Get SymbRef (to the covariate)
                xml::Node symbref_node = this->context->getSingleElement(rand_node, "./ct:SymbRef");
                PharmML::SymbRef *symbRef = new PharmML::SymbRef(symbref_node);
                this->randomEffects.push_back(symbRef);
            }
        } else {
            // Individual parameter is NOT structured (type 1 or 4; explicit or distribution)
            this->is_structured = false;

            xml::Node assign_node = this->context->getSingleElement(node, "./ct:Assign");
            if (assign_node.exists()) {
                // Explicit covariate model -- type 1
                this->is_explicit_cov = true;

                // Get explicit assignment (can contain references to random variables)
                this->explicitAssignment = this->context->factory.create(assign_node.getChild());
            } else {
                // Generic distributional ("eta-free") covariate model -- type 4
                this->is_generic_cov = true;

                // TODO: Support type 4
            }
        }
    }

    bool IndividualParameter::isStructured() {
        return this->is_structured;
    }

    bool IndividualParameter::isLinear() {
        return this->is_linear_cov;
    }

    bool IndividualParameter::isGeneral() {
        return this->is_general_cov;
    }

    bool IndividualParameter::isExplicit() {
        return this->is_explicit_cov;
    }

    bool IndividualParameter::isGeneric() {
        return this->is_generic_cov;
    }

    std::string IndividualParameter::getTransformation() {
        return this->transformation;
    }

    PharmML::AstNode *IndividualParameter::getPopulationValue() {
        return this->populationValue;
    }

    std::vector<PharmML::SymbRef *> IndividualParameter::getCovariates() {
        return this->covariates;
    }

    std::vector<FixedEffect *> IndividualParameter::getFixedEffects(SymbRef *covariate) {
        return this->fixedEffects[covariate];
    }

    std::vector<FixedEffect *> IndividualParameter::getFixedEffects(Symbol *covariate) {
        PharmML::SymbRef *symbRef = nullptr;
        for (SymbRef *cov : this->covariates) {
            if (cov->getSymbol() == covariate) {
                symbRef = cov;
            }
        }
        return this->fixedEffects[symbRef];
    }

    std::vector<PharmML::SymbRef *> IndividualParameter::getRandomEffects() {
        return this->randomEffects;
    }

    // FIXME: This could in the future create an assignment for the full expression given a structured model
    PharmML::AstNode *IndividualParameter::getAssignment() {
        if (this->is_general_cov) {
            return this->generalAssignment;
        } else if (this->is_explicit_cov) {
            return this->explicitAssignment;
        }
        return nullptr;
    }

    void IndividualParameter::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        for (PharmML::AstNode *trans_param : this->transformationParameters) {
            this->symbRefsFromAst(trans_param, symbolMap);
        }
        if (this->populationValue) {
            this->symbRefsFromAst(this->populationValue, symbolMap);
        }
        for (SymbRef *cov : this->covariates) {
            Symbol *found_symbol = this->addSymbRef(cov, symbolMap);
            this->addReference(found_symbol);
            for (FixedEffect *fixed_eff : this->fixedEffects[cov]) {
                this->addReference(fixed_eff->gatherSymbRefs(symbolMap));
            }
        }
        for (SymbRef *rand_effect : this->randomEffects) {
            Symbol *found_symbol = this->addSymbRef(rand_effect, symbolMap);
            this->addReference(found_symbol);
        }
        if (this->generalAssignment) {
            this->symbRefsFromAst(this->generalAssignment, symbolMap);
        }
        if (this->explicitAssignment) {
            this->symbRefsFromAst(this->explicitAssignment, symbolMap);
        }
    }

    void IndividualParameter::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void IndividualParameter::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }
}
