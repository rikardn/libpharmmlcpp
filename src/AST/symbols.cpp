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

#include "symbols.h"
#include <iostream>

namespace PharmML
{
    SymbRef::SymbRef(std::string ref) {
        this->symbIdRef = ref;
    }

    SymbRef::SymbRef(xml::Node node) {
        this->blkIdRef = node.getAttribute("blkIdRef").getValue();
        this->symbIdRef = node.getAttribute("symbIdRef").getValue();
    }

    void SymbRef::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    std::string SymbRef::getSymbIdRef() {
        return this->symbIdRef;
    }

    std::string SymbRef::getBlkIdRef() {
        return this->blkIdRef;
    }

    std::string SymbRef::getBlkIdRef(std::string defaultBlkId) {
        if (this->blkIdRef.empty()) {
            return defaultBlkId;
        } else {
            return this->blkIdRef;
        }
    }

    void SymbRef::setSymbol(Symbol *symbol) {
        this->symbol = symbol;
    }

    Symbol *SymbRef::getSymbol() {
        return this->symbol;
    }

    ColumnRef::ColumnRef(std::string ref) {
        this->columnIdRef = ref;
    }

    ColumnRef::ColumnRef(xml::Node node) {
        this->columnIdRef = node.getAttribute("columnIdRef").getValue();
    }

    void ColumnRef::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    std::string ColumnRef::toString() {
        return this->columnIdRef;
    }
}
