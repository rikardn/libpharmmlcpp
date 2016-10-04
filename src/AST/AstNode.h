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

#ifndef PHARMMLCPP_ASTNODE_H_
#define PHARMMLCPP_ASTNODE_H_

#include <memory>
#include <string>
#include <xml/xml.h>
#include <visitors/AstNodeVisitor.h>
#include <PharmML/PharmMLSection.h>
#include <PharmML/PharmMLWriter.h>

namespace pharmmlcpp
{
    class PharmMLReader;

    class AstNode : public PharmMLSection
    {
        public:
            virtual void accept(AstNodeVisitor *visitor) = 0;
            bool hasParentheses() { return this->parenthesized; };
            void elideParentheses() { this->parenthesized = false; };
            void parenthesize() { this->parenthesized = true; };
            virtual std::unique_ptr<AstNode> clone() = 0;
            virtual xml::Node xml(PharmMLWriter &writer) = 0;
            static std::unique_ptr<AstNode> create(PharmMLReader &reader, xml::Node node);

        private:
            bool parenthesized = true;
    };
}

#endif
