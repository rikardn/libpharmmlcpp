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

#include <generators/MDL/MDLSymbols.h>
#include <symbols/PopulationParameter.h>
#include <symbols/IndividualParameter.h>
#include <symbols/RandomVariable.h>
#include <symbols/IndependentVariable.h>
#include <symbols/Variable.h>
#include <symbols/DerivativeVariable.h>


namespace PharmML
{
    void MDLSymbols::visit(PopulationParameter *node) {
        if (this->symbol_numbermap.count(node) == 0) {
            this->symbol_numbermap[node] = next_popparm++;
        }
        this->setValue(std::to_string(this->symbol_numbermap[node]));
    }

    void MDLSymbols::visit(IndividualParameter *node) {
        this->setValue(node->getSymbId());
    }

    void MDLSymbols::visit(RandomVariable *node) {
        if (this->symbol_numbermap.count(node) == 0) {
            this->symbol_numbermap[node] = next_randvar++;
        }
        this->setValue(std::to_string(this->symbol_numbermap[node]));
    }

    void MDLSymbols::visit(IndependentVariable *node) {
        this->setValue(node->getSymbId());
    }

    void MDLSymbols::visit(Variable *node) {
        this->setValue(node->getSymbId());
    }

    void MDLSymbols::visit(DerivativeVariable *node) {
        this->setValue(node->getSymbId());
    }
}
