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

#ifndef PHARMML_RSYMBOLS_H_
#define PHARMML_RSYMBOLS_H_

#include <unordered_map>
#include <visitors/StringVisitor.h>
#include <visitors/SymbolVisitor.h>
#include <symbols/Symbol.h>
#include <generators/R/RAstGenerator.h>

namespace PharmML
{
    class ObservationModel;
    class PopulationParameter;
    class IndividualParameter;
    class RandomVariable;
    class VariabilityLevel;
    class Variable;
    class DerivativeVariable;
    class IndependentVariable;
    class Covariate;

    class RSymbols : public SymbolVisitor, public StringVisitor
    {
        private:
            RAstGenerator *astgen;

        public:
            RSymbols();
            RSymbols(RAstGenerator *astgen);
            ~RSymbols();
            void visit(ObservationModel *node);
            void visit(PopulationParameter *node);
            void visit(IndividualParameter *node);
            void visit(RandomVariable *node);
            void visit(VariabilityLevel *node);
            void visit(IndependentVariable *node);
            void visit(Variable *node);
            void visit(DerivativeVariable *node);
            void visit(Covariate *node);
    };
}

#endif
