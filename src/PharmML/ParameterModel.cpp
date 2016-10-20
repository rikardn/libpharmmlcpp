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

#include "ParameterModel.h"
#include <visitors/AstAnalyzer.h>
#include <AST/AstBuilder.h>
#include <iostream>

namespace pharmmlcpp
{
    ParameterModel::ParameterModel(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void ParameterModel::parse(PharmMLReader &reader, xml::Node node) {
        this->Block::parse(node);
        std::vector<xml::Node> param_nodes = reader.getElements(node, "./mdef:Parameter");
        for (xml::Node n : param_nodes) {
            Parameter *param = new Parameter(reader, n);
            this->parameters.push_back(param);
        }

        std::vector<xml::Node> pop_nodes = reader.getElements(node, "./mdef:PopulationParameter");
        for (xml::Node n : pop_nodes) {
            pharmmlcpp::PopulationParameter *pop = new PopulationParameter(reader, n);
            this->populationParameters.push_back(pop);
        }

        std::vector<xml::Node> ind_nodes = reader.getElements(node, "./mdef:IndividualParameter");
        for (xml::Node n : ind_nodes) {
            pharmmlcpp::IndividualParameter *ind = new IndividualParameter(reader, n);
            this->individualParameters.push_back(ind);
        }

        std::vector<xml::Node> rand_nodes = reader.getElements(node, "./mdef:RandomVariable");
        for (xml::Node n : rand_nodes) {
            pharmmlcpp::RandomVariable *ind = new RandomVariable(reader, n);
            this->randomVariables.push_back(ind);
        }

        std::vector<xml::Node> corr_nodes = reader.getElements(node, "./mdef:Correlation");
        for (xml::Node n : corr_nodes) {
            pharmmlcpp::Correlation *ind = new Correlation(reader, n);
            this->correlations.push_back(ind);
        }
    }

    xml::Node ParameterModel::xml(PharmMLWriter &writer) {
        xml::Node pmod("ParameterModel");
        this->Block::xml(writer, pmod);

        for (const auto &popparam : this->populationParameters) {
            pmod.addChild(popparam->xml(writer));
        }

        for (const auto &indparam : this->individualParameters) {
            pmod.addChild(indparam->xml(writer));
        }

        return pmod;
    }

    void ParameterModel::setupRefererSymbRefs(SymbolGathering &gathering) {
        // Only Correlation in ParameterModel are Referer's (and not Symbol's)
        for (pharmmlcpp::Correlation *corr : this->getCorrelations()) {
            corr->setupSymbRefs(gathering, this->getBlkId());
        }
    }

    void ParameterModel::gatherSymbols(SymbolGathering &gatherer) {
        gatherer.newBlock(this);
        for (Parameter *parameter : this->parameters) {
            gatherer.addSymbol(parameter);
        }
        for (PopulationParameter *parameter : this->populationParameters) {
            gatherer.addSymbol(parameter);
        }
        for (IndividualParameter *parameter : this->individualParameters) {
            gatherer.addSymbol(parameter);
        }
        for (RandomVariable *rv : this->randomVariables) {
            gatherer.addSymbol(rv);
        }
    }

    std::vector<Parameter *> ParameterModel::getParameters() {
        return this->parameters;
    }

    std::vector<PopulationParameter *> ParameterModel::getPopulationParameters() {
        return this->populationParameters;
    }

    std::vector<IndividualParameter *> ParameterModel::getIndividualParameters() {
        return this->individualParameters;
    }

    std::vector<RandomVariable *> ParameterModel::getRandomVariables() {
        return this->randomVariables;
    }

    std::vector<Correlation *> ParameterModel::getCorrelations() {
        return this->correlations;
    }

    // Return the the initial covariance between var1 and var2 given a vector of parameterEstimations
    // FIXME: How to know which correlations are applicable? Can correlations from other ParameterModel apply?
    std::unique_ptr<AstNode> ParameterModel::initialCovariance(RandomVariable *var1, RandomVariable *var2, std::vector<ParameterEstimation *> parameterEstimations) {
        for (Correlation *correlation : this->correlations) {
            if (correlation->isPairwise()) {
                Symbol *symbol_1 = correlation->getPairwiseSymbRefs()[0]->getSymbol();
                Symbol *symbol_2 = correlation->getPairwiseSymbRefs()[1]->getSymbol();
                // Is this the correlation we are searching for?
                if ((symbol_1 == var1 && symbol_2 == var2) || (symbol_2 == var1 && symbol_1 == var2)) {
                    std::shared_ptr<AstNode> assignment = correlation->getPairwiseAssignment();
                    AstAnalyzer analyzer;
                    assignment->accept(&analyzer);
                    if (analyzer.getPureScalar() && correlation->getPairwiseType() == "Covariance") {
                        // A scalar covariance
                        return std::move(analyzer.getPureScalar()->clone());
                    } else if (analyzer.getPureScalar() && correlation->getPairwiseType() == "CorrelationCoefficient") {
                        // A scalar correlation coefficient
                        // cov(X,Y) = cor(X,Y) * stdev(X) * stdev(Y)
                        // Have method initialStdev on a RandomVariable with parameterEstimations as argument
                        std::vector<std::unique_ptr<AstNode>> multiplicands;
                        multiplicands.push_back(std::move(analyzer.getPureScalar()->clone()));
                        multiplicands.push_back(std::move(var1->initialStdev(parameterEstimations)));
                        multiplicands.push_back(std::move(var2->initialStdev(parameterEstimations)));
                        std::unique_ptr<AstNode> cov = AstBuilder::multiplyMany(multiplicands);
                        return std::move(cov);
                    } else if (analyzer.getPureSymbRef() && correlation->getPairwiseType() == "Covariance") {
                        Symbol *symbol = analyzer.getPureSymbRef()->getSymbol();
                        // FIXME: Better way of parameterEstimations lookup
                        for (ParameterEstimation *pe : parameterEstimations) {
                            if (pe->getSymbRef()->getSymbol() == symbol) {
                                return std::move(pe->getInitValue()->clone());
                            }
                        }
                        return std::make_unique<ScalarInt>(0);    // FIXME: What to do when we cannot find initial value
                    } else if (analyzer.getPureSymbRef() && correlation->getPairwiseType() == "CorrelationCoefficient") {
                        Symbol *symbol = analyzer.getPureSymbRef()->getSymbol();
                        for (ParameterEstimation *pe : parameterEstimations) {
                            if (pe->getSymbRef()->getSymbol() == symbol) {
                                std::vector<std::unique_ptr<AstNode>> multiplicands;
                                multiplicands.push_back(std::move(pe->getInitValue()->clone()));
                                multiplicands.push_back(std::move(var1->initialStdev(parameterEstimations)));
                                multiplicands.push_back(std::move(var2->initialStdev(parameterEstimations)));
                                return std::move(AstBuilder::multiplyMany(multiplicands));
                            }
                        }
                        return std::make_unique<ScalarInt>(0);    // FIXME: What to do when we cannot find initial value
                    }
                }
            } else {
                // TODO
                return nullptr;
            }
        }
        return std::make_unique<ScalarInt>(0);        // No covariance
    }

    /* THOUGHTS ON IMPLEMENTATION BELOW:
     * postParse() for linking refering RandomVariable, IndividualParameter and Correlation to PopulationParameter
     * does not work since SymbRef's are not setup at construction time AND it would mean cache invalidation mess if
     * internals are accessed!
     *
     * FIXME: What kind of pointer list type to return? A reference to source object is not senseful since
     * none such exists; list is subset of source object. Shared pointer list? */

    // Get RandomVariable's referencing a given Symbol (if PopulationParameter: (possibly?) a variability parameter)
    std::vector<RandomVariable *> ParameterModel::getRandomVariables(Symbol *symbol) {
        std::vector<RandomVariable *> rand_vars;
        for (RandomVariable *rand_var : this->randomVariables) {
            bool depends_on_symbol = rand_var->referencedSymbols.hasSymbol(symbol);
            if (depends_on_symbol) {
                rand_vars.push_back(rand_var);
            }
        }
        return rand_vars;
    }

    // Get IndividualParameter's referencing a given Symbol (if PopulationParameter: it's a structural parameter)
    std::vector<IndividualParameter *> ParameterModel::getIndividualParameters(Symbol *symbol) {
        std::vector<IndividualParameter *> ind_params;
        for (IndividualParameter *ind_param : this->individualParameters) {
            bool depends_on_symbol = ind_param->referencedSymbols.hasSymbol(symbol);
            if (depends_on_symbol) {
                ind_params.push_back(ind_param);
            }
        }
        return ind_params;
    }

    // Get Correlation's referencing a given Symbol (if PopulationParameter: it's a variability/correlation parameter)
    std::vector<Correlation *> ParameterModel::getCorrelations(Symbol *symbol) {
        std::vector<Correlation *> corrs;
        for (Correlation *corr : this->correlations) {
            bool depends_on_symbol = corr->referencedSymbols.hasSymbol(symbol);
            if (depends_on_symbol) {
                corrs.push_back(corr);
            }
        }
        return corrs;
    }

    // Get RandomVariables on a specific VariabilityLevel
    std::vector<RandomVariable *> ParameterModel::getRandomVariablesOnLevel(VariabilityLevel *level) {
        std::vector<RandomVariable *> rand_vars;
        for (RandomVariable *rand_var : this->randomVariables) {
            for (VariabilityReference *var_ref : rand_var->getVariabilityReferences()) {
                if (var_ref->referencedSymbols.hasSymbol(level)) {
                    rand_vars.push_back(rand_var);
                }
            }
        }
        return rand_vars;
    }

    // Get Correlations on a specific VariabilityLevel
    std::vector<Correlation *> ParameterModel::getCorrelationsOnLevel(VariabilityLevel *level) {
        std::vector<Correlation *> corrs;
        for (Correlation *corr : this->correlations) {
            VariabilityReference *var_ref = corr->getVariabilityReference();
            if (var_ref->referencedSymbols.hasSymbol(level)) {
                corrs.push_back(corr);
            }
        }
        return corrs;
    }
}
