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

namespace PharmML
{
    RandomVariable::RandomVariable(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->RandomVariable::parse(node);
    }

    void RandomVariable::parse(xml::Node node) {
        this->Symbol::parse(node);
        std::vector<xml::Node> ref_nodes = this->context->getElements(node, "./ct:VariabilityReference");
        for (xml::Node ref_node : ref_nodes) {
            PharmML::VariabilityReference *var_ref = new VariabilityReference(this->context, ref_node);
            this->variabilityReferences.push_back(var_ref);
        }
        xml::Node dist_node = this->context->getSingleElement(node, "./mdef:Distribution");
        if (dist_node.exists()) {
            PharmML::Distribution *distribution = new PharmML::Distribution(context, dist_node.getChild());
            this->Distribution = distribution;
        }
    }

    std::vector<VariabilityReference *> RandomVariable::getVariabilityReferences() {
        return this->variabilityReferences;
    }

    PharmML::Distribution *RandomVariable::getDistribution() {
        return this->Distribution;
    }


    void RandomVariable::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        for (DistributionParameter *par : this->Distribution->getDistributionParameters()) {
            this->setupAstSymbRefs(par->getAssignment(), gathering, blkId);
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
    AstNode *RandomVariable::initialStdev(std::vector<ParameterEstimation *> parameterEstimations) {
        for (DistributionParameter *param : this->Distribution->getDistributionParameters()) {
            if (param->getName() == "stdev") {
                AstAnalyzer analyzer;
                param->getAssignment()->accept(&analyzer);
                if (analyzer.getPureScalar()) {
                    return analyzer.getPureScalar();
                } else if (analyzer.getPureSymbRef()) {
                    Symbol *symbol = analyzer.getPureSymbRef()->getSymbol();
                    for (ParameterEstimation *pe : parameterEstimations) {
                        if (pe->getSymbRef()->getSymbol() == symbol) {
                            return pe->getInitValue();
                        }
                    }
                }
                return new ScalarInt(0);
            } else if (param->getName() == "var") {
                AstAnalyzer analyzer;
                param->getAssignment()->accept(&analyzer);
                if (analyzer.getPureScalar()) {
                    UniopSqrt *stdev = new UniopSqrt();
                    stdev->setChild(analyzer.getPureScalar());
                    return stdev;
                } else if (analyzer.getPureSymbRef()) {
                    Symbol *symbol = analyzer.getPureSymbRef()->getSymbol();
                    for (ParameterEstimation *pe : parameterEstimations) {
                        if (pe->getSymbRef()->getSymbol() == symbol) {
                            UniopSqrt *stdev = new UniopSqrt();
                            stdev->setChild(pe->getInitValue());
                            return stdev;
                        }
                    }
                }
                return new ScalarInt(0);
            }
        }
        return new ScalarInt(0);
    }
}
