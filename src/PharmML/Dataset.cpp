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

#include "Dataset.h"

namespace pharmmlcpp
{
    // Class HeaderDefinition (single header specification of dataset)
    HeaderDefinition::HeaderDefinition(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void HeaderDefinition::parse(PharmMLReader &reader, xml::Node node) {
        // Get name, header type and row number
        this->name = node.getAttribute("name").getValue();
        // TODO: Support headerType (some symbolId stuff)
        //~ this->headerType = node.getAttribute("headerType").getValue();
        this->rowNumber = std::stoi(node.getAttribute("rowNumber").getValue());
    }

    xml::Node HeaderDefinition::xml() {
        xml::Node def("Header");
        def.setAttribute("name", this->name);
        def.setAttribute("rowNumber", std::to_string(this->rowNumber));
        return def;
    }

    void addHeaderRow(xml::Node node) {
        // TODO: Support this (from DataColumn parent class maybe)
    }

    std::string HeaderDefinition::getName() {
        return this->name;
    }

    int HeaderDefinition::getRowNumber() {
        return this->rowNumber;
    }

    // Class ColumnDefinition (single column specification of dataset)
    ColumnDefinition::ColumnDefinition(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void ColumnDefinition::parse(PharmMLReader &reader, xml::Node node) {
        // Get attributes of column
        this->id = node.getAttribute("columnId").getValue();
        this->type = node.getAttribute("columnType").getValue();
        this->level = node.getAttribute("level").getValue();
        this->valueType = node.getAttribute("valueType").getValue();
        this->num = std::stoi(node.getAttribute("columnNum").getValue());
    }

    xml::Node ColumnDefinition::xml() {
        xml::Node def("Column");
        def.setAttribute("columnId", this->id);
        if (this->type != "") {
            def.setAttribute("columnType", this->type);
        }
        if (this->level != "") {
            def.setAttribute("level", this->level);
        }
        def.setAttribute("valueType", this->valueType);
        def.setAttribute("columnNum", std::to_string(this->num));
        return def;
    }

    std::string ColumnDefinition::getId() {
        return this->id;
    }

    std::string ColumnDefinition::getType() {
        return this->type;
    }

    std::string ColumnDefinition::getLevel() {
        return this->level;
    }

    std::string ColumnDefinition::getValueType() {
        return this->valueType;
    }

    int ColumnDefinition::getNum() {
        return this->num;
    }

    // Class DatasetDefinition (header/column specifications of dataset)
    DatasetDefinition::DatasetDefinition(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    ColumnDefinition *DatasetDefinition::getColumnDefinition(int colNum) {
        return this->columns[colNum-1];
    }

    std::vector<ColumnDefinition *> DatasetDefinition::getColumnDefinitions() {
        return this->columns;
    }

    int DatasetDefinition::getNumColumns() {
        return this->columns.size();
    }

    void DatasetDefinition::parse(PharmMLReader &reader, xml::Node node) {
        // Get header definitions
        std::vector<xml::Node> headers = reader.getElements(node, "./ds:Header");
        for (xml::Node header_node : headers) {
            HeaderDefinition *header = new HeaderDefinition(reader, header_node);
            this->headers.push_back(header);
        }

        // Get column definitions
        std::vector<xml::Node> columns = reader.getElements(node, "./ds:Column");
        for (xml::Node column_node : columns) {
            ColumnDefinition *column = new ColumnDefinition(reader, column_node);
            this->columns.push_back(column);
        }

        // Get ignore condition and/or ignore symbols
        xml::Node ignore = reader.getSingleElement(node, "./ds:IgnoreLineType");
        if (ignore.exists()) {
            xml::Node condition = reader.getSingleElement(ignore, "./math:Condition");
            if (condition.exists()) {
                // TODO: Include deps below via moving creation of AstNode to Factory
                this->ignoreCondition = AstNodeFactory::create(reader, condition);
            }
            this->ignoreSymbols = ignore.getAttribute("symbol").getValue();
        }
    }

    xml::Node DatasetDefinition::xml() {
        xml::Node def("Definition");
        for (HeaderDefinition *header : this->headers) {
            def.addChild(header->xml());
        }
        for (ColumnDefinition *column : this->columns) {
            def.addChild(column->xml());
        }
        if (this->ignoreSymbols != "") {
            xml::Node ignore = def.createChild("IgnoreLineType");
            ignore.setAttribute("symbol", this->ignoreSymbols);
            if (this->ignoreCondition) {
                xml::Node cond = ignore.createChild("Condition");
                XMLAstVisitor xml;
                this->ignoreCondition->accept(&xml);
                cond.addChild(xml.getValue());
            }
        }
        return def;
    }

    // class ExternalFile (data is stored externally)
    ExternalFile::ExternalFile(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    std::string ExternalFile::getOid() {
        return this->oid;
    }

    std::string ExternalFile::getPath() {
        return this->path;
    }

    std::string ExternalFile::getFormat() {
        return this->format;
    }

    std::string ExternalFile::getDelimiter() {
        return this->delimiter;
    }

    void ExternalFile::parse(PharmMLReader &reader, xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        // Get path, format and delimiter
        xml::Node path_node = reader.getSingleElement(node, "./ds:path");
        this->path = path_node.getText();
        xml::Node format_node = reader.getSingleElement(node, "./ds:format");
        this->format = format_node.getText();
        xml::Node delimiter_node = reader.getSingleElement(node, "./ds:delimiter");
        std::string delimiter = delimiter_node.getText();

        // Parse predefined delimiter options
        if (delimiter == "TAB") {
            this->delimiter = "\t";
        } else if (delimiter == "SPACE") {
            this->delimiter = " ";
        } else if (delimiter == "COMMA") {
            this->delimiter = ",";
        } else if (delimiter == "SEMICOLON") {
            this->delimiter = ";";
        } else {
            this->delimiter = delimiter;
        }
    }

    void ExternalFile::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    // Class DataColumn (single column with its definition)
    // Preliminary class (forced conversion of all scalars into AstNode's)
    // TODO: Improve DataColumn data structure typing
    DataColumn::DataColumn(PharmMLReader &reader, xml::Node table_node, ColumnDefinition *definition) {
        this->definition = definition;
        this->parse(reader, table_node);
    }

    void DataColumn::parse(PharmMLReader &reader, xml::Node table_node) {
        // Get values for column from each row element
        int colIndex = (this->definition->getNum() - 1); // Column numbers start at 1
        std::vector<xml::Node> rows = reader.getElements(table_node, "./ds:Row");
        this->numRows = rows.size();
        for (xml::Node row_node : rows) {
            std::vector<xml::Node> values = row_node.getChildren();
            xml::Node value_node = values[colIndex];
            column.push_back(AstNodeFactory::create(reader, value_node));
        }

        // TODO: Support HeaderRow (HeaderDefinition defines HeaderRow as ColumnDefinition defines Row)
    }

    std::vector<std::shared_ptr<AstNode>> DataColumn::getData() {
        return this->column;
    }

    ColumnDefinition *DataColumn::getDefinition() {
        return this->definition;
    }

    std::shared_ptr<AstNode> DataColumn::getElement(int row) {
        return this->column[row - 1];
    }

    int DataColumn::getNumRows() {
        return this->numRows;
    }

    void DataColumn::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    // Class Dataset (top-level of above)
    Dataset::Dataset(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->parse(reader, node);
    }

    void Dataset::parse(PharmMLReader &reader, xml::Node node) {
        // Get definition (specifies headers/columns)
        xml::Node def_node = reader.getSingleElement(node, "./ds:Definition");
        if (def_node.exists()) {
            DatasetDefinition *def = new DatasetDefinition(reader, def_node);
            this->definition = def;
        }

        // Get the table and create (generic) columns
        xml::Node table = reader.getSingleElement(node, "./ds:Table");
        if (table.exists()) {
            int numColumns = this->definition->getNumColumns();
            for (int colNum = 1; colNum <= numColumns; colNum++) {
                ColumnDefinition *definition = this->definition->getColumnDefinition(colNum);
                DataColumn *column = new DataColumn(reader, table, definition);
                columns.push_back(column);
            }
        } else {
            xml::Node ext_file = reader.getSingleElement(node, "./ds:ExternalFile");
            ExternalFile *externalFile = new ExternalFile(reader, ext_file);
            this->externalFile = externalFile;
        }
    }

    xml::Node Dataset::xml() {
        xml::Node ds("DataSet");
        ds.setAttribute("xmlns", "http://www.pharmml.org/pharmml/0.8/Dataset");
        if (this->oid != "") {
            ds.setAttribute("oid", this->oid);
        }
        if (this->definition) {
            ds.addChild(this->definition->xml());
        }
        // TODO: Table/external
        if (this->isExternal()) {
            //~ ds.addChild(this->externalFile->xml());
        } else {
            xml::Node table = ds.createChild("Table");
            int numRows = columns[0]->getNumRows(); // TODO: Better?
            // Columns can't generate the rows so do it all here
            for (int rowNum = 1; rowNum <= numRows; rowNum++) {
                xml::Node row = table.createChild("Row");
                for (DataColumn *column : columns) {
                    std::shared_ptr<AstNode> element = column->getElement(rowNum);
                    XMLAstVisitor xml;
                    element->accept(&xml);
                    row.addChild(xml.getValue());
                }
            }
        }
        return ds;
    }

    // A defined name might be required for visitors
    void Dataset::setName(std::string name) {
        this->name = name;
    }

    std::string Dataset::getName() {
        return this->name;
    }

    DatasetDefinition *Dataset::getDefinition() {
        return this->definition;
    }

    bool Dataset::isExternal() {
        return (externalFile != nullptr);
    }

    ExternalFile *Dataset::getExternal() {
        return externalFile;
    }

    std::vector<DataColumn *> Dataset::getColumns() {
        return this->columns;
    }

    // Get the column with corresponding columnType.
    // If more than one column of the same type exists or if column could not be found return nullptr
    DataColumn *Dataset::getColumnFromType(std::string columnType) {
        DataColumn *found = nullptr;
        for (DataColumn *column : this->columns) {
            ColumnDefinition *cd = column->getDefinition();
            if (cd->getType() == columnType) {
                if (found) {        // Duplicate columns of same columnType
                    return nullptr;
                } else {
                    found = column;
                }
            }
        }
        return found;
    }

    DataColumn *Dataset::getIdvColumn() {
        return this->getColumnFromType("idv");
    }

    void Dataset::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
