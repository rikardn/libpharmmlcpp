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

#ifndef PHARMML_MDLPHARMMLGENERATOR_H_
#define PHARMML_MDLPHARMMLGENERATOR_H_

#include <string>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
typedef std::pair<std::string, std::string> stringpair;
typedef std::unordered_map<std::string, std::string> stringmap;

#include <visitors/AstNodeVisitor.h>
#include <PharmML/FunctionDefinition.h>
#include <symbols/PopulationParameter.h>
#include <symbols/IndividualParameter.h>
#include <symbols/Covariate.h>
#include <symbols/Variable.h>
#include <symbols/DerivativeVariable.h>
#include <PharmML/ObservationModel.h>
#include <symbols/IndependentVariable.h>
#include <symbols/RandomVariable.h>
#include <PharmML/Distribution.h>
#include <PharmML/ColumnMapping.h>
#include <PharmML/Dataset.h>
#include <PharmML/ModellingSteps.h>
#include "MDLAstGenerator.h" // TODO: Diverge from RAstGenerator (now a copy)
#include <PharmML/Model.h>
#include <generators/TextFormatter.h>

#include <consolidators/Consolidator.h>
#include <consolidators/PopulationParameters.h>

namespace PharmML
{
    class MDLGenerator : public PharmMLVisitor
    {
        private:
            MDLAstGenerator ast_gen;
            std::vector<std::string> structuralParameterNames;
            std::vector<std::string> variabilityParameterNames;
            
            std::string value;
            void setValue(std::string str);
            
            std::vector<std::string> values;
            void setValue(std::vector<std::string> str);
            
            stringpair hvalue;
            void setValue(stringpair pair);
            
            stringmap hvalues;
            void setValue(stringmap hash);
            
            std::string accept(AstNode *);

        public:
            std::string getValue();
            std::vector<std::string> getValues();
            stringpair getPairValue();
            stringmap getHashValue();
            
            std::string generateModel(Model *model);
            
            std::string genDataObj(ExternalDataset *ext_ds);
            std::string genDataInputVariablesBlock(Dataset *node, stringmap &column_mappings);
            
            std::string genParObj(std::vector<CPharmML::PopulationParameter *> populationParameters);
            std::string genStructuralBlock(std::vector<CPharmML::PopulationParameter *> structuralParameters);
            std::string genVariabilityBlock(std::vector<CPharmML::PopulationParameter *> variabilityParameters);
            std::string genRandomVariableDefinitionBlock(std::vector<PharmML::RandomVariable *> random_vars, PharmML::VariabilityLevel *level);
            
            std::string genMdlObj(PharmML::Model *model);
            std::string genTaskObj();
            std::string genMogObj(std::string dataObj, std::string parObj, std::string mdlObj, std::string taskObj);
            
            virtual void visit(FunctionDefinition *node);
            virtual void visit(PopulationParameter *node);
            virtual void visit(IndividualParameter *node);
            virtual void visit(RandomVariable *node);
            virtual void visit(VariabilityLevel *node);
            virtual void visit(Correlation *node);
            virtual void visit(Covariate *node);
            virtual void visit(IndependentVariable *node);
            virtual void visit(Variable *node);
            virtual void visit(DerivativeVariable *node);
            virtual void visit(ObservationModel *node);
            virtual void visit(Distribution *node);
            virtual void visit(ColumnMapping *node);
            
            virtual void visit(ExternalFile *node);
            virtual void visit(DataColumn *node);
            virtual void visit(Dataset *node);
            
            virtual void visit(ExternalDataset *node);
            
            virtual void visit(Interventions *node);
            virtual void visit(Administration *node);
            virtual void visit(IndividualAdministration *node);
            
            virtual void visit(Observations *node);
            virtual void visit(Observation *node);
            virtual void visit(IndividualObservations *node);
            virtual void visit(ObservationCombination *node);
            
            virtual void visit(Arms *node);
            virtual void visit(Arm *node);
            virtual void visit(InterventionSequence *node);
            virtual void visit(ObservationSequence *node);
            virtual void visit(OccasionSequence *node);
            
            virtual void visit(DesignSpaces *node);
            virtual void visit(DesignSpace *node);
            
            virtual void visit(ParameterEstimation *node);
    };
}

#endif
