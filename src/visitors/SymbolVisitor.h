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

#ifndef PHARMML_SYMBOLVISITOR_H_
#define PHARMML_SYMBOLVISITOR_H_

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

    class FunctionDefinition;
    class FunctionArgumentDefinition;

    class SymbolVisitor
    {
        public:
            virtual void visit(ObservationModel *node) = 0;
            virtual void visit(PopulationParameter *node) = 0;
            virtual void visit(IndividualParameter *node) = 0;
            virtual void visit(RandomVariable *node) = 0;
            virtual void visit(VariabilityLevel *node) = 0;
            virtual void visit(IndependentVariable *node) = 0;
            virtual void visit(Variable *node) = 0;
            virtual void visit(DerivativeVariable *node) = 0;
            virtual void visit(Covariate *node) = 0;

            virtual void visit(FunctionDefinition *node) = 0;
            virtual void visit(FunctionArgumentDefinition *node) = 0;
    };
}

#endif
