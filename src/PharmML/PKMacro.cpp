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

#include "PKMacro.h"

namespace PharmML
{
    PKMacro::PKMacro(PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
    }

    void PKMacro::parse(xml::Node node) {
        // Get type and all values of macro
        this->name = node.getName();
        std::vector<xml::Node> val_nodes = this->context->getElements(node, "./mdef:Value");
        for (xml::Node val_node : val_nodes) {
            // Get (optional) argument name
            MacroValue value;
            value.first = val_node.getAttribute("argument").getValue();

            // Get Assign, Symbref or Scalar
            xml::Node assign_node = this->context->getSingleElement(val_node, "./ct:Assign");
            if (assign_node.exists()) {
                xml::Node tree_node = assign_node.getChild();
                value.second = this->context->factory.create(tree_node);
            } else if (val_node.getChild().exists()) {
                // SymbRef or Scalar
                value.second = this->context->factory.create(val_node.getChild());
            } else {
                // TODO: Shouldn't this be schema illegal? Doesn't seem to stop me from crashing the code...
                value.second = nullptr;
            }

            this->values.push_back(value);
        }
    }

    std::string PKMacro::getName() {
        return this->name;
    }

    // Check if macro has a certain attribute
    bool PKMacro::hasAttribute(std::string attribute) {
        for (MacroValue value : this->values) {
            if (value.first == attribute) {
                return true;
            }
        }
        return false;
    }

    // Get pairs of all attributes and values
    std::vector<MacroValue> PKMacro::getValues() {
        return this->values;
    }

    // Find and get a specific attribute's assignment
    AstNode *PKMacro::getAssignment(std::string attribute) {
        for (MacroValue value : this->values) {
            if (value.first == attribute) {
                return value.second;
            }
        }
        return nullptr;
    }

    void PKMacro::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        for (MacroValue value : this->values) {
            if (value.second) { // TODO: See above comment
                std::unordered_set<Symbol *> symbols = this->symbRefsFromAst(value.second, symbolMap);
                this->referencedSymbols.addSymbols(symbols);
            }
        }
    }

    void PKMacro::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    PKMacros::PKMacros(PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
    }

    void PKMacros::parse(xml::Node node) {
        // Get all PK macros (nice that they are homologous, right?)
        std::vector<xml::Node> abs = this->context->getElements(node, "./mdef:Absorption");
        std::vector<xml::Node> cmt = this->context->getElements(node, "./mdef:Compartment");
        std::vector<xml::Node> dpt = this->context->getElements(node, "./mdef:Depot");
        std::vector<xml::Node> eff = this->context->getElements(node, "./mdef:Effect");
        std::vector<xml::Node> el = this->context->getElements(node, "./mdef:Elimination");
        std::vector<xml::Node> iv = this->context->getElements(node, "./mdef:IV");
        std::vector<xml::Node> orl = this->context->getElements(node, "./mdef:Oral");
        std::vector<xml::Node> per = this->context->getElements(node, "./mdef:Peripheral");
        std::vector<xml::Node> tra = this->context->getElements(node, "./mdef:Transfer");

        // Pre-allocate for premature optimization
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

        // Construct one PKMacro object for each macro
        for (xml::Node macro_node : macro_nodes) {
            PharmML::PKMacro *macro = new PharmML::PKMacro(this->context, macro_node);
            this->macros.push_back(macro);
        }
    }

    std::vector<PKMacro *> PKMacros::getAllMacros() {
        return this->macros;
    }

    void PKMacros::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        for (PKMacro *macro : this->macros) {
            macro->gatherSymbRefs(symbolMap);
        }
    }
}
