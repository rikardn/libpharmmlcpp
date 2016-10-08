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

namespace pharmmlcpp
{
    CommonVariable::CommonVariable(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->CommonVariable::parse(reader, node);
    }

    void CommonVariable::parse(PharmMLReader &reader, xml::Node node) {
        this->Symbol::parse(node);
        xml::Node assign = reader.getSingleElement(node, ".//ct:Assign");
        if (assign.exists()) {
            xml::Node tree = assign.getChild();
            this->assignment = AstNode::create(reader, tree);
        }
    }

    std::shared_ptr<AstNode> CommonVariable::getAssignment() {
        return this->assignment;
    }

    bool CommonVariable::isDerivative() {
        return this->is_derivative;
    }


    xml::Node Variable::xml(PharmMLWriter &writer) {
        xml::Node var("ct:Variable");
        this->Symbol::xml(writer, var);

        xml::Node assign("ct:Assign");
        var.addChild(assign);
        assign.addChild(this->assignment->xml(writer));

        return var;
    }

    void Variable::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
       if (this->assignment) {
            this->setupAstSymbRefs(this->assignment.get(), gathering, blkId);
       }
    }

    void Variable::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void Variable::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }


    xml::Node DesignParameter::xml(PharmMLWriter &writer) {
        xml::Node var("ct:DesignParameter");

        return var;
    }
}
