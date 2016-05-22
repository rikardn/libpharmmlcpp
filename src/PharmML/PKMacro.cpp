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
            } else {
                // SymbRef or Scalar
                value.second = this->context->factory.create(val_node.getChild());
            }
            
            this->values.push_back(value);
        }
    }
    
    std::string PKMacro::getName() {
        return this->name;
    }
    
    std::vector<MacroValue> PKMacro::getValues() {
        return this->values;
    }
    
    void PKMacro::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        for (MacroValue value : this->values) {
            std::unordered_set<Symbol *> symbols = this->symbRefsFromAst(value.second, symbolMap);
            this->referencedSymbols.addSymbols(symbols);
        }
    }
    
    void PKMacro::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
