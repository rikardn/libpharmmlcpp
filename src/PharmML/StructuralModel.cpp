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

namespace PharmML
{
    StructuralModel::StructuralModel(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void StructuralModel::parse(xml::Node node) {
        this->blkId = node.getAttribute("blkId").getValue();
        std::vector<xml::Node> array = this->context->getElements(node, ".//ct:Variable");
        for (xml::Node n : array) {
            PharmML::Variable *var = new PharmML::Variable(this->context, n);
            this->variables.push_back(var);
        }
        std::vector<xml::Node> derivs = this->context->getElements(node, ".//ct:DerivativeVariable");
        for (xml::Node n : derivs) {
            PharmML::DerivativeVariable *var = new PharmML::DerivativeVariable(this->context, n);
            this->variables.push_back(var);
        }
        // Construct PKMacros object if macros are available
        xml::Node macros_node = this->context->getSingleElement(node, "./mdef:PKmacros");
        if (macros_node.exists()) {
            PharmML::PKMacros *macros = new PharmML::PKMacros(this->context, macros_node);
            this->pk_macros = macros;
        }
    }

    std::vector<PharmML::CommonVariable *> StructuralModel::getVariables() {
        return this->variables;
    }

    bool StructuralModel::hasDerivatives() {
        return this->getDerivatives().size() > 0;
    }

    // Return all derivative variables
    std::vector<PharmML::CommonVariable *> StructuralModel::getDerivatives() {
        std::vector<PharmML::CommonVariable *> derivs;
        for (CommonVariable *var : this->variables) {
            if (var->isDerivative()) {
                derivs.push_back(var);
            }
        }

        return derivs;
    }

    PharmML::PKMacros *StructuralModel::getPKMacros() {
        return this->pk_macros;
    }

    void StructuralModel::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        if (this->pk_macros) {
            this->pk_macros->gatherSymbRefs(symbolMap);
        }
    }

    // Get all needed prerequisites of a list of variables
    std::vector<PharmML::CommonVariable *> StructuralModel::getPrerequisiteVariables(std::vector<PharmML::CommonVariable *> list) {
        // Map of all variables: symbId=>CommonVariable*
        std::unordered_map<std::string, CommonVariable *> variables_map;
        for (CommonVariable *var : this->variables) {
            variables_map[var->getSymbId()] = var;
        }

        // Set of all added symbIds
        std::unordered_set<std::string> added_symbols;
        for (CommonVariable *var : list) {
            added_symbols.insert(var->getSymbId());
        }

        // Loop through list and add all prerequisites
        bool added = true;
        while (added) {
            added = false;
            std::vector<CommonVariable *> added_vars;
            for (CommonVariable *var : list) {
                Dependencies& deps = var->getDependencies();
                std::unordered_set<std::string> depset = deps.getDependencySet();
                for (std::string symbol : depset) {
                    // If is a variable and not previously added
                    if (variables_map.count(symbol) == 1 && added_symbols.count(symbol) == 0) {
                        added = true;
                        added_symbols.insert(symbol);
                        added_vars.push_back(variables_map[symbol]);
                    }
                }
            }
            list.insert(list.end(), added_vars.begin(), added_vars.end());
        }

        return list;
    }

    // Get all variables that depends on at least one in variable in list and that is not in the list
    // Also get the dependencies of these
    // Warning! Don't use this method!
    std::vector<PharmML::CommonVariable *> StructuralModel::DependsOn(std::vector<PharmML::CommonVariable *> list) {
        std::vector<PharmML::CommonVariable *> found;

        for (CommonVariable *listed_var : list) {
            for (CommonVariable *var : this->variables) {
                for (std::string symbol : var->getDependencies().getDependencySet()) {
                    // if not in list and not already found and with the right name
                    if (std::find(list.begin(), list.end(), var) == list.end() &&
                            std::find(found.begin(), found.end(), var) == found.end() &&
                            symbol == listed_var->getSymbId()) {
                        found.push_back(var);
                    }
                }
            }
        }

        return this->getPrerequisiteVariables(found);
    }
}
