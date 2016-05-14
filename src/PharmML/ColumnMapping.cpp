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

#include "ColumnMapping.h"

namespace PharmML
{
    ColumnMapping::ColumnMapping(PharmML::PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void ColumnMapping::parse(xml::Node node) {
        xml::Node ref_node = this->context->getSingleElement(node, "./ds:ColumnRef");
        this->columnIdRef = ref_node.getAttribute("columnIdRef").getValue();
        xml::Node assign_node = this->context->getSingleElement(node, "./ct:Assign");
        xml::Node symbref_node = this->context->getSingleElement(node, "./ct:SymbRef");
        xml::Node piecewise_node= this->context->getSingleElement(node, "./ds:Piecewise");
        
        // Store mapping expression (should only contain one symbol reference)
        if (symbref_node.exists()) {
            this->symbRef = new SymbRef(symbref_node);
        } else if (assign_node.exists()) {
            this->assignment = this->context->factory.create(assign_node);
        } else if (piecewise_node.exists()) {
            this->assignment = this->context->factory.create(piecewise_node);
        }
    }
    
    xml::Node ColumnMapping::xml() {
        xml::Node cm("ColumnMapping");
        xml::Node idref("ColumnRef", xml::Namespace::ds);
        idref.setAttribute("columnIdRef", this->columnIdRef);
        cm.addChild(idref);
        XMLAstVisitor xml;
        this->assignment->accept(&xml);
        cm.addChild(xml.getValue());
        return cm;
    }

    AstNode *ColumnMapping::getAssignment() {
        return this->assignment;
    }

    std::string ColumnMapping::getColumnIdRef() {
        return this->columnIdRef;
    }
    
    Symbol *ColumnMapping::getMappedSymbol() {
        return this->mappedSymbol;
    }
    
    void ColumnMapping::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        if (this->symbRef) {
            this->mappedSymbol = this->addSymbRef(this->symbRef, symbolMap);
        } else {
            std::unordered_set<Symbol *> symbols = this->symbRefsFromAst(this->assignment, symbolMap);
            this->mappedSymbol = *(symbols.begin()); // There shall only be one
        }
    }
    
    void ColumnMapping::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
