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

#ifndef PHARMMLCPP_POPEDPASTDERIVATIVESSYMBOLS_H_
#define PHARMMLCPP_POPEDPASTDERIVATIVESSYMBOLS_H_

#include <visitors/StringVisitor.h>
#include <visitors/SymbolVisitor.h>
#include <symbols/Symbol.h>
#include <generators/PopED/PopEDSymbols.h>

namespace pharmmlcpp
{
    class ObservationModel;
    class PopulationParameter;
    class IndividualParameter;
    class RandomVariable;
    class VariabilityLevel;
    class Variable;
    class DerivativeVariable;
    class IndependentVariable;
    class FunctionDefinition;

    class PopEDPastDerivativesSymbols : public PopEDSymbols
    {
        public:
            void visit(ObservationModel *node) override;
            void visit(PopulationParameter *node) override;
            void visit(IndividualParameter *node) override;
            void visit(VariabilityLevel *node) override;
            void visit(IndependentVariable *node) override;
            void visit(Variable *node) override;
            void visit(DerivativeVariable *node) override;
            void visit(RandomVariable *node) override;
            void visit(FunctionDefinition *node) override;
    };
}

#endif
