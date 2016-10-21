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
#include <symbols/SymbolGathering.h>
#include <PharmML/PharmML.h>

namespace pharmmlcpp
{
    // Convenience method for naked SymbRefs.
    pharmmlcpp::Symbol *Referer::addSymbRef(SymbRef *symbRef, SymbolGathering &gathering, std::string blkId) {
        Symbol *found_symbol = gathering.getSymbol(symbRef->getBlkIdRef(blkId), symbRef->getSymbIdRef());
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
    void Referer::setupAstSymbRefs(AstNode *node, SymbolGathering &gathering, std::string blkId) {
        SymbRefFinder finder;
        node->accept(&finder);
        for (SymbRef *symbref : finder.getSymbRefs()) {
            Symbol *symbol = gathering.getSymbol(symbref->getBlkIdRef(blkId), symbref->getSymbIdRef());
            if (!symbol) {
                symbol = gathering.getSymbol(symbref->getBlkIdRef(""), symbref->getSymbIdRef());
            }
            symbref->setSymbol(symbol);
            this->referencedSymbols.addSymbol(symbol);
        }
    }

    std::string Symbol::getSymbId() {
        return this->symbId;
    }

    void Symbol::setSymbId(std::string symbId) {
        this->symbId = symbId;
    }

    // Get the name mangled through the global model symbolMangler
    std::string Symbol::getName() {
        if (this->namer) {
            return this->namer->getNameString(this);
        } else {        // No namer was installed. Do the most sensible
            return this->symbId;
        }
    }

    void Symbol::setSymbolNamer(SymbolNamer *namer) {
        this->namer = namer;
    }

    void Symbol::parse(xml::Node node) {
        this->symbId = node.getAttribute("symbId").getValue();
    }

    void Symbol::xml(PharmMLWriter &writer, xml::Node node) {
        node.setAttribute("symbId", this->getSymbId());
    }
}
