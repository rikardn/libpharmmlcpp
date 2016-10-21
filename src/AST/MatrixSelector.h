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

#ifndef PHARMMLCPP_MATRIXSELECTOR_H_
#define PHARMMLCPP_MATRIXSELECTOR_H_

#include <AST/AstNode.h>
#include <xml/xml.h>

namespace pharmmlcpp
{
    /**
     *  MatrixSelector can select a submatrix. It can be a single cell, a block, a row or a column of a matrix
     */
    class MatrixSelector : public AstNode
    {
        public:
            MatrixSelector(PharmMLReader &reader, xml::Node node);
            MatrixSelector(std::unique_ptr<AstNode> matrix);
            MatrixSelector(const MatrixSelector& from);
            AstNode *getLeftEndpoint();
            AstNode *getRightEndpoint();
            void setLeftEndpoint(std::unique_ptr<AstNode> node);
            void setRightEndpoint(std::unique_ptr<AstNode> node);
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;

        private:
            std::unique_ptr<SymbRef> matrix;

            // a single cell
            std::unique_ptr<MatrixVectorIndex> cellRow;
            std::unique_ptr<MatrixVectorIndex> cellColumn;

            // a block
            std::unique_ptr<MatrixVectorIndex> blockStartRow;
            std::unique_ptr<MatrixVectorIndex> blockStartColumn;
            std::unique_ptr<MatrixVectorIndex> rowsNumber;
            std::unique_ptr<MatrixVectorIndex> columnsNumber;

            // a row
            std::unique_ptr<MatrixVectorIndex> row;

            // a column
            std::unique_ptr<MatrixVectorIndex> column;
    };
}

#endif
