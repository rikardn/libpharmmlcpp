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

#ifndef PHARMML_SYMBOL_H_
#define PHARMML_SYMBOL_H_

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <xml/xml.h>
#include <AST/AstNode.h>

#include <visitors/SymbolVisitor.h>
#include <symbols/SymbolSet.h>
#include <AST/symbols.h>

namespace PharmML
{
    // Experimental Referer class; It's an experiment, all to solve the infamous "referer problem"
    class Referer
    {
        public:
            PharmML::SymbolSet referencedSymbols;
            PharmML::Symbol *addSymbRef(SymbRef *symbRef, std::unordered_map<std::string, Symbol *> &symbolMap);
            void addReference(Symbol *symbol);
            void addReferences(std::unordered_set<Symbol *> symbols);
            bool refersTo(Symbol *symbol);
            bool refersIndirectlyTo(Symbol *symbol);
            std::unordered_set<Symbol *> symbRefsFromAst(AstNode *node, std::unordered_map<std::string, Symbol *> &symbolMap);

        protected:
            // FIXME: Why haven't FunctionCall got parse(..) like normal nice classes?
            //~ virtual void parse(xml::Node node) = 0;
    };

    class Symbol : public Referer
    {
        public:
            std::string getSymbId();
            virtual void gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) = 0;
            virtual void accept(SymbolVisitor *visitor) = 0;

        protected:
            std::string symbId;
            void parse(xml::Node node);
    };
}

#endif
