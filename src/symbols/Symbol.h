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

#ifndef PHARMMLCPP_SYMBOL_H_
#define PHARMMLCPP_SYMBOL_H_

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <xml/xml.h>
#include <AST/AstNode.h>

#include <visitors/SymbolVisitor.h>
#include <symbols/SymbolSet.h>
#include <AST/symbols.h>
#include <PharmML/Block.h>
#include <helpers/SymbolNamer.h>

namespace pharmmlcpp
{
    // Experimental Referer class; It's an experiment, all to solve the infamous "referer problem"
    class SymbolNamer;
    class Referer
    {
        public:
            SymbolSet referencedSymbols;
            Symbol *addSymbRef(SymbRef *symbRef, SymbolGathering &gathering, std::string blkId);
            void addReference(Symbol *symbol);
            void addReferences(std::unordered_set<Symbol *> symbols);
            bool refersTo(Symbol *symbol);
            bool refersIndirectlyTo(Symbol *symbol);
            void setupAstSymbRefs(AstNode *node, SymbolGathering &gathering, std::string blkId);
            virtual void setupSymbRefs(SymbolGathering &gathering, std::string blkId) = 0;

        protected:
            // FIXME: Why haven't FunctionCall got parse(..) like normal nice classes?
            //~ virtual void parse(xml::Node node) = 0;
    };

    class Symbol : public Referer
    {
        public:
            std::string getSymbId();
            std::string getName();
            void setSymbolNamer(SymbolNamer *namer);
            virtual void accept(SymbolVisitor *visitor) = 0;

        protected:
            std::string symbId;
            void parse(xml::Node node);

        private:
            SymbolNamer *namer = nullptr;
    };
}

#endif
