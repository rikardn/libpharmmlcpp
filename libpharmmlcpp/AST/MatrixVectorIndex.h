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

#ifndef PHARMMLCPP_MATRIXVECTORINDEX_H_
#define PHARMMLCPP_MATRIXVECTORINDEX_H_

#include <AST/AstNode.h>
#include <xml/xml.h>
#include <PharmML/PharmMLReader.h>

namespace pharmmlcpp
{
    /**
     *  An index of a vector or a matrix
     */
    class MatrixVectorIndex
    {
        public:
            MatrixVectorIndex(PharmMLReader &reader, xml::Node node);
            MatrixVectorIndex(std::unique_ptr<AstNode> index);
            MatrixVectorIndex(const MatrixVectorIndex& from);
            AstNode *getIndex();
            void setIndex(std::unique_ptr<AstNode> index);
            std::unique_ptr<MatrixVectorIndex> clone();
            xml::Node xml(PharmMLWriter &writer, std::string name);

        private:
            std::unique_ptr<AstNode> index;
    };
}

#endif
