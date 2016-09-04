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
    ScalarInt::ScalarInt(PharmMLReader &reader, xml::Node xml_node) {
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
     *  \throws std::invalid_argument
     *  \throws std::out_of_range
     */
    void ScalarInt::set(std::string value) {
        this->value = std::stoi(value);
    }

    std::unique_ptr<AstNode> ScalarInt::clone() {
        std::unique_ptr<ScalarInt> cl(new ScalarInt(*this));
        return std::move(cl);
    }

    void ScalarInt::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    xml::Node ScalarInt::xml(PharmMLWriter &writer) {
        xml::Node scalar("Int", xml::Namespace::ct);
        scalar.setContent(this->toString());
        return scalar;
    }

    /**
     *  Create a new ScalarReal from a double value
     */
    ScalarReal::ScalarReal(double value) {
        this->value = std::to_string(value);
    }

    /**
     *  Create a new ScalarReal from a string representing a double value
     *  \throws std::invalid_argument
     *  \throws std::out_of_range
     */
    ScalarReal::ScalarReal(std::string value) {
        std::stod(value);
        this->value = value;
    }

    /**
     *  Create a new ScalarReal from a PharmML xml node
     */
    ScalarReal::ScalarReal(PharmMLReader &reader, xml::Node xml_node) {
        this->value = xml_node.getText();
    }

    /**
     *  Convert to a double value
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
     *  \throws std::invalid_argument
     *  \throws std::out_of_range
     */
    void ScalarReal::set(std::string value) {
        std::stod(value);
        this->value = value;
    }

    std::unique_ptr<AstNode> ScalarReal::clone() {
        std::unique_ptr<ScalarReal> cl(new ScalarReal(*this));
        return std::move(cl);
    }

    void ScalarReal::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    xml::Node ScalarReal::xml(PharmMLWriter &writer) {
        xml::Node scalar("Real", xml::Namespace::ct);
        scalar.setContent(this->toString());
        return scalar;
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

    std::unique_ptr<AstNode> ScalarBool::clone() {
        std::unique_ptr<ScalarBool> cl(new ScalarBool(*this));
        return std::move(cl);
    }

    void ScalarBool::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    xml::Node ScalarBool::xml(PharmMLWriter &writer) {
        if (value) {
            return xml::Node("True", xml::Namespace::ct);
        } else {
            return xml::Node("False", xml::Namespace::ct);
        }
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
    ScalarString::ScalarString(PharmMLReader &reader, xml::Node xml_node) {
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

    std::unique_ptr<AstNode> ScalarString::clone() {
        std::unique_ptr<ScalarString> cl(new ScalarString(*this));
        return std::move(cl);
    }

    void ScalarString::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    xml::Node ScalarString::xml(PharmMLWriter &writer) {
        xml::Node scalar("String", xml::Namespace::ct);
        scalar.setContent(this->value);
        return scalar;
    }

    /**
     *  Create a new ScalarId from a string value
     *  \throws std::invalid_argument if value does not match NCName
     */
    ScalarId::ScalarId(std::string value) {
        if (!xml::validateNCName(value)) {
            throw std::invalid_argument("Argument does not match NCName");
        }
        this->value = value;
    }

    /**
     *  Create a new ScalarString from the PharmML xml node
     */
    ScalarId::ScalarId(PharmMLReader &reader, xml::Node xml_node) {
        this->value = xml_node.getText();
    }

    /**
     *  Convert to a string value
     */
    std::string ScalarId::toString() {
        return value;
    }

    /**
     *  Set a new value from a string value
     *  \throws std::invalid_argument if value does not match NCName
     */
    void ScalarId::set(std::string value) {
        if (!xml::validateNCName(value)) {
            throw std::invalid_argument("Argument does not match NCName");
        }
        this->value = value;
    }

    std::unique_ptr<AstNode> ScalarId::clone() {
        std::unique_ptr<ScalarId> cl(new ScalarId(*this));
        return std::move(cl);
    }

    void ScalarId::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    xml::Node ScalarId::xml(PharmMLWriter &writer) {
        xml::Node scalar("Id", xml::Namespace::ct);
        scalar.setContent(this->value);
        return scalar;
    }

    void ScalarNA::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    std::unique_ptr<AstNode> ScalarNA::clone() {
        std::unique_ptr<ScalarNA> cl(new ScalarNA(*this));
        return std::move(cl);
    }

    xml::Node ScalarNA::xml(PharmMLWriter &writer) {
        return xml::Node("NA", xml::Namespace::ct);
    }

    void ScalarNaN::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    std::unique_ptr<AstNode> ScalarNaN::clone() {
        std::unique_ptr<ScalarNaN> cl(new ScalarNaN(*this));
        return std::move(cl);
    }

    xml::Node ScalarNaN::xml(PharmMLWriter &writer) {
        return xml::Node("NaN", xml::Namespace::ct);
    }

    void ScalarPlusInf::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    std::unique_ptr<AstNode> ScalarPlusInf::clone() {
        std::unique_ptr<ScalarPlusInf> cl(new ScalarPlusInf(*this));
        return std::move(cl);
    }

    xml::Node ScalarPlusInf::xml(PharmMLWriter &writer) {
        return xml::Node("plusInf", xml::Namespace::ct);
    }

    void ScalarMinusInf::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    std::unique_ptr<AstNode> ScalarMinusInf::clone() {
        std::unique_ptr<ScalarMinusInf> cl(new ScalarMinusInf(*this));
        return std::move(cl);
    }

    xml::Node ScalarMinusInf::xml(PharmMLWriter &writer) {
        return xml::Node("minusInf", xml::Namespace::ct);
    }

    void ScalarBLQ::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    std::unique_ptr<AstNode> ScalarBLQ::clone() {
        std::unique_ptr<ScalarBLQ> cl(new ScalarBLQ(*this));
        return std::move(cl);
    }

    xml::Node ScalarBLQ::xml(PharmMLWriter &writer) {
        return xml::Node("BLQ", xml::Namespace::ct);
    }

    std::unique_ptr<AstNode> ScalarALQ::clone() {
        std::unique_ptr<ScalarALQ> cl(new ScalarALQ(*this));
        return std::move(cl);
    }

    void ScalarALQ::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    xml::Node ScalarALQ::xml(PharmMLWriter &writer) {
        return xml::Node("ALQ", xml::Namespace::ct);
    }
}
