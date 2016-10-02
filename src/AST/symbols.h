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

#ifndef PHARMMLCPP_SYMBOLS_H_
#define PHARMMLCPP_SYMBOLS_H_

#include <string>
#include <xml/xml.h>
#include <AST/AstNode.h>
#include <symbols/Symbol.h>
#include <PharmML/PharmMLWriter.h>
#include <PharmML/PharmMLReader.h>

namespace pharmmlcpp
{
    /**
     *  A reference to a symbol
     */
    class SymbRef : public AstNode
    {
        public:
            SymbRef(std::string ref);
            SymbRef(PharmMLReader &reader, xml::Node node);
            std::string getSymbIdRef();
            void setSymbIdRef(std::string symbId);
            std::string getBlkIdRef();
            void setBlkIdRef(std::string blkId);
            std::string getBlkIdRef(std::string defaultBlkId);
            void setSymbol(Symbol *symbol);
            Symbol *getSymbol();
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;

        private:
            std::string blkIdRef;
            std::string symbIdRef;
            Symbol *symbol = nullptr;
    };

    class ColumnRef : public AstNode
    {
        public:
            ColumnRef(std::string ref);
            ColumnRef(PharmMLReader &reader, xml::Node node);
            std::string toString();
            std::string getColumnIdRef();
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;

        private:
            std::string columnIdRef;
    };

    class CatRef : public AstNode
    {
        public:
            CatRef(std::string ref);
            CatRef(PharmMLReader &reader, xml::Node node);

            std::string getCatRef();
            void setCatRef(std::string symbId);

            std::string getBlkIdRef();
            void setBlkIdRef(std::string blkId);
            std::string getBlkIdRef(std::string defaultBlkId);

            std::string toString();

            // FIXME: Use implemented Category class in new reference system
            // void setCategory(Category *category);
            // Category *getCategory();

            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;

        private:
            std::string blkIdRef;
            std::string catRef;
            // Category *category = nullptr; // FIXME: Use implemented Category class in new reference system
    };
}

#endif
