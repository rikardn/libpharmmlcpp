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

#include "Dependencies.h"

namespace PharmML
{
    void Dependencies::addDependency(PharmML::SymbRef *symbRef) {
        this->symbRefs.insert(symbRef);
        this->dependencies.insert(symbRef->toString());
    }
    
    void Dependencies::addDependency(std::string symbol) {
        this->dependencies.insert(symbol);
    }
    
    bool Dependencies::hasDependency(PharmML::SymbRef *symbRef) {
        return (this->dependencies.count(symbRef->toString()) > 0);
    }
    
    bool Dependencies::hasDependency(std::string symbol) {
        return (this->dependencies.count(symbol) > 0);
    }
    
    std::unordered_set<PharmML::SymbRef *> Dependencies::getSymbRefs() {
        return this->symbRefs;
    }

    int Dependencies::numDependencies() {
        return this->dependencies.size();
    }
}
