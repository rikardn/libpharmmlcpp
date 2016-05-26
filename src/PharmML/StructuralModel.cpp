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
        // Get all PK macros (nice that they are homologous, right?)
        xml::Node macros_node = this->context->getSingleElement(node, "./mdef:PKmacros");
        if (macros_node.exists()) {
            std::vector<xml::Node> abs = this->context->getElements(macros_node, "./mdef:Absorption");
            std::vector<xml::Node> cmt = this->context->getElements(macros_node, "./mdef:Compartment");
            std::vector<xml::Node> dpt = this->context->getElements(macros_node, "./mdef:Depot");
            std::vector<xml::Node> eff = this->context->getElements(macros_node, "./mdef:Effect");
            std::vector<xml::Node> el = this->context->getElements(macros_node, "./mdef:Elimination");
            std::vector<xml::Node> iv = this->context->getElements(macros_node, "./mdef:IV");
            std::vector<xml::Node> orl = this->context->getElements(macros_node, "./mdef:Oral");
            std::vector<xml::Node> per = this->context->getElements(macros_node, "./mdef:Peripheral");
            std::vector<xml::Node> tra = this->context->getElements(macros_node, "./mdef:Transfer");
            std::vector<xml::Node> macro_nodes;
            macro_nodes.reserve(abs.size() + cmt.size() + dpt.size() + eff.size() + el.size() + iv.size() + orl.size() + per.size() + tra.size());
            macro_nodes.insert(macro_nodes.end(), abs.begin(), abs.end());
            macro_nodes.insert(macro_nodes.end(), cmt.begin(), cmt.end());
            macro_nodes.insert(macro_nodes.end(), dpt.begin(), dpt.end());
            macro_nodes.insert(macro_nodes.end(), eff.begin(), eff.end());
            macro_nodes.insert(macro_nodes.end(), el.begin(), el.end());
            macro_nodes.insert(macro_nodes.end(), iv.begin(), iv.end());
            macro_nodes.insert(macro_nodes.end(), orl.begin(), orl.end());
            macro_nodes.insert(macro_nodes.end(), per.begin(), per.end());
            macro_nodes.insert(macro_nodes.end(), tra.begin(), tra.end());
            for (xml::Node macro_node : macro_nodes) {
                this->pk_macros_section = new PharmMLSection(macros_node);
                PharmML::PKMacro *macro = new PharmML::PKMacro(this->context, macro_node);
                this->pk_macros.push_back(macro);
            }
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

    std::vector<PharmML::PKMacro *> StructuralModel::getPKMacros() {
        return this->pk_macros;
    }

    PharmMLSection *StructuralModel::getPKMacrosSection() {
        return this->pk_macros_section;
    }

    void StructuralModel::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        for (PKMacro *pk_macro : this->pk_macros) {
            pk_macro->gatherSymbRefs(symbolMap);
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
