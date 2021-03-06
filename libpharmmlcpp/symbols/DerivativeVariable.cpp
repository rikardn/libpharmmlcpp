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
            this->independent_variable = AstNode::create(reader, iv.getChild());
        }
        xml::Node ival = reader.getSingleElement(node, ".//ct:InitialCondition/ct:InitialValue");
        if (ival.exists()) {
            this->initial_value = AstNode::create(reader, ival.getChild().getChild());
        }
        xml::Node itime = reader.getSingleElement(node, ".//ct:InitialCondition/ct:InitialTime");
        if (itime.exists()) {
            this->initial_time = AstNode::create(reader, itime.getChild().getChild());
        }
    }

    xml::Node DerivativeVariable::xml(PharmMLWriter &writer) {
        xml::Node var("ct:DerivativeVariable");
        this->Symbol::xml(writer, var);
        var.setAttribute("symbolType", this->type);

        xml::Node assign("ct:Assign");
        var.addChild(assign);
        assign.addChild(this->assignment->xml(writer));

        xml::Node iv("ct:IndependentVariable");
        var.addChild(iv);
        iv.addChild(this->independent_variable->xml(writer));

        xml::Node icond("ct:InitialCondition");
        var.addChild(icond);
        xml::Node ival("ct:InitialValue");
        icond.addChild(ival);
        xml::Node ival_assign("ct:Assign");
        ival.addChild(ival_assign);
        ival_assign.addChild(this->initial_value->xml(writer));
        xml::Node itime("ct:InitialTime");
        icond.addChild(itime);
        xml::Node itime_assign("ct:Assign");       
        itime.addChild(itime_assign);
        itime_assign.addChild(this->initial_time->xml(writer));

        return var;
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


    void DerivativeVariable::setIndependentVariable(std::shared_ptr<AstNode> iv) {
        this->independent_variable = iv;
    }

    void DerivativeVariable::setInitialTime(std::shared_ptr<AstNode> time) {
        this->initial_time = time;   
    }

    void DerivativeVariable::setInitialValue(std::shared_ptr<AstNode> value) {
        this->initial_value = value;
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
