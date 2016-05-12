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

#include "Symbol.h"
#include <visitors/SymbRefFinder.h>
#include <iostream>

namespace PharmML
{
    // Experimental Referer class; It's an experiment, all to solve the infamous "referer problem"
    void Referer::addReferences(std::unordered_set<Symbol *> symbols) {
        for (Symbol *symbol : symbols) {
            this->referencedSymbols.addSymbol(symbol);
        }
    }
    
    bool Referer::refersTo(Symbol *symbol) {
        return this->referencedSymbols.dependsOn(symbol);
    }
    
    bool Referer::refersIndirectlyTo(Symbol *symbol) {
        return this->referencedSymbols.hasSymbol(symbol);
    }
    
    std::string Symbol::getSymbId() {
        return this->symbId;
    }

    // Put Symbols into SymbRefs and add Symbols to referencedSymbols from an AST
    // Also return the found symbols (see Consolidator, DistributionParameter and Referer class above)
    std::unordered_set<Symbol *> Symbol::symbRefsFromAst(AstNode *node, std::unordered_map<std::string, Symbol *> &symbolMap) {
        std::unordered_set<Symbol *> found_symbols;
        SymbRefFinder finder;
        node->accept(&finder);
        for (SymbRef *symbref : finder.getSymbRefs()) {
            Symbol *symbol = symbolMap[symbref->toString()];
            symbref->setSymbol(symbol);
            found_symbols.insert(symbol);
            this->referencedSymbols.addSymbol(symbol);
        }
        return found_symbols;
    }
    
    void Symbol::parse(xml::Node node) {
        this->symbId = node.getAttribute("symbId").getValue();
    }
}
