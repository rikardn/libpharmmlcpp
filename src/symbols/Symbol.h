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

namespace PharmML
{
    // Experiment (if sucessful, maybe another file should contain it)
    class Referer
    {
        public:
            virtual void addReferences(std::unordered_set<Symbol *> symbol);
            virtual bool refersTo(Symbol *symbol);
            virtual bool refersIndirectlyTo(Symbol *symbol);

        protected:
            void parse(xml::Node node);
            
        private:
            SymbolSet referencedSymbols; // No need to expose this considering the task of a Referer
    };
    
    class Symbol {
        public:
            SymbolSet referencedSymbols;
            std::string getSymbId();
            virtual void gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) = 0;
            virtual void accept(SymbolVisitor *visitor) = 0;

        protected:
            std::string symbId;
            std::unordered_set<Symbol *> symbRefsFromAst(AstNode *node, std::unordered_map<std::string, Symbol *> &symbolMap);
            void parse(xml::Node node);
    };
}

#endif
