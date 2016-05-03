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

#include "RPharmMLConsolidator.h"
#include "RPharmMLGenerator.h"

namespace PharmML
{
    // Derivatives consolidator (visits to DerivativeVariable builds)
    void DerivativesConsolidator::addDerivative(std::string y, std::string x, std::string y0, std::string x0) {
        this->y.push_back(y);
        this->x.push_back(x);
        this->y0.push_back(y0);
        this->x0.push_back(x0);
    }

    std::vector<std::string> DerivativesConsolidator::getSymbols() {
        return this->y;
    }

    std::vector<std::string> DerivativesConsolidator::getAssigns() {
        return this->x;
    }

    std::string DerivativesConsolidator::genInitVector() {
        std::vector<std::string> pairs;
        for (int i = 0; i < this->y.size(); i++) {
            std::string pair = this->y[i] + "=" + this->y0[i];
            pairs.push_back(pair);
        }
        // TODO: Figure out place for helpers
        return Text::formatVector(pairs, "c", "");
    }

    // Variables consolidator (visits to Variable builds)
    void VariablesConsolidator::addVariable(std::string symbol, std::string assign) {
        this->symbols.push_back(symbol);
        this->assigns.push_back(assign);
    }

    std::string VariablesConsolidator::genStatements() {
        // Generate standard R assigns of all symbols and expressions
        Text::Indenter ind;

        for (int i = 0; i < symbols.size(); i++) {
            ind.addRow(this->symbols[i] + " <- " + this->assigns[i]);
        }

        return ind.createString(); 
    }
}
