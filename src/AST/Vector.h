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

#ifndef PHARMMLCPP_VECTOR_H_
#define PHARMMLCPP_VECTOR_H_

#include <AST/AstNode.h>
#include <AST/Scalar.h>
#include <xml/xml.h>

namespace pharmmlcpp
{
    class VectorCell
    {
        public:
            VectorCell(int index, std::unique_ptr<AstNode> content);
            int getIndex();
            AstNode* getContent();
        
        private:
            int index;
            std::unique_ptr<AstNode> content;
    };

    class Vector : public AstNode
    {
        public:
            Vector(std::string length, std::string defaultValue);
            Vector(PharmMLReader &reader, xml::Node node);
            void addElement(std::shared_ptr<AstNode> element);
            void populateCell(VectorCell *cell);
            std::vector<std::shared_ptr<AstNode>> getElements();
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;

        private:
            int length; // To infer length when implicitly built
            std::unique_ptr<ScalarReal> defaultContent; // To infer value of implicit elements
            std::vector<std::shared_ptr<AstNode>> elements;
    };
}

#endif
