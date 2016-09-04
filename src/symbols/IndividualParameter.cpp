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
#include <AST/Uniop.h>
#include <AST/Binop.h>
#include <AST/AstBuilder.h>

namespace pharmmlcpp
{
    FixedEffect::FixedEffect(PharmMLReader &reader, xml::Node node) {
        this->FixedEffect::parse(reader, node);
    }

    void FixedEffect::parse(PharmMLReader &reader, xml::Node node) {
        // Get either SymbRef or Scalar
        xml::Node symbref_node = reader.getSingleElement(node, "./ct:SymbRef");
        if (symbref_node.exists()) {
            SymbRef *symbRef = new SymbRef(symbref_node);
            this->symbRef = symbRef;
        } else {
            xml::Node scalar_node = reader.getSingleElement(node, "./ct:Scalar");
            this->scalar = reader.factory.create(reader, scalar_node);
        }

        // Get category (for categorical covariates)
        xml::Node cat_node = reader.getSingleElement(node, "./mdef:Category");
        if (cat_node.exists()) {
            this->catId = cat_node.getAttribute("catId").getValue();
        }
    }

    SymbRef *FixedEffect::getReference() {
        return this->symbRef;
    }

    std::shared_ptr<AstNode> FixedEffect::getScalar() {
        return this->scalar;
    }

    std::string FixedEffect::getCategory() {
        return this->catId;
    }


    void FixedEffect::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        if (this->symbRef) {
            this->addSymbRef(symbRef, gathering, blkId);
        }
    }

    IndividualParameter::IndividualParameter(PharmMLReader &reader, xml::Node node) {
        this->IndividualParameter::parse(reader, node);
    }

    void IndividualParameter::parse(PharmMLReader &reader, xml::Node node) {
        this->Symbol::parse(node);

        xml::Node structm_node = reader.getSingleElement(node, "./mdef:StructuredModel");
        if (structm_node.exists()) {
            // Individual parameter is structured (type 2 or 3; general or linear)
            this->is_structured = true;

            // Get transformation if available
            xml::Node trans = reader.getSingleElement(structm_node, "./mdef:Transformation");
            if (trans.exists()) {
                // Get transformation type (Box-Cox, identity, log, logit or probit)
                this->transformation = trans.getAttribute("type").getValue();

                // Get transformation parameters (if available)
                std::vector<xml::Node> trans_params = reader.getElements(trans, "./ct:Parameter");
                for (xml::Node trans_param : trans_params) {
                    std::shared_ptr<AstNode> param = reader.factory.create(reader, trans_param.getChild());
                    this->transformationParameters.push_back(param);
                }
            }

            // Get structured type
            xml::Node pop = reader.getSingleElement(structm_node, "./mdef:PopulationValue");
            xml::Node lin = reader.getSingleElement(structm_node, "./mdef:LinearCovariate");
            xml::Node gen = reader.getSingleElement(structm_node, "./mdef:GeneralCovariate");
            if (pop.exists()) {
                // Only a population value -- type 3 (without fixed effects)
                this->is_linear_cov = true;

                // Get population value
                xml::Node assign = reader.getSingleElement(pop, "./ct:Assign");
                this->populationValue = reader.factory.create(reader, assign.getChild());
            } else if (lin.exists()) {
                // Linear covariate model -- type 3
                this->is_linear_cov = true;

                // Get population value
                xml::Node assign = reader.getSingleElement(lin, "./mdef:PopulationValue/ct:Assign");
                this->populationValue = reader.factory.create(reader, assign.getChild());

                // Get covariate terms
                std::vector<xml::Node> cov_nodes = reader.getElements(lin, "./mdef:Covariate");
                for (xml::Node cov_node : cov_nodes) {
                    // Get SymbRef (to the covariate)
                    xml::Node symbref_node = reader.getSingleElement(cov_node, "./ct:SymbRef");
                    pharmmlcpp::SymbRef *cov_symbref = new SymbRef(symbref_node);
                    this->covariates.push_back(cov_symbref);

                    // Get fixed effects (unlimited amount per covariate)
                    std::vector<xml::Node> fixed_nodes = reader.getElements(cov_node, "./mdef:FixedEffect");
                    this->fixedEffects.emplace(cov_symbref, std::vector<FixedEffect *>()); // Need to initialize vector first (insert and std::make_pair if not C++11)
                    for (xml::Node fixed_node : fixed_nodes) {
                        // Create fixed effect object
                        FixedEffect *fixed_effect = new FixedEffect(reader, fixed_node);
                        // Store fixed effect object in vector under associative array (of covariate SymbRef)
                        this->fixedEffects[cov_symbref].push_back(fixed_effect);
                    }
                }
            } else if (gen.exists()) {
                // General covariate model -- type 2
                this->is_general_cov = true;

                // Get population value
                xml::Node assign = reader.getSingleElement(gen, "./mdef:PopulationValue/ct:Assign");
                this->populationValue = reader.factory.create(reader, assign.getChild());

                // Get general covariate definition (should not contain any random variable references)
                assign = reader.getSingleElement(gen, "./ct:Assign");
                this->generalAssignment = reader.factory.create(reader, assign);
            }

            // Finally, get the random effects
            std::vector<xml::Node> rand_nodes = reader.getElements(structm_node, "./mdef:RandomEffects");
            for (xml::Node rand_node : rand_nodes) {
                // Get SymbRef (to the covariate)
                xml::Node symbref_node = reader.getSingleElement(rand_node, "./ct:SymbRef");
                pharmmlcpp::SymbRef *symbRef = new SymbRef(symbref_node);
                this->randomEffects.push_back(symbRef);
            }
        } else {
            // Individual parameter is NOT structured (type 1 or 4; explicit or distribution)
            this->is_structured = false;

            xml::Node assign_node = reader.getSingleElement(node, "./ct:Assign");
            if (assign_node.exists()) {
                // Explicit covariate model -- type 1
                this->is_explicit_cov = true;

                // Get explicit assignment (can contain references to random variables)
                this->explicitAssignment = reader.factory.create(reader, assign_node.getChild());
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

    std::shared_ptr<AstNode> IndividualParameter::getPopulationValue() {
        return this->populationValue;
    }

    std::vector<pharmmlcpp::SymbRef *> IndividualParameter::getCovariates() {
        return this->covariates;
    }

    std::vector<FixedEffect *> IndividualParameter::getFixedEffects(SymbRef *covariate) {
        return this->fixedEffects[covariate];
    }

    std::vector<FixedEffect *> IndividualParameter::getFixedEffects(Symbol *covariate) {
        pharmmlcpp::SymbRef *symbRef = nullptr;
        for (SymbRef *cov : this->covariates) {
            if (cov->getSymbol() == covariate) {
                symbRef = cov;
            }
        }
        return this->fixedEffects[symbRef];
    }

    std::vector<pharmmlcpp::SymbRef *> IndividualParameter::getRandomEffects() {
        return this->randomEffects;
    }

    std::shared_ptr<AstNode> IndividualParameter::getAssignment() {
        if (this->is_general_cov) {
            return this->generalAssignment;
        } else if (this->is_explicit_cov) {
            return this->explicitAssignment;
        }
        return std::shared_ptr<AstNode>(nullptr);
    }

    // Convert any structure of parameter into general form
    // FIXME: Currently support only explicit and structured and only log transform
    AstNode *IndividualParameter::asExplicit() {
        AstNode *result;
        if (this->isExplicit()) {
            result = this->getAssignment().get();
        } else {
            // trans^-1(  trans(pop) + fixedeff1*fixedeff2*covariate + ... + randomeffect1 + randomeffect2)
/* FIXME: This is a regression. Need copy methods to do this right!
              if (this->transformation == "log") {
                // FIXME: Smart pointers. Need to copy nodes?
                std::vector<AstNode *> addition_nodes;

                UniopLog *log_node = new UniopLog();
                log_node->setChild(this->getPopulationValue());
                addition_nodes.push_back(log_node);

                for (SymbRef *covariate : this->getCovariates()) {
                    std::vector<AstNode *> product_nodes;
                    for (FixedEffect *fe : this->getFixedEffects(covariate)) {
                        product_nodes.push_back(fe->getReference());
                    }
                    product_nodes.push_back(covariate);
                    addition_nodes.push_back(AstBuilder::multiplyMany(product_nodes));
                }

                for (SymbRef *random_effect : this->getRandomEffects()) {
                    addition_nodes.push_back(random_effect);
                }

                auto exp_node = std::make_unique<UniopExp>();
                exp_node->setChild(std::unique<AstNode>(AstBuilder::addMany(addition_nodes)));
                result = exp_node.get(); 
            }*/
        }

        return result;
    }

    void IndividualParameter::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        for (auto trans_param : this->transformationParameters) {
            this->setupAstSymbRefs(trans_param.get(), gathering, blkId);
        }
        if (this->populationValue) {
            this->setupAstSymbRefs(this->populationValue.get(), gathering, blkId);
        }
        for (SymbRef *cov : this->covariates) {
            this->addSymbRef(cov, gathering, blkId);
            for (FixedEffect *fixed_eff : this->fixedEffects[cov]) {
                fixed_eff->setupSymbRefs(gathering, blkId);
                this->addReference(*(fixed_eff->referencedSymbols.begin()));
            }
        }
        for (SymbRef *rand_effect : this->randomEffects) {
            this->addSymbRef(rand_effect, gathering, blkId);
        }
        if (this->generalAssignment) {
            this->setupAstSymbRefs(this->generalAssignment.get(), gathering, blkId);
        }
        if (this->explicitAssignment) {
            this->setupAstSymbRefs(this->explicitAssignment.get(), gathering, blkId);
        }
    }

    void IndividualParameter::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void IndividualParameter::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }
}
