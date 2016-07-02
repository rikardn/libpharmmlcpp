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

#ifndef PHARMMLCPP_DATASET_H_
#define PHARMMLCPP_DATASET_H_

#include <xml/xml.h>
#include <PharmML/PharmMLContext.h>
#include <PharmML/PharmMLSection.h>
#include <AST/AstNode.h>
#include <visitors/AstNodeVisitor.h>
#include <visitors/XMLAstVisitor.h>
#include <AST/Scalar.h>

namespace pharmmlcpp
{
    // Class HeaderDefinition (single header specification of dataset)
    class HeaderDefinition
    {
        public:
            HeaderDefinition(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            xml::Node xml();
            void addHeaderRow(xml::Node node);
            std::string getName();
            int getRowNumber();

        private:
            std::string name;
            int rowNumber;
    };

    // Class ColumnDefinition (single column specification of dataset)
    class ColumnDefinition
    {
        public:
            ColumnDefinition(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            xml::Node xml();
            std::string getId();
            std::string getType();
            std::string getLevel();
            std::string getValueType();
            int getNum();

        private:
            std::string id;
            std::string type;
            std::string level;
            std::string valueType;
            int num;
    };

    // Class DatasetDefinition (header/column specifications of dataset)
    class DatasetDefinition
    {
        public:
            DatasetDefinition(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            xml::Node xml();
            ColumnDefinition *getColumnDefinition(int colNum);
            std::vector<ColumnDefinition *> getColumnDefinitions();
            int getNumColumns();

        private:
            std::vector<HeaderDefinition *> headers;
            std::vector<ColumnDefinition *> columns;
            AstNode *ignoreCondition = nullptr;
            std::string ignoreSymbols; // 1 to 5 non-whitespace characters
    };

    // class ExternalFile (data is stored externally)
    class ExternalFile
    {
        public:
            ExternalFile(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            std::string getOid();
            std::string getPath();
            std::string getFormat();
            std::string getDelimiter();
            void accept(PharmMLVisitor *visitor);

        private:
            std::string oid;
            std::string path;
            std::string format;
            std::string delimiter;
            // TODO: Support MissingDataMapType
    };

    // Class DataColumn (single column with its definition)
    class DataColumn
    {
        public:
            DataColumn(PharmMLReader &reader, xml::Node table_node, ColumnDefinition *definition);
            void parse(PharmMLReader &reader, xml::Node table_node);
            std::vector<AstNode *> getData();
            ColumnDefinition *getDefinition();
            AstNode *getElement(int row);
            int getNumRows();
            void accept(PharmMLVisitor *visitor);

        private:
            ColumnDefinition *definition;
            std::vector<AstNode *> column;
            int numRows;
    };

    // Class Dataset (top-level of above)
    class Dataset : public PharmMLSection
    {
        public:
            Dataset(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            xml::Node xml();
            std::string getOid();
            DatasetDefinition *getDefinition();
            bool isExternal();
            ExternalFile *getExternal();
            std::vector<DataColumn *> getColumns();
            DataColumn *getColumnFromType(std::string columnType);
            DataColumn *getIdvColumn();
            void setName(std::string name);
            std::string getName();
            void accept(PharmMLVisitor *visitor);

        private:
            std::string oid;
            DatasetDefinition *definition = nullptr;
            ExternalFile *externalFile = nullptr;
            std::vector<DataColumn *> columns;
            std::string name;
    };
}

#endif
