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
        xml::Node column_ref = node.getChild();
        this->columnIdRef = column_ref.getAttribute("columnIdRef").getValue();
        xml::Node assign_ref = node.getLastChild();     // Currently can support one symbref or one piecewise
        this->assignment = this->context->factory.create(assign_ref);
    }

    AstNode *ColumnMapping::getAssignment() {
        return this->assignment;
    }

    std::string ColumnMapping::getColumnIdRef() {
        return this->columnIdRef;
    }
    
    void ColumnMapping::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
