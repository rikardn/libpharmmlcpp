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
            xml::Node bs("Block", xml::Namespace::ct);
            ms.addChild(bs);
            bs.addChild(this->blockStartRow->xml(writer, "BlockStartRow"));
            bs.addChild(this->blockStartColumn->xml(writer, "BlockStartColumn"));
            bs.addChild(this->blockStartColumn->xml(writer, "RowsNumber"));
            bs.addChild(this->blockStartColumn->xml(writer, "ColumnsNumber"));
        } else if (this->row) {
            ms.addChild(this->row->xml(writer, "Row"));
        } else {
            ms.addChild(this->row->xml(writer, "Column"));
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
        this->matrix = std::make_unique<SymbRef>(*from.matrix.get());
        if (from.cellRow) {
            this->cellRow = from.cellRow->clone();
            this->cellColumn = from.cellColumn->clone();
        } else if (from.blockStartRow) {
            this->blockStartRow = from.blockStartRow->clone();
            this->blockStartColumn = from.blockStartColumn->clone();
            this->rowsNumber = from.rowsNumber->clone();
            this->columnsNumber = from.columnsNumber->clone();
        } else if (from.row) {
            this->row = from.row->clone();
        } else if (from.column) {
            this->column = from.column->clone();
        }


    }

    /**
     *  Does this MatrixSelector select a cell?
     */
    bool MatrixSelector::isCell() {
        if (this->cellRow) {
            return true;
        } else {
            return false;
        }
    }

    /**
     *  Does this MatrixSelector select a block?
     */
    bool MatrixSelector::isBlock() {
        if (this->blockStartRow) {
            return true;
        } else {
            return false;
        }
    }

    /**
     *  Does this MatrixSelector select a row?
     */
    bool MatrixSelector::isRow() {
        if (this->row) {
            return true;
        } else {
            return false;
        }
    }

    /**
     *  Does this MatrixSelector select a column?
     */
    bool MatrixSelector::isColumn() {
        if (this->column) {
            return true;
        } else {
            return false;
        }
    }

    /**
     *  Return the CellRowIndex if this MatrixSelector selects a cell and nullptr otherwise
     */
    AstNode *MatrixSelector::getCellRow() {
        return this->cellRow->getIndex();
    }

    /**
     *  Return the CellColumnIndex if this MatrixSelector selects a cell and nullptr otherwise
     */
    AstNode *MatrixSelector::getCellColumn() {
        return this->cellColumn->getIndex();
    }

    /**
     *  Return the BlockStartRow if this MatrixSelector selects a block and nullptr otherwise
     */
    AstNode *MatrixSelector::getBlockStartRow() {
        return this->blockStartRow->getIndex();
    }
    
    /**
     *  Return the BlockStartColumn if this MatrixSelector selects a block and nullptr otherwise
     */
    AstNode *MatrixSelector::getBlockStartColumn() {
        return this->blockStartColumn->getIndex();
    }

    /**
     *  Return the RowsNumber if this MatrixSelector selects a block and nullptr otherwise
     */
    AstNode *MatrixSelector::getRowsNumber() {
        return this->rowsNumber->getIndex();
    }
    
    /**
     *  Return the ColumnsNumber if this MatrixSelector selects a block and nullptr otherwise
     */
    AstNode *MatrixSelector::getColumnsNumber() {
        return this->columnsNumber->getIndex();
    }
 
    /**
     *  Return the row if this MatrixSelector selects a row and nullptr otherwise
     */
    AstNode *MatrixSelector::getRow() {
        return this->row->getIndex();
    }

    /**
     *  Return the column if this MatrixSelector selects a column and nullptr otherwise
     */
    AstNode *MatrixSelector::getColumn() {
        return this->column->getIndex();
    }

    void MatrixSelector::clearAll() {
        this->cellRow = nullptr;
        this->cellColumn = nullptr;
        this->blockStartRow = nullptr;
        this->blockStartColumn = nullptr;
        this->rowsNumber = nullptr;
        this->columnsNumber = nullptr;
        this->row = nullptr;
        this->column = nullptr;
    }

    /**
     *  Set the selector to select a cell of the matrix
     */
    void MatrixSelector::selectCell(std::unique_ptr<AstNode> cellRow, std::unique_ptr<AstNode> cellColumn) {
        if (!cellRow or !cellColumn) {
            throw std::invalid_argument("nullptr");
        }
        this->clearAll();
        this->cellRow = std::make_unique<MatrixVectorIndex>(std::move(cellRow));
        this->cellColumn = std::make_unique<MatrixVectorIndex>(std::move(cellColumn));
    }

    /**
     *  Set the selector to select a block of the matrix
     */
    void MatrixSelector::selectBlock(std::unique_ptr<AstNode> blockStartRow, std::unique_ptr<AstNode> blockStartColumn,
            std::unique_ptr<AstNode> rowsNumber, std::unique_ptr<AstNode> columnsNumber) {
        if (!blockStartRow or !blockStartColumn or !rowsNumber or !columnsNumber) {
            throw std::invalid_argument("nullptr");
        }
        this->clearAll();
        this->blockStartRow = std::make_unique<MatrixVectorIndex>(std::move(blockStartRow));
        this->blockStartColumn = std::make_unique<MatrixVectorIndex>(std::move(blockStartColumn));
        this->rowsNumber = std::make_unique<MatrixVectorIndex>(std::move(rowsNumber));
        this->columnsNumber = std::make_unique<MatrixVectorIndex>(std::move(columnsNumber));
    }

    /**
     *  Set the selector to select a row of the matrix
     */
    void MatrixSelector::selectRow(std::unique_ptr<AstNode> row) {
        if (!row) {
            throw std::invalid_argument("nullptr");
        }
        this->clearAll();
        this->row = std::make_unique<MatrixVectorIndex>(std::move(row));
    }

    /**
     *  Set the selector to select a column of the matrix
     */
    void MatrixSelector::selectColumn(std::unique_ptr<AstNode> column) {
        if (!column) {
            throw std::invalid_argument("nullptr");
        }
        this->clearAll();
        this->column = std::make_unique<MatrixVectorIndex>(std::move(column));
    }

    /**
     *  Make a clone (deep copy) of this MatrixSelector.
     */
    std::unique_ptr<AstNode> MatrixSelector::clone() {
        std::unique_ptr<MatrixSelector> cl = std::make_unique<MatrixSelector>(std::make_unique<SymbRef>(*this->matrix.get()));
        if (this->cellRow) {
            cl->cellRow = this->cellRow->clone();
            cl->cellColumn = this->cellColumn->clone();
        } else if (this->blockStartRow) {
            cl->blockStartRow = this->blockStartRow->clone();
            cl->blockStartColumn = this->blockStartColumn->clone();
            cl->rowsNumber = this->rowsNumber->clone();
            cl->columnsNumber = this->columnsNumber->clone();
        } else if (this->row) {
            cl->row = this->row->clone();
        } else if (this->column) {
            cl->column = this->column->clone();
        }
        return std::move(cl);
    }

    void MatrixSelector::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }
}
