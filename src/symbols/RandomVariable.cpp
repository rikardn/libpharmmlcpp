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

#include "RandomVariable.h"
#include <visitors/AstAnalyzer.h>

namespace pharmmlcpp
{
    RandomVariable::RandomVariable(PharmMLReader &reader, xml::Node node) {
        this->RandomVariable::parse(reader, node);
    }

    void RandomVariable::parse(PharmMLReader &reader, xml::Node node) {
        this->Symbol::parse(node);
        std::vector<xml::Node> ref_nodes = reader.getElements(node, "./ct:VariabilityReference");
        for (xml::Node ref_node : ref_nodes) {
            pharmmlcpp::VariabilityReference *var_ref = new VariabilityReference(reader, ref_node);
            this->variabilityReferences.push_back(var_ref);
        }
        xml::Node dist_node = reader.getSingleElement(node, "./mdef:Distribution");
        if (dist_node.exists()) {
            std::shared_ptr<Distribution> dist = std::make_shared<Distribution>(reader, dist_node.getChild());
            this->distribution = dist;
        }
    }

    std::vector<VariabilityReference *> RandomVariable::getVariabilityReferences() {
        return this->variabilityReferences;
    }

    std::shared_ptr<Distribution> RandomVariable::getDistribution() {
        return this->distribution;
    }

    void RandomVariable::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        if (this->distribution) {
            // Get all (possibly nested) parameters in the distribution and setup symbrefs
            std::vector<std::shared_ptr<DistributionParameter>> all_params = this->distribution->getAllDistributionParameters();
            for (auto & param : all_params) {
                this->setupAstSymbRefs(param->getAssignment().get(), gathering, blkId);
            }
        }
        for (VariabilityReference *var_ref : this->getVariabilityReferences()) {
            var_ref->setupSymbRefs(gathering, blkId);
        }
    }

    void RandomVariable::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void RandomVariable::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }

    // get the initial value for the stdev.
    // FIXME: Check which distribution
    std::unique_ptr<AstNode> RandomVariable::initialStdev(std::vector<ParameterEstimation *> parameterEstimations) {
        for (auto const &param : this->distribution->getDistributionParameters()) {
            if (param->getName() == "stdev") {
                AstAnalyzer analyzer;
                param->getAssignment()->accept(&analyzer);
                if (analyzer.getPureScalar()) {
                    return std::move(analyzer.getPureScalar()->clone());
                } else if (analyzer.getPureSymbRef()) {
                    Symbol *symbol = analyzer.getPureSymbRef()->getSymbol();
                    for (ParameterEstimation *pe : parameterEstimations) {
                        if (pe->getSymbRef()->getSymbol() == symbol) {
                            return std::move(pe->getInitValue()->clone());
                        }
                    }
                }
                return std::make_unique<ScalarInt>(0);
            } else if (param->getName() == "var") {
                AstAnalyzer analyzer;
                param->getAssignment()->accept(&analyzer);
                if (analyzer.getPureScalar()) {
                    std::unique_ptr<AstNode> child = std::move(analyzer.getPureScalar()->clone());
                    std::unique_ptr<AstNode> stdev = std::make_unique<UniopSqrt>(std::move(child));
                    return std::move(stdev);
                } else if (analyzer.getPureSymbRef()) {
                    Symbol *symbol = analyzer.getPureSymbRef()->getSymbol();
                    for (ParameterEstimation *pe : parameterEstimations) {
                        if (pe->getSymbRef()->getSymbol() == symbol) {
                            std::unique_ptr<AstNode> child = std::move(pe->getInitValue()->clone());
                            std::unique_ptr<AstNode> stdev = std::make_unique<UniopSqrt>(std::move(child));
                            return std::move(stdev);
                        }
                    }
                }
                return std::make_unique<ScalarInt>(0);
            }
        }
        return std::make_unique<ScalarInt>(0);
    }
}
