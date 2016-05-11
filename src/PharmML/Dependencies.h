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

#ifndef PHARMML_DEPENDENCIES_H_
#define PHARMML_DEPENDENCIES_H_

#include <unordered_set>
#include <AST/symbols.h>

namespace PharmML
{
    class Dependencies
    {
        std::unordered_set<PharmML::SymbRef *> symbRefs;
        std::unordered_set<std::string> dependencies;

        public:
        void addDependency(PharmML::SymbRef *symbRef);
        void addDependency(std::string symbol);
        bool hasDependency(PharmML::SymbRef *symbRef);
        bool hasDependency(std::string symbol);
        std::unordered_set<std::string> getDependencySet();
        int numDependencies();
        std::unordered_set<PharmML::SymbRef *> getSymbRefs();
    };
}

#endif