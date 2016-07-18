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

#ifndef PHARMMLCPP_SCALAR_H_
#define PHARMMLCPP_SCALAR_H_

#include <AST/AstNode.h>
#include <xml/xml.h>

namespace pharmmlcpp
{
    /**
     * Base class for all scalar constant AstNodes 
     */
    class Scalar : public AstNode
    {
    };

    /**
     * A scalar integer representing a signed 32-bit integer constant
     */
    class ScalarInt : public Scalar
    {
        public:
            ScalarInt(int value);
            ScalarInt(std::string s);
            ScalarInt(xml::Node xml_node);
            std::string toString();
            int toInt();
            void set(int value);
            void set(std::string value);
            virtual void accept(AstNodeVisitor *visitor);
        
        private:
            int value;
    };

    /**
     * A scalar real representing a double precision floating point constant
     */
    class ScalarReal : public Scalar
    {
        public:
            ScalarReal(double value);
            ScalarReal(std::string s);
            ScalarReal(xml::Node xml_node);
            std::string toString();
            double toDouble();
            void set(double value);
            void set(std::string value);
            virtual void accept(AstNodeVisitor *visitor);
        
        private:
            std::string value;
    };

    /**
     * A scalar boolean constant
     */
    class ScalarBool : public Scalar
    {
        public:
            ScalarBool(bool value);
            bool toBool();
            void set(bool value);
            virtual void accept(AstNodeVisitor *visitor);

        private:
            bool value;
    };

    /**
     *  A scalar string constant
     */
    class ScalarString : public Scalar
    {
        public:
            ScalarString(std::string value);
            ScalarString(xml::Node xml_node);
            std::string toString();
            void set(std::string value);
            virtual void accept(AstNodeVisitor *visitor);

        private:
            std::string value;
    };

    /**
     *  A scalar id constant
     *  Note that there is currently no check that the id matches xs:NCName
     */
    class ScalarId : public ScalarString
    {
        public:
            ScalarId(std::string value) : ScalarString(value) {}
            ScalarId(xml::Node xml_node) : ScalarString(xml_node) {}
    };
}

#endif
