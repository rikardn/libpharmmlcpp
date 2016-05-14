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
#include <symbols/Covariate.h>
#include <PharmML/PharmMLContext.h>
#include <AST/AstNodeFactory.h>
#include <AST/AstNode.h>

namespace PharmML
{
    Covariate::Covariate(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void Covariate::parse(xml::Node node) {
        xml::Node n = this->context->getSingleElement(node, ".//mdef:TransformedCovariate");
        if (n.exists()) {
            this->transformedName = n.getAttribute("symbId").getValue();
        }
        xml::Node assign = this->context->getSingleElement(node, ".//ct:Assign");
        xml::Node tree = assign.getChild();
        this->assignment = this->context->factory.create(tree);
    } 

    std::string Covariate::getTransformedName() {
        return this->transformedName;
    }

    AstNode *Covariate::getAssignment() {
        return this->assignment;
    }
    
    void Covariate::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        std::unordered_set<Symbol *> found_symbols = this->symbRefsFromAst(this->assignment, symbolMap);
        this->addReferences(found_symbols);
    }
    
    void Covariate::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
    
    void Covariate::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }
}