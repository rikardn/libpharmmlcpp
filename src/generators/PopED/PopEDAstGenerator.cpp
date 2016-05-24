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

#include <iostream>
#include <generators/PopED/PopEDAstGenerator.h>
#include <symbols/PopulationParameter.h>
#include <generators/PopED/PopEDSymbols.h>

namespace PharmML
{
    PopEDAstGenerator::PopEDAstGenerator() {
        this->symbgen = new PopEDSymbols();
    }

    PopEDAstGenerator::PopEDAstGenerator(PopEDSymbols *symbgen) {
        this->symbgen = symbgen;
    }

    PopEDAstGenerator::~PopEDAstGenerator() {
        //FIXME: How to delete this?  delete this->symbgen;
    }

    void PopEDAstGenerator::visit(SymbRef *node) {
        Symbol *symbol = node->getSymbol();
        symbol->accept(this->symbgen);
        this->setValue(this->symbgen->getValue());
    }
}
