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

#include "Variable.h"
#include <iostream>

namespace PharmML
{
    CommonVariable::CommonVariable(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void CommonVariable::parse(xml::Node node) {
        this->symbId = node.getAttribute("symbId").getValue();
        xml::Node assign = this->context->getSingleElement(node, ".//ct:Assign");
        if (assign.exists()) {
            xml::Node tree = assign.getChild();
            this->assignment = this->context->factory.create(tree, &this->deps);
        }
    }
        
    AstNode *CommonVariable::getAssignment() {
        return this->assignment;
    }

    std::string CommonVariable::getSymbId() {
        return this->symbId;
    }

    Dependencies& CommonVariable::getDependencies() {
        return this->deps;
    }

    void Variable::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
