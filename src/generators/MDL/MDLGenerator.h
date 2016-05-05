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
#include <unordered_map>

#include <visitors/AstNodeVisitor.h>
#include <PharmML/FunctionDefinition.h>
#include <symbols/PopulationParameter.h>
#include <symbols/IndividualParameter.h>
#include <PharmML/Covariate.h>
#include <symbols/Variable.h>
#include <symbols/DerivativeVariable.h>
#include <PharmML/ObservationModel.h>
#include <symbols/IndependentVariable.h>
#include <symbols/RandomVariable.h>
#include <PharmML/Distribution.h>
#include <PharmML/ColumnMapping.h>
#include <PharmML/Dataset.h>
#include <PharmML/Interventions.h>
#include <PharmML/Observations.h>
#include <PharmML/Arms.h>
#include <PharmML/DesignSpaces.h>
#include "MDLAstGenerator.h" // TODO: Diverge from RAstGenerator (now a copy). Also reason if MDLSymbols.h has a place (as PopEDSymbols.h has in PopEDAstGenerator).
#include <PharmML/Model.h>
#include <generators/R/RTextFormatter.h> // TODO: If useful for MDL: Please remove the R designation. Thank you!
//~ #include "MDLPharmMLConsolidator.h" // TODO: Will this be useful?

namespace PharmML
{
    class MDLGenerator : public PharmMLVisitor
    {
        private:
            MDLAstGenerator ast_gen;
            Model *model;
            std::string value;
            std::pair<std::string, std::string> hvalue;
            std::unordered_map<std::string, std::string> hvalues;
            void setValue(std::string str);
            void setValue(std::pair<std::string, std::string> pair);
            void setValue(std::unordered_map<std::string, std::string> hash);
            std::string accept(AstNode *);

        public:
            //~ Consolidator consol;

            std::string getValue();
            std::pair<std::string, std::string> getPairValue();
            std::unordered_map<std::string, std::string> getHashValue();
            
            std::string generateModel(Model *model);
            
            std::string genDataObj(ExternalDataset *ext_ds);
            std::string genParObj();
            std::string genMdlObj();
            std::string genTaskObj();
            std::string genMogObj();
            
            virtual void visit(FunctionDefinition *node);
            virtual void visit(PopulationParameter *node);
            virtual void visit(IndividualParameter *node);
            virtual void visit(RandomVariable *node);
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
            std::string genDATA_INPUT_VARIABLES(Dataset *node, std::unordered_map<std::string, std::string> column_mappings);
            
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
    };
}

#endif
