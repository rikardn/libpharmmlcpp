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

#include "SymbolSet.h"
#include <symbols/Symbol.h>

namespace PharmML
{
    // Make the SymbolSet iterable
    std::unordered_set<Symbol *>::iterator SymbolSet::begin() {
        return this->symbols.begin();
    }

    std::unordered_set<Symbol *>::iterator SymbolSet::end() {
        return this->symbols.end();
    }

    void SymbolSet::addSymbol(Symbol *symbol) {
        this->symbols.insert(symbol);
    }

    bool SymbolSet::hasSymbol(Symbol *symbol) {
       return (this->symbols.count(symbol) > 0); 
    }

    void SymbolSet::merge(SymbolSet& set) {
        this->symbols.insert(set.symbols.begin(), set.symbols.end()); 
    }

    void SymbolSet::remove(SymbolSet& set) {
        for (Symbol *symbol : set) {
            this->symbols.erase(symbol);
        }
    }

    // Get all dependencies of a set of symbols. Excluding the symbols themselves
    SymbolSet SymbolSet::getDependencies() {
        SymbolSet dependencies;

        for (Symbol *symbol : this->symbols) {
            dependencies.merge(symbol->referencedSymbols);
        }

        int prev_count = 0;
        int count = dependencies.symbols.size();
        while (count > prev_count) {
            for (Symbol *symbol : dependencies.symbols) {
                dependencies.merge(symbol->referencedSymbols);
            }
            prev_count = count;
            count = dependencies.symbols.size();
        }

        dependencies.remove(*this);  // Remove the symbols we started with

        return dependencies; 
    }

    // Order the symbols in this SymbolSet
    std::vector<Symbol *> SymbolSet::getOrdered() {
        std::vector<Symbol *> ordered;
        for (Symbol *symbol : this->symbols) {
            // add first element on top
            if (ordered.size() == 0) {
                ordered.push_back(symbol);
            } else {
                bool inserted = false;
                for (auto j = ordered.begin(); j < ordered.end(); j++) {
                    if (ordered[j - ordered.begin()]->referencedSymbols.hasSymbol(symbol)) {
                        ordered.insert(j, symbol);
                        inserted = true;
                        break;
                    }
                    if (!inserted) {
                        ordered.push_back(symbol);
                    }
                }
            }
        }
        return ordered;
    }

    // Order the dependencies of the symbols of this SymbolSet
    std::vector<Symbol *> SymbolSet::getOrderedDependencies() {
        return this->getDependencies().getOrdered();
    }
}
