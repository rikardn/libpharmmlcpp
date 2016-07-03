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
}
