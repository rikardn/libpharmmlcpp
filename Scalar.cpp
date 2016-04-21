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

#include "Scalar.h"

namespace PharmML
{
    ScalarInt::ScalarInt(int value) {
        this->value = value;
    }

    ScalarInt::ScalarInt(std::string s) {
        this->value = std::stoi(s);
    }

    ScalarInt::ScalarInt(xml::Node xml_node) {
        this->value = std::stoi(xml_node.getText());
    }

    std::string ScalarInt::toString() {
        return std::to_string(value); 
    } 

    void ScalarInt::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    ScalarReal::ScalarReal(double value) {
        this->value = value;
    }

    ScalarReal::ScalarReal(std::string s) {
        this->value = std::stod(s);
    }

    ScalarReal::ScalarReal(xml::Node xml_node) {
        this->value = std::stod(xml_node.getText());
    }

    std::string ScalarReal::toString() {
        return std::to_string(value); 
    } 

    void ScalarReal::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }
}
