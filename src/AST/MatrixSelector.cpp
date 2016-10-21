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

#include "MatrixSelector.h"

namespace pharmmlcpp
{
    /**
     *  Creates a new MatrixSelector from the xml PharmML code
     */
    MatrixSelector::MatrixSelector(PharmMLReader &reader, xml::Node node) {
        xml::Node row = reader.getSingleElement(node, "./ct:Row");
        if (row.exists()) {
            this->row = std::make_unique<MatrixVectorIndex>(reader, row);
        }
        xml::Node column = reader.getSingleElement(node, "./ct:Column");
        if (column.exists()) {
            this->column = std::make_unique<MatrixVectorIndex>(reader, column);
        }
        xml::Node cell_row = reader.getSingleElement(node, "./ct:Cell/ct:RowIndex");
        if (cell_row.exists()) {
            this->cellRow = std::make_unique<MatrixVectorIndex>(reader, cell_row); 
        }
        xml::Node cell_column = reader.getSingleElement(node, "./ct:Cell/ct:ColumnIndex");
        if (cell_column.exists()) {
            this->cellColumn = std::make_unique<MatrixVectorIndex>(reader, cell_column); 
        }
        xml::Node block_start_row = reader.getSingleElement(node, "./ct:Block/ct:BlockStartRow");
        if (block_start_row.exists()) {
            this->blockStartRow = std::make_unique<MatrixVectorIndex>(reader, block_start_row); 
        }
        xml::Node block_start_column = reader.getSingleElement(node, "./ct:Block/ct:BlockStartColumn");
        if (block_start_column.exists()) {
            this->blockStartColumn = std::make_unique<MatrixVectorIndex>(reader, block_start_column); 
        }
        xml::Node block_rows_number = reader.getSingleElement(node, "./ct:Block/ct:RowsNumber");
        if (block_rows_number.exists()) {
            this->rowsNumber = std::make_unique<MatrixVectorIndex>(reader, block_rows_number); 
        }
        xml::Node block_columns_number = reader.getSingleElement(node, "./ct:Block/ct:ColumnsNumber");
        if (block_columns_number.exists()) {
            this->columnsNumber = std::make_unique<MatrixVectorIndex>(reader, block_columns_number); 
        }
    }

    xml::Node MatrixSelector::xml(PharmMLWriter &writer) {
        xml::Node ms("MatrixSelector", xml::Namespace::ct);
        ms.addChild(this->matrix->xml(writer));
        if (this->cellRow) {
            xml::Node cell("Cell", xml::Namespace::ct);
            ms.addChild(cell);
            cell.addChild(this->cellRow->xml(writer, "RowIndex"));
            cell.addChild(this->cellColumn->xml(writer, "ColumnIndex"));
        } else if (this->blockStartRow) {

        } else if (this->row) {

        } else {

        }
        return ms;
    }


    /**
     *  Creates a new MatrixSelector selecting the full matrix
     */
    MatrixSelector::MatrixSelector(std::unique_ptr<SymbRef> matrix) {
        if (!matrix) {
            throw std::invalid_argument("nullptr");
        }
        this->matrix = std::move(matrix);
    }

    /**
     *  Copy constructor
     */
    MatrixSelector::MatrixSelector(const MatrixSelector& from) {
        this->matrix = from.matrix->clone();
    }

    /**
     *  Get the left endpoint of the interval
     */
    AstNode *MatrixSelector::getLeftEndpoint() {
        return this->leftEndpoint.get();
    }

    /**
     *  Get the right endpoint of the interval
     */
    AstNode *MatrixSelector::getRightEndpoint() {
        return this->rightEndpoint.get();
    }

    /**
     *  Set the left endpoint of the interval
     */
    void MatrixSelector::setLeftEndpoint(std::unique_ptr<AstNode> node) {
        if (!node) {
            throw std::invalid_argument("nullptr");
        }
        this->leftEndpoint = std::move(node);
    }

    /**
     *  Set the right endpoint of the interval
     */
    void MatrixSelector::setRightEndpoint(std::unique_ptr<AstNode> node) {
        if (!node) {
            throw std::invalid_argument("nullptr");
        }
        this->rightEndpoint = std::move(node);
    }

    /**
     *  Check if the left endpoint is open or closed.
     *  True means open and false means closed
     */
    bool MatrixSelector::isLeftEndpointOpenClosed() {
        return this->openLeftEndpoint;
    }

    /**
     *  Check if the right endpoint is open or closed.
     *  True means open and false means closed
     */
    bool MatrixSelector::isRightEndpointOpenClosed() {
        return this->openRightEndpoint;
    }

    /**
     *  Set the left endpoint open or closed.
     *  True means open and false means closed
     */
    void MatrixSelector::setLeftEndpointOpenClosed(bool open) {
        this->openLeftEndpoint = open;
    }

    /**
     *  Set the right endpoint open or closed.
     *  True means open and false means closed
     */
    void MatrixSelector::setRightEndpointOpenClosed(bool open) {
        this->openRightEndpoint = open;
    }
    
    /**
     *  Make a clone (deep copy) of this interval.
     */
    std::unique_ptr<AstNode> MatrixSelector::clone() {
        std::unique_ptr<Interval> cl = std::make_unique<Interval>(this->leftEndpoint->clone(), this->rightEndpoint->clone());
        cl->openLeftEndpoint = this->openLeftEndpoint;
        cl->openRightEndpoint = this->openRightEndpoint;
        return std::move(cl);
    }

    void MatrixSelector::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }
}
