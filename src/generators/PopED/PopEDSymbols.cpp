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

#include <generators/PopED/PopEDSymbols.h>
#include <symbols/PopulationParameter.h>
#include <symbols/IndividualParameter.h>
#include <symbols/RandomVariable.h>
#include <symbols/IndependentVariable.h>
#include <symbols/Variable.h>
#include <symbols/DerivativeVariable.h>


namespace PharmML
{
    void PopEDSymbols::visit(PopulationParameter *node) {
        this->setValue("popParm");
    }

    void PopEDSymbols::visit(IndividualParameter *node) {
        this->setValue(node->getSymbId());
    }

    void PopEDSymbols::visit(RandomVariable *node) {
        this->setValue("randVar");
    }

    void PopEDSymbols::visit(IndependentVariable *node) {
        this->setValue(node->getSymbId());
    }

    void PopEDSymbols::visit(Variable *node) {
        this->setValue(node->getSymbId());
    }

    void PopEDSymbols::visit(DerivativeVariable *node) {
        this->setValue(node->getSymbId());
    }
}
