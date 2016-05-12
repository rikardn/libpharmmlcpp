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

#ifndef PHARMML_SYMBOLSET_H_
#define PHARMML_SYMBOLSET_H_

#include <unordered_set>
#include <vector>
#include <string>

namespace PharmML
{
    class Symbol;
    class SymbolSet {
        public:
            SymbolSet();
            SymbolSet(Symbol *symbol);
            void addSymbol(Symbol *symbol);
            bool hasSymbol(Symbol *symbol);
            bool dependsOn(Symbol *symbol);
            void merge(SymbolSet& set);
            void remove(SymbolSet& set);
            SymbolSet getDependencies();
            SymbolSet getDependenciesNoPass(SymbolSet &nopass);
            std::vector<Symbol *> getOrdered();
            std::vector<Symbol *> getOrderedDependencies();
            std::vector<Symbol *> getOrderedDependenciesNoPass(SymbolSet &nopass);
            std::unordered_set<Symbol *>::iterator begin();
            std::unordered_set<Symbol *>::iterator end();
 
        private:
            std::unordered_set<Symbol *> symbols;
    };
}

#endif
