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
#include <iostream>

namespace pharmmlcpp
{
    /**
     *  Creates a new ScalarInt from an integer value
     */
    ScalarInt::ScalarInt(int value) {
        this->value = value;
    }

    /**
     *  Creates a new ScalarInt from a string
     *  \throws std::invalid_argument if the argument string could not be converted to an int
     *  \throws std::out_of_range if the argument string represent a number smaller or larger than an int32
     */
    ScalarInt::ScalarInt(std::string s) {
        this->value = std::stoi(s);
    }

    /**
     *  Creates a new ScalarInt from the PharmML xml node
     */
    ScalarInt::ScalarInt(xml::Node xml_node) {
        this->value = std::stoi(xml_node.getText());
    }

    /**
     *  Convert to a string
     */
    std::string ScalarInt::toString() {
        return std::to_string(value);
    }

    /**
     *  Convert to an integer
     */
    int ScalarInt::toInt() {
        return value;
    }

    /**
     *  Set a new integer value
     */
    void ScalarInt::set(int value) {
        this->value = value;
    }

    /**
     *  Set a new value from a string
     */
    void ScalarInt::set(std::string value) {
        this->value = std::stoi(value);
    }

    void ScalarInt::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    /**
     *  Create a new ScalarReal from a double value
     */
    ScalarReal::ScalarReal(double value) {
        this->value = std::to_string(value);
    }

    /**
     *  Create a new ScalarReal from a string representing a double value
     */
    ScalarReal::ScalarReal(std::string value) {
        this->value = value;
    }

    /**
     *  Create a new ScalarReal from a PharmML xml node
     */
    ScalarReal::ScalarReal(xml::Node xml_node) {
        this->value = xml_node.getText();
    }

    /**
     *  Convert to a double value
     *  \throws std::invalid_argument
     *  \throws std::out_of_range
     */
    double ScalarReal::toDouble() {
        return std::stod(this->value);
    }

    /**
     *  Convert to a string representing a double value
     */
    std::string ScalarReal::toString() {
        return value;
    }

    /**
     *  Set a new value from a double value
     */
    void ScalarReal::set(double value) {
        this->value = std::to_string(value);
    }

    /**
     *  Set a new value from a string representing a double value
     */
    void ScalarReal::set(std::string value) {
        this->value = value;
    }

    void ScalarReal::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    /**
     *  Create a new ScalarBool from a boolean value
     */
    ScalarBool::ScalarBool(bool value) {
        this->value = value;
    }

    /**
     *  Convert to a bool value
     */
    bool ScalarBool::toBool() {
        return value;
    }
    
    /**
     *  Set a new value from a bool value
     */
    void ScalarBool::set(bool value) {
        this->value = value;
    }

    void ScalarBool::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    /**
     *  Create a new ScalarString from a string value
     */
    ScalarString::ScalarString(std::string value) {
        this->value = value;
    }

    /**
     *  Create a new ScalarString from the PharmML xml node
     */
    ScalarString::ScalarString(xml::Node xml_node) {
        this->value = xml_node.getText();
    }

    /**
     *  Convert to a string value
     */
    std::string ScalarString::toString() {
        return value;
    }

    /**
     *  Set a new value from a string value
     */
    void ScalarString::set(std::string value) {
        this->value = value;
    }

    void ScalarString::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }
}
