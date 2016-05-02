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

#ifndef PHARMML_POPEDGENERATOR_H_
#define PHARMML_POPEDGENERATOR_H_

#include <string>
#include "AstNodeVisitor.h"
#include "PopulationParameter.h"
#include "IndividualParameter.h"
#include "Variable.h"
#include "RAstGenerator.h"
#include "RPharmMLGenerator.h"
#include "PharmMLVisitor.h"
#include "ObservationModel.h"
#include "Model.h"

namespace PharmML
{
    class PopEDGenerator : public PharmMLVisitor
    {
        private:
            RAstGenerator ast_gen;
            RPharmMLGenerator r_gen;
            Model *model;
            std::string value;
            void setValue(std::string str);
            // Helper function to reduce redundant code
            std::string formatVector(std::vector<std::string> vector, std::string prefix, std::string quote = "'", int pre_indent = 0);
            std::string accept(AstNode *);
            int parameter_count = 1;
            std::string genParameterModel();
            std::string genODEFunc();
            std::string genStructuralModel();
            std::string genErrorFunction();
            std::string genDatabaseCall();

        public:
            std::string getValue();
            std::string generateModel(Model *model);
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
