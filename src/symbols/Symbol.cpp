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
    /* Convenience method for those Referer derived classes that have naked SymbRef's. Also returns
     * Symbol so gatherer also can add them to a separate collection if deemed necessary. */
    PharmML::Symbol *Referer::addSymbRef(SymbRef *symbRef, std::unordered_map<std::string, Symbol *> &symbolMap) {
        Symbol *found_symbol = symbolMap[symbRef->toString()];
        symbRef->setSymbol(found_symbol);
        this->referencedSymbols.addSymbol(found_symbol);
        return found_symbol;
    }
    void Referer::addReference(Symbol *symbol) {
        this->referencedSymbols.addSymbol(symbol);
    }

    void Referer::addReferences(std::unordered_set<Symbol *> symbols) {
        for (Symbol *symbol : symbols) {
            this->referencedSymbols.addSymbol(symbol);
        }
    }

    bool Referer::refersTo(Symbol *symbol) {
        return this->referencedSymbols.dependsOn(symbol);
    }

    // FIXME: Isn't this misnamed? This seems to check shallowly!
    bool Referer::refersIndirectlyTo(Symbol *symbol) {
        return this->referencedSymbols.hasSymbol(symbol);
    }

    // Put Symbols into SymbRefs and add Symbols to referencedSymbols from an AST
    // Also return the found symbols (see Consolidator, DistributionParameter and Referer class)
    std::unordered_set<Symbol *> Referer::setupAstSymbRefs(AstNode *node, std::unordered_map<std::string, Symbol *> &symbolMap) {
        std::unordered_set<Symbol *> found_symbols;
        //~ SymbRefFinder finder; // See comment in SymbRefFinder.cpp
        SymbRefFinder finder(&symbolMap);
        node->accept(&finder);
        for (SymbRef *symbref : finder.getSymbRefs()) {
            Symbol *symbol = symbolMap[symbref->toString()];
            symbref->setSymbol(symbol);
            found_symbols.insert(symbol);
            this->referencedSymbols.addSymbol(symbol);
        }
        return found_symbols;
    }

    std::string Symbol::getSymbId() {
        return this->symbId;
    }

    void Symbol::parse(xml::Node node) {
        this->symbId = node.getAttribute("symbId").getValue();
        /* Here it may be tempting to place blkId puller as well but keep in mind that for e.g.
         * ObservationModel's this is on root node while symbId is further down. I.e.
         * Symbol::parse(..) argument list needs to take two nodes as argument. */
    }
}
