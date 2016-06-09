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

#ifndef PHARMML_OBSERVATIONMODEL_H_
#define PHARMML_OBSERVATIONMODEL_H_

#include <PharmML/PharmMLContext.h>
#include <AST/AstNode.h>
#include <xml/xml.h>
#include <visitors/PharmMLVisitor.h>
#include <visitors/SymbolVisitor.h>
#include <symbols/VariabilityLevel.h>
#include <PharmML/PharmMLSection.h>
#include <PharmML/Block.h>

namespace PharmML
{
    class ObservationModel : public Block, public Symbol, public PharmMLSection
    {
        public:
            ObservationModel(PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);

            // Type of observation data
            bool isContinuous(); // Standard, general och distributional
            bool isCategorical();
            bool isCount();
            bool isTTE();

            // Type of observation model
            bool hasStandardErrorModel(); // u(y) = u(f) + g * eps
            bool hasGeneralErrorModel(); // h(y) = H(f, xi, eps)
            bool hasDistributionalErrorModel(); // u(y) ~ distribution(parameter1, parameter2, ...)

            // Transformations (LHS or LHS & RHS)
            std::string getTransformation();
            // getTransformationParameters()

            // Standard error model
            SymbRef *getOutput();
            AstNode *getErrorModel();
            SymbRef *getResidualError();

            // General error model
            AstNode *getAssignment();

            // Distributional error model
            std::vector<PharmML::VariabilityReference *> getVariabilityReferences();

            // Convenience methods
            SymbolSet getNeededSymbols();

            void setupSymbRefs(SymbolGathering &gathering, std::string blkId);
            void gatherSymbols(SymbolGathering &gathering) override;
            void accept(PharmMLVisitor *visitor);
            void accept(SymbolVisitor *visitor);

        private:
            // Common to all observation models
            std::string name;

            // Type of observation data
            bool continuousData = false;
            bool categoricalData = false;
            bool countData = false;
            bool tteData = false;

            // Type of observation model
            bool standardErrorModel = false;
            bool generalErrorModel = false;
            bool distributionalErrorModel = false;

            // Transformations (on LHS or both)
            std::string transformation;
            std::vector<PharmML::AstNode *> transformationParameters;

            // Standard error model
            SymbRef *output = nullptr;
            AstNode *errorModel = nullptr;
            SymbRef *residualError = nullptr;

            // General error model
            AstNode *generalAssignment = nullptr;

            // Distributional error model
            std::vector<PharmML::VariabilityReference *> variabilityReferences;
    };
}

#endif
