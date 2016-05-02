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

#ifndef PHARMML_SYMBOLS_H_
#define PHARMML_SYMBOLS_H_

#include <string>
#include "xml.h"
#include "AstNode.h"

namespace PharmML
{
    class Symbol {
        public:
            std::string getSymbId();

        protected:
            std::string symbId;
            void parse(xml::Node node);
    };

    class SymbRef : public AstNode
    {
        std::string blkIdRef;
        std::string symbIdRef;
        Symbol *symbol;

        public:
        SymbRef(std::string ref);
        SymbRef(xml::Node node);
        virtual void accept(AstNodeVisitor *visitor);
        std::string toString();
        std::string getBlkId();
        void setSymbol(Symbol *symbol);
        Symbol *getSymbol();
    };
    
    class ColumnRef : public AstNode
    {
        std::string columnIdRef;

        public:
        ColumnRef(std::string ref);
        ColumnRef(xml::Node node);
        virtual void accept(AstNodeVisitor *visitor);
        std::string toString();
    };
}


#endif
