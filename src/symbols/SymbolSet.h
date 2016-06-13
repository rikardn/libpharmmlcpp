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

#ifndef PHARMMLCPP_SYMBOLSET_H_
#define PHARMMLCPP_SYMBOLSET_H_

#include <unordered_set>
#include <vector>
#include <string>

namespace pharmmlcpp
{
    class Symbol;
    class SymbolSet {
        public:
            SymbolSet();
            SymbolSet(Symbol *symbol);
            SymbolSet(std::unordered_set<Symbol *> symbol_set);
            void addSymbol(Symbol *symbol);
            void addSymbols(std::unordered_set<Symbol *> symbol_set);
            void remove(Symbol *symbol);
            void remove(SymbolSet& set);
            bool hasSymbol(Symbol *symbol);
            bool isEmpty();
            bool dependsOn(Symbol *symbol);
            void merge(const SymbolSet& set);
            SymbolSet getDependencies();
            SymbolSet getDependenciesNoPass(SymbolSet &nopass);
            std::vector<Symbol *> getOrdered();
            std::vector<Symbol *> getOrderedDependencies();
            std::vector<Symbol *> getOrderedDependenciesNoPass(SymbolSet &nopass);
            SymbolSet getDerivatives();
            SymbolSet getRandomVariables();
            SymbolSet getPopulationParameters();
            SymbolSet getParameters();
            SymbolSet getCovariates();
            SymbolSet getIndividualParameters();
            SymbolSet getAllParameters();       // Parameters, PopulationParameters, IndividualParameters and RandomVariables
            bool hasDerivatives();
            int numSymbols();
            std::unordered_set<Symbol *>::iterator begin();
            std::unordered_set<Symbol *>::iterator end();

        protected:
            std::unordered_set<Symbol *> symbols;
    };
}

#endif
