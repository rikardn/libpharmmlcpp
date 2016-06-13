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

#ifndef PHARMML_RPHARMMLGENERATOR_H_
#define PHARMML_RPHARMMLGENERATOR_H_

#include <string>
#include <sstream>
#include <visitors/AstNodeVisitor.h>
#include <symbols/FunctionDefinition.h>
#include <symbols/PopulationParameter.h>
#include <symbols/IndividualParameter.h>
#include <symbols/Covariate.h>
#include <symbols/Variable.h>
#include <symbols/DerivativeVariable.h>
#include <symbols/ObservationModel.h>
#include <symbols/IndependentVariable.h>
#include <symbols/RandomVariable.h>
#include <PharmML/PKMacro.h>
#include <PharmML/Distribution.h>
#include <PharmML/ColumnMapping.h>
#include <PharmML/Dataset.h>
#include <PharmML/Interventions.h>
#include <PharmML/Observations.h>
#include <PharmML/Arms.h>
#include <PharmML/DesignSpaces.h>
#include "RAstGenerator.h"
#include <PharmML/Model.h>
#include <generators/TextFormatter.h>
#include "RPharmMLConsolidator.h"

namespace pharmmlcpp
{
    class RPharmMLGenerator : public PharmMLVisitor
    {
        public:
            Consolidator consol;

            std::string getValue();

            std::vector<std::string> genFunctionDefinitions(Model *model);

            void visit(FunctionDefinition *node) override;
            void visit(FunctionArgumentDefinition *node) override;

            void visit(PopulationParameter *node) override;
            void visit(IndividualParameter *node) override;
            void visit(RandomVariable *node) override;
            void visit(VariabilityLevel *node) override;
            void visit(Correlation *node) override;
            void visit(Covariate *node) override;
            void visit(IndependentVariable *node) override;
            void visit(Variable *node) override;
            void visit(DerivativeVariable *node) override;
            void visit(ObservationModel *node) override;
            void visit(Distribution *node) override;
            void visit(ColumnMapping *node) override;

            void visit(ExternalFile *node) override;
            void visit(DataColumn *node) override;
            void visit(Dataset *node) override;
            void visit(TargetMapping *node) override;

            void visit(ExternalDataset *node) override;

            void visit(Interventions *node) override;
            void visit(Administration *node) override;
            void visit(IndividualAdministration *node) override;

            void visit(Observations *node) override;
            void visit(Observation *node) override;
            void visit(IndividualObservations *node) override;
            void visit(ObservationCombination *node) override;

            void visit(Arms *node) override;
            void visit(Arm *node) override;
            void visit(InterventionSequence *node) override;
            void visit(ObservationSequence *node) override;
            void visit(OccasionSequence *node) override;

            void visit(DesignSpaces *node) override;
            void visit(DesignSpace *node) override;

            void visit(ParameterEstimation *node) override;

            void visit(PKMacro *node) override;

        private:
            RAstGenerator ast_gen;
            std::string value;
            void setValue(std::string str);
            std::string accept(AstNode *);
    };
}

#endif
