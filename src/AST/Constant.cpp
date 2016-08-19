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

#include "Constant.h"

namespace pharmmlcpp
{
    // Null value (example usage: missing otherwise piece)
    void NullValue::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    std::unique_ptr<AstNode> NullValue::clone() {
        std::unique_ptr<NullValue> cl;
        return std::move(cl);
    }

    xml::Node NullValue::xml(PharmMLWriter &writer) {
        return xml::Node("Null", xml::Namespace::ct);
    }

    // Pi and Euler's constant
    void Pi::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    std::unique_ptr<AstNode> Pi::clone() {
        std::unique_ptr<Pi> cl;
        return std::move(cl);
    }

    xml::Node Pi::xml(PharmMLWriter &writer) {
        xml::Node constant("Constant", xml::Namespace::math);
        constant.setAttribute("op", "pi");
        return constant;
    }

    void Exponentiale::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    std::unique_ptr<AstNode> Exponentiale::clone() {
        std::unique_ptr<Exponentiale> cl;
        return std::move(cl);
    }

    xml::Node Exponentiale::xml(PharmMLWriter &writer) {
        xml::Node constant("Constant", xml::Namespace::math);
        constant.setAttribute("op", "exponentiale");
        return constant;
    }
}
