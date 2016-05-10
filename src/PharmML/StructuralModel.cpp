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
    }

    std::vector<PharmML::CommonVariable *> StructuralModel::getVariables() {
        return this->variables;
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
}
