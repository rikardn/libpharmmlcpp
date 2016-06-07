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

#include "DerivativeVariable.h"
#include <iostream>

namespace PharmML
{
    void DerivativeVariable::parse(xml::Node node) {
        xml::Node iv = this->context->getSingleElement(node, ".//ct:IndependentVariable");
        if (iv.exists()) {
            this->independent_variable = this->context->factory.create(iv.getChild());
        }
        xml::Node ival = this->context->getSingleElement(node, ".//ct:InitialCondition/ct:InitialValue");
        if (ival.exists()) {
            this->initial_value = this->context->factory.create(ival.getChild().getChild());
        }
        xml::Node itime = this->context->getSingleElement(node, ".//ct:InitialCondition/ct:InitialTime");
        if (itime.exists()) {
            this->initial_time = this->context->factory.create(itime.getChild().getChild());
        }
    }

    AstNode *DerivativeVariable::getIndependentVariable() {
        return this->independent_variable;
    }

    AstNode *DerivativeVariable::getInitialValue() {
        return this->initial_value;
    }

    AstNode *DerivativeVariable::getInitialTime() {
        return this->initial_time;
    }

    void DerivativeVariable::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        if (this->assignment) {
            this->symbRefsFromAst(this->assignment, symbolMap);
        }
        if (this->initial_value) {
            this->symbRefsFromAst(this->initial_value, symbolMap);
        }
        if (this->initial_time) {
            this->symbRefsFromAst(this->initial_time, symbolMap);
        }
    }

    void DerivativeVariable::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void DerivativeVariable::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }
}
