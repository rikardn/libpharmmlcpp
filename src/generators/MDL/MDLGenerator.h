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

#ifndef PHARMMLCPP_MDLPHARMMLGENERATOR_H_
#define PHARMMLCPP_MDLPHARMMLGENERATOR_H_

#include <memory>
#include <string>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
typedef std::pair<std::string, std::string> stringpair;
typedef std::unordered_map<std::string, std::string> stringmap;

#include <visitors/AstNodeVisitor.h>
#include <visitors/AstAnalyzer.h>
#include <symbols/FunctionDefinition.h>
#include <symbols/PopulationParameter.h>
#include <symbols/IndividualParameter.h>
#include <symbols/Covariate.h>
#include <symbols/Variable.h>
#include <symbols/DerivativeVariable.h>
#include <symbols/ObservationModel.h>
#include <symbols/IndependentVariable.h>
#include <symbols/RandomVariable.h>
#include <PharmML/Distribution.h>
#include <PharmML/ColumnMapping.h>
#include <PharmML/Dataset.h>
#include <PharmML/ModellingSteps.h>
#include <generators/MDL/MDLSymbols.h>
#include "MDLAstGenerator.h" // TODO: Diverge from RAstGenerator (now a copy)
#include "MDLColumnMappingAstGenerator.h" // TODO: More elegant parser of Piecewise trees in ColumnMapping
#include <PharmML/PharmML.h>
#include <generators/TextFormatter.h>

#include <consolidators/Consolidator.h>
#include <consolidators/PopulationParameters.h>

#include <generators/MDL/MDLSymbolNamer.h>

#include <helpers/Logger.h>

namespace pharmmlcpp
{
    struct MDLObject {
        std::string name;
        std::string code;
    };

    struct MDLObjects {
        std::vector<MDLObject> data;
        std::vector<MDLObject> parameter;
        std::vector<MDLObject> model;
        std::vector<MDLObject> task;
        std::vector<MDLObject> mog;
        std::vector<MDLObject> design;
    };

    class MDLGenerator : public PharmMLVisitor
    {
        public:
            MDLGenerator();
            std::string getValue();
            std::vector<std::string> getValues();
            stringpair getPairValue();
            stringmap getHashValue();

            std::string generateModel(PharmML *model);

            std::string genDataObj(ExternalDataset *ext_ds);
            std::string genDataInputVariablesBlock(Dataset *node, std::vector<ColumnMapping *> col_maps, std::vector<std::string> &declared_vars);

            std::string genParObj(ParameterModel *par_model, ModellingSteps *msteps);
            std::string genStructuralBlock(std::vector<PopulationParameter *> structural_params, EstimationStep *est_step);
            std::string genVariabilityBlock(std::vector<PopulationParameter *> variability_params, EstimationStep *est_step);
            std::string genRandomVariableDefinitionBlock(VariabilityLevel *level, std::vector<RandomVariable *> rand_vars, std::vector<Correlation *> corrs);
            std::string genIndividualVariablesBlock(std::vector<pharmmlcpp::IndividualParameter *> individualParameters);
            std::string genModelPredictionBlock(pharmmlcpp::StructuralModel *structuralModel);
            std::string genCompartmentBlock(pharmmlcpp::PKMacros *pk_macros);
            std::string genObservationBlock(pharmmlcpp::PharmML *model);

            std::string genMdlObj(pharmmlcpp::PharmML *model);
            std::string genTaskObj();
            std::string genDesignObj(PharmML *model);
            void genDesignParameters(TextFormatter &form, std::vector<DesignParameter *> designParameters);
            void genDesignIntervention(TextFormatter &form, Interventions *interventions);
            void genDesignSampling(TextFormatter &form, Observations *observations);
            void genDesignSpaces(TextFormatter &form, DesignSpaces *design_spaces);
            void genStudyDesign(TextFormatter &form, Arms *arms);
            std::string genDesignDeclaredVariables();
            std::string genMogObj(MDLObjects &objects);
            std::string genCompleteMDL(MDLObjects &objects);

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
            std::unique_ptr<MDLAstGenerator> ast_gen;
            std::unique_ptr<MDLColumnMappingAstGenerator> col_map_ast_gen;
            std::unique_ptr<MDLSymbols> symb_gen;
            AstAnalyzer ast_analyzer;
            std::shared_ptr<Logger> logger;
            MDLSymbolNamer symbol_namer;

            // Global state variables (to link model together)
            std::unordered_set<std::string> data_object_names;
            std::vector<std::string> structural_parameters;
            std::vector<std::string> variability_parameters;
            std::string selected_data_object;
            std::vector<std::string> designDeclaredVariables;       // List of all declared variables from the various design parts

            std::string value;
            void setValue(std::string str);

            std::vector<std::string> values;
            void setValue(std::vector<std::string> str);

            stringpair hvalue;
            void setValue(stringpair pair);

            stringmap hvalues;
            void setValue(stringmap hash);

            std::string accept(AstNode *);
    };
}

#endif
