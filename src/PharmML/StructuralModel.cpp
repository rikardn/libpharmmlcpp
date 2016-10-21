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
#include <algorithm>
#include "StructuralModel.h"
#include <symbols/SymbolGathering.h>
#include <PharmML/PharmML.h>

namespace pharmmlcpp
{
    StructuralModel::StructuralModel(PharmMLReader &reader, xml::Node node) {
        this->Block::parse(node);
        this->parse(reader, node);
    }

    void StructuralModel::parse(PharmMLReader &reader, xml::Node node) {
        std::vector<xml::Node> array = reader.getElements(node, ".//ct:Variable");
        for (xml::Node n : array) {
            Variable *var = new Variable(reader, n);
            this->variables.push_back(var);
        }
        std::vector<xml::Node> derivs = reader.getElements(node, ".//ct:DerivativeVariable");
        for (xml::Node n : derivs) {
            DerivativeVariable *var = new DerivativeVariable(reader, n);
            this->variables.push_back(var);
        }
        // Construct PKMacros object if macros are available
        xml::Node macros_node = reader.getSingleElement(node, "./mdef:PKmacros");
        if (macros_node.exists()) {
            PKMacros *macros = new PKMacros(reader, macros_node);
            this->pk_macros = macros;
        }
    }

    xml::Node StructuralModel::xml(PharmMLWriter &writer) {
        xml::Node str("StructuralModel");
        this->Block::xml(writer, str);

        for (const auto &var : this->variables) {
            str.addChild(var->xml(writer));
        }

        return str;
    }

    std::vector<CommonVariable *> StructuralModel::getVariables() {
        return this->variables;
    }

    bool StructuralModel::hasDerivatives() {
        return this->getDerivatives().size() > 0;
    }

    // Return all derivative variables
    std::vector<CommonVariable *> StructuralModel::getDerivatives() {
        std::vector<CommonVariable *> derivs;
        for (CommonVariable *var : this->variables) {
            if (var->isDerivative()) {
                derivs.push_back(var);
            }
        }

        return derivs;
    }

    pharmmlcpp::PKMacros *StructuralModel::getPKMacros() {
        return this->pk_macros;
    }

    void StructuralModel::setupRefererSymbRefs(SymbolGathering &gathering) {
        if (this->pk_macros) {
            this->pk_macros->setupRefererSymbRefs(gathering, this->getBlkId());
        }
    }

    void StructuralModel::gatherSymbols(SymbolGathering &gathering) {
        gathering.newBlock(this);
        for (CommonVariable *common_variable : this->variables) {
            gathering.addSymbol(common_variable);
        }
    }

    void StructuralModel::gatherMacros(MacroGathering &gatherer) {
        if (this->pk_macros) {
            gatherer.newBlock(this);
            for (PKMacro *macro : this->pk_macros->getMacros()) {
                gatherer.addMacro(macro);
            }
        }
    }

    // To convert PK macros into differential equations. Should probably be put in separate file on PharmML-level. Time pressure hackish for now
    void StructuralModel::convertMacrosIntoDEs(PharmML *model) {
        if (this->pk_macros) {
            for (PKMacro *compartment : this->pk_macros->getCompartments()) {
                int cmtNum = compartment->getCmtNum();

                std::unique_ptr<AstNode> elimination_equation;
                PKMacro *elimination = this->pk_macros->getElimination(cmtNum);
                if (elimination) {
                    std::shared_ptr<AstNode> volume;
                    std::shared_ptr<AstNode> clearance;
                    for (MacroValue value : elimination->getValues()) {
                        if (value.first == "V") {
                            volume = value.second;
                        } else if (value.first == "CL") {
                            clearance = value.second;
                        }
                    }
                    // Case 2: linear elimination with CL
                    if (volume && clearance) {
                        std::unique_ptr<AstNode> div = std::make_unique<BinopDivide>(clearance->clone(), volume->clone());  // FIXME: Really clone? but we have shared_ptrs..
                        std::unique_ptr<AstNode> amount;
                        for (MacroValue value : compartment->getValues()) {
                            if (value.first == "amount") {
                                amount = value.second->clone();
                            }
                        }
                        std::unique_ptr<AstNode> mul = std::make_unique<BinopTimes>(std::move(div), std::move(amount)); 
                        elimination_equation = std::move(mul);
                    }
                }

                std::unique_ptr<AstNode> depot_equation;
                std::unique_ptr<AstNode> absorption_equation;
                std::shared_ptr<AstNode> tlag;
                PKMacro *absorption = this->pk_macros->getAbsorption(cmtNum);
                if (absorption) {
                    std::shared_ptr<AstNode> ka;
                    for (MacroValue value : absorption->getValues()) {
                        if (value.first == "ka") {
                            ka = value.second;
                        } else if (value.first == "Tlag") {
                            tlag = value.second;
                        }
                    }

                    std::unique_ptr<AstNode> symbref = std::make_unique<SymbRef>(absorption->getName());
                    std::unique_ptr<AstNode> ka_mul = std::make_unique<BinopTimes>(ka->clone(), symbref->clone()); 
                    if (elimination_equation) {
                        std::unique_ptr<AstNode> minus = std::make_unique<BinopMinus>(std::move(ka_mul), std::move(elimination_equation));
                        absorption_equation = std::move(minus);
                    } else {
                        absorption_equation = std::move(ka_mul);
                    }

                    //DEPOT
                    DerivativeVariable *depot = new DerivativeVariable();
                    depot->setSymbId(absorption->getName());
                    std::shared_ptr<AstNode> iv_ref = std::make_shared<SymbRef>(model->getIndependentVariable()->getSymbId());
                    depot->setIndependentVariable(iv_ref);
                    std::shared_ptr<AstNode> init_time = std::make_shared<ScalarInt>(0);
                    depot->setInitialTime(init_time);
                    if (tlag) {
                        depot->setInitialValue(tlag);
                    } else {
                        std::shared_ptr<AstNode> init_value = std::make_shared<ScalarInt>(0);
                        depot->setInitialValue(init_value);
                    }

                    std::unique_ptr<AstNode> times = std::make_unique<BinopTimes>(ka->clone(), symbref->clone());
                    std::unique_ptr<AstNode> minus = std::make_unique<UniopMinus>(std::move(times));
                    depot->setAssignment(std::move(minus));
                    this->variables.push_back(depot);
                }

                // CENTRAL
                DerivativeVariable *deriv = new DerivativeVariable();
                deriv->setSymbId(compartment->getName());
                std::shared_ptr<AstNode> iv_ref = std::make_shared<SymbRef>(model->getIndependentVariable()->getSymbId());
                deriv->setIndependentVariable(iv_ref);
                std::shared_ptr<AstNode> init_time = std::make_shared<ScalarInt>(0);
                deriv->setInitialTime(init_time);
                std::shared_ptr<AstNode> init_value = std::make_shared<ScalarInt>(0);
                deriv->setInitialValue(init_value);

                std::shared_ptr<AstNode> assignment = std::move(absorption_equation);
                deriv->setAssignment(assignment);

                this->variables.push_back(deriv);
            }
            model->setupSymbols();

            this->pk_macros = nullptr;      // FIXME smartpointers to help!
        }
    }

}
