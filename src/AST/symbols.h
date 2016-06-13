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

namespace pharmmlcpp
{
    class SymbRef : public AstNode
    {
        public:
            SymbRef(std::string ref);
            SymbRef(xml::Node node);
            virtual void accept(AstNodeVisitor *visitor);
            std::string getSymbIdRef();
            std::string getBlkIdRef();
            std::string getBlkIdRef(std::string defaultBlkId);
            void setSymbol(Symbol *symbol);
            Symbol *getSymbol();

        private:
            std::string blkIdRef;
            std::string symbIdRef;
            Symbol *symbol;
    };

    class ColumnRef : public AstNode
    {
        public:
            ColumnRef(std::string ref);
            ColumnRef(xml::Node node);
            virtual void accept(AstNodeVisitor *visitor);
            std::string toString();
        
        private:
            std::string columnIdRef;
    };
}

#endif
