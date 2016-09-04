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

namespace pharmmlcpp
{
    void DerivativeVariable::parse(PharmMLReader &reader, xml::Node node) {
        xml::Node iv = reader.getSingleElement(node, ".//ct:IndependentVariable");
        if (iv.exists()) {
            this->independent_variable = reader.factory.create(reader, iv.getChild());
        }
        xml::Node ival = reader.getSingleElement(node, ".//ct:InitialCondition/ct:InitialValue");
        if (ival.exists()) {
            this->initial_value = reader.factory.create(reader, ival.getChild().getChild());
        }
        xml::Node itime = reader.getSingleElement(node, ".//ct:InitialCondition/ct:InitialTime");
        if (itime.exists()) {
            this->initial_time = reader.factory.create(reader, itime.getChild().getChild());
        }
    }

    std::shared_ptr<AstNode> DerivativeVariable::getIndependentVariable() {
        return this->independent_variable;
    }

    std::shared_ptr<AstNode> DerivativeVariable::getInitialValue() {
        return this->initial_value;
    }

    std::shared_ptr<AstNode> DerivativeVariable::getInitialTime() {
        return this->initial_time;
    }


    void DerivativeVariable::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        if (this->assignment) {
            this->setupAstSymbRefs(this->assignment.get(), gathering, blkId);
        }
        if (this->independent_variable) {
            this->setupAstSymbRefs(this->independent_variable.get(), gathering, blkId);
        }
        if (this->initial_value) {
            this->setupAstSymbRefs(this->initial_value.get(), gathering, blkId);
        }
        if (this->initial_time) {
            this->setupAstSymbRefs(this->initial_time.get(), gathering, blkId);
        }
    }

    void DerivativeVariable::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void DerivativeVariable::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }
}
