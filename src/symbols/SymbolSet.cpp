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
#include <visitors/SymbolSortVisitor.h>
#include <iostream>

namespace PharmML
{
    SymbolSet::SymbolSet() {
    }

    SymbolSet::SymbolSet(Symbol *symbol) {
        this->addSymbol(symbol);
    }

    SymbolSet::SymbolSet(std::unordered_set<Symbol *> symbol_set) {
        this->addSymbols(symbol_set);
    }

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

    void SymbolSet::addSymbols(std::unordered_set<Symbol *> symbol_set) {
        for (Symbol *symbol : symbol_set) {
            this->addSymbol(symbol);
        }
    }

    void SymbolSet::removeSymbol(Symbol *symbol) {
        this->symbols.erase(symbol);
    }

    bool SymbolSet::hasSymbol(Symbol *symbol) {
       return (this->symbols.count(symbol) > 0);
    }

    bool SymbolSet::isEmpty() {
        return this->symbols.size() == 0;
    }

    // For those moments when you want to traverse, hunt and gather a dependency
    bool SymbolSet::dependsOn(Symbol *symbol) {
        if (this->hasSymbol(symbol)) {
            return true;
        }
        SymbolSet dependencies = this->getDependencies();
        if (dependencies.hasSymbol(symbol)) {
            return true;
        }
        return false;
    }

    void SymbolSet::merge(const SymbolSet& set) {
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

    // Get all dependencies of a set of symbols. Excluding the symbols themselves AND NOT passing through or adding the nopass set
    SymbolSet SymbolSet::getDependenciesNoPass(SymbolSet &nopass) {
        SymbolSet dependencies;

        for (Symbol *symbol : this->symbols) {
            SymbolSet refs = symbol->referencedSymbols;
            refs.remove(nopass);
            dependencies.merge(refs);
        }

        int prev_count = 0;
        int count = dependencies.symbols.size();
        while (count > prev_count) {
            for (Symbol *symbol : dependencies.symbols) {
                SymbolSet refs = symbol->referencedSymbols;
                refs.remove(nopass);
                dependencies.merge(refs);
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
                for (std::vector<Symbol *>::size_type j = 0; j != ordered.size(); ++j) {
                    if (ordered[j]->referencedSymbols.hasSymbol(symbol)) {
                        ordered.insert(ordered.begin() + j, symbol);
                        inserted = true;
                        break;
                    }
                }
                if (!inserted) {
                    ordered.push_back(symbol);
                }
            }
        }
        return ordered;
    }

    // Order the dependencies of the symbols of this SymbolSet
    std::vector<Symbol *> SymbolSet::getOrderedDependencies() {
        return this->getDependencies().getOrdered();
    }

    std::vector<Symbol *> SymbolSet::getOrderedDependenciesNoPass(SymbolSet &nopass) {
        return this->getDependenciesNoPass(nopass).getOrdered();
    }

    // Return the set of all derivatives contained in this set
    SymbolSet SymbolSet::getDerivatives() {
        SymbolSortVisitor sorter;
        for (Symbol *symbol : this->symbols) {
            symbol->accept(&sorter);
        }
        return sorter.getDerivatives();
    }

    bool SymbolSet::hasDerivatives() {
        return !this->getDerivatives().isEmpty();
    }

    SymbolSet SymbolSet::getRandomVariables() {
        SymbolSortVisitor sorter;
        for (Symbol *symbol : this->symbols) {
            symbol->accept(&sorter);
        }
        return sorter.getRandomVariables();
    }
            
    SymbolSet SymbolSet::getPopulationParameters() {
        SymbolSortVisitor sorter;
        for (Symbol *symbol : this->symbols) {
            symbol->accept(&sorter);
        }
        return sorter.getPopulationParameters();
    }

    SymbolSet SymbolSet::getParameters() {
        SymbolSortVisitor sorter;
        for (Symbol *symbol : this->symbols) {
            symbol->accept(&sorter);
        }
        return sorter.getParameters();
    }

    SymbolSet SymbolSet::getCovariates() {
        SymbolSortVisitor sorter;
        for (Symbol *symbol : this->symbols) {
            symbol->accept(&sorter);
        }
        return sorter.getCovariates();
    }
}
