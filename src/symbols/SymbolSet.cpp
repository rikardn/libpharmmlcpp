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
    void SymbolSet::addSymbol(Symbol *symbol) {
        this->symbols.insert(symbol);
    }

    bool SymbolSet::hasSymbol(Symbol *symbol) {
       return (this->symbols.count(symbol) > 0); 
    }

    void SymbolSet::merge(SymbolSet& set) {
        this->symbols.insert(set.symbols.begin(), set.symbols.end()); 
    }

    // Get all dependencies of a set of symbols
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

        return dependencies; 
    }

    std::vector<Symbol *> SymbolSet::getOrderedDependencies() {

    }
}
