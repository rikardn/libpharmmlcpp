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

#ifndef PHARMML_POPEDSYMBOLS_H_
#define PHARMML_POPEDSYMBOLS_H_

#include <unordered_map>
#include <visitors/StringVisitor.h>
#include <visitors/SymbolVisitor.h>
#include <symbols/Symbol.h>

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

    class PopEDSymbols : public SymbolVisitor, public StringVisitor
    {
        private:
            int next_popparm = 1;
            int next_randvar = 1;
            std::unordered_map<Symbol *, int> symbol_numbermap;

        public:
            virtual void visit(ObservationModel *node);
            virtual void visit(PopulationParameter *node);
            virtual void visit(IndividualParameter *node);
            virtual void visit(VariabilityLevel *node);
            virtual void visit(IndependentVariable *node);
            virtual void visit(Variable *node);
            virtual void visit(DerivativeVariable *node);
            virtual void visit(RandomVariable *node);
            virtual void visit(Covariate *node);
    };
}

#endif
