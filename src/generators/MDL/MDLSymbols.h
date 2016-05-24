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

#ifndef PHARMML_MDLSYMBOLS_H_
#define PHARMML_MDLSYMBOLS_H_

#include <memory>
#include <unordered_map>
#include <visitors/StringVisitor.h>
#include <visitors/SymbolVisitor.h>
#include <symbols/Symbol.h>
#include <generators/TextFormatter.h>
#include <helpers/Logger.h>

namespace PharmML
{
    class MDLAstGenerator;

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

    class MDLSymbols : public SymbolVisitor, public StringVisitor
    {
        private:
            std::shared_ptr<Logger> logger;
            std::unique_ptr<MDLAstGenerator> ast_gen;

        public:
            MDLSymbols(std::shared_ptr<Logger> logger);

            void visit(ObservationModel *node) override;
            void visit(PopulationParameter *node) override;
            void visit(IndividualParameter *node) override;
            void visit(RandomVariable *node) override;
            void visit(VariabilityLevel *node) override;
            void visit(IndependentVariable *node) override;
            void visit(Variable *node) override;
            void visit(DerivativeVariable *node) override;
            void visit(Covariate *node) override;

            void visit(FunctionDefinition *node) override;
            void visit(FunctionArgumentDefinition *node) override;
    };
}

#endif
