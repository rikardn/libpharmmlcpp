/* libpharmml cpp - Library to handle PharmML
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

#ifndef PHARMML_POPEDGENERATOR_H_
#define PHARMML_POPEDGENERATOR_H_

#include <string>
#include <visitors/AstNodeVisitor.h>
#include <symbols/PopulationParameter.h>
#include <symbols/IndividualParameter.h>
#include <symbols/Variable.h>
#include <PharmML/PKMacro.h>
#include <generators/R/RAstGenerator.h>
#include <generators/R/RPharmMLGenerator.h>
#include <visitors/PharmMLVisitor.h>
#include <symbols/ObservationModel.h>
#include <PharmML/Model.h>
#include <generators/PopED/PopEDAstGenerator.h>
#include <generators/TextFormatter.h>
#include <generators/R/RSymbols.h>
#include <generators/PopED/PopEDPastDerivativesSymbols.h>
#include <generators/PopED/PopEDErrorAstGenerator.h>
#include <helpers/Logger.h>
#include <generators/PopED/PopEDObjects.h>

namespace PharmML
{
    class PopEDGenerator : public PharmMLVisitor
    {
        private:
            Logger logger;
            RAstGenerator ast_gen;
            RSymbols r_symb;
            PopEDAstGenerator poped_astgen;
            RPharmMLGenerator r_gen;
            Model *model;

            SymbolSet remaining_parameters;     // THETAs not connected to an IndividualVariable

            std::string value;
            void setValue(std::string str);
            std::string accept(AstNode *);
            std::string genParameterModel();
            std::string genODEFunc();
            std::string genStructuralModel();
            std::string genErrorFunction();
            std::string genDatabaseCall();
            std::vector<std::string> genDoseTimeNames();
            std::vector<std::string> genDoseAmountNames();
            std::string getDoseVariable();

        public:
            std::string getValue();
            std::string generateModel(Model *model);

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
    };
}

#endif
