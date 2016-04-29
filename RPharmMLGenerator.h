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
#include "AstNodeVisitor.h"
#include "FunctionDefinition.h"
#include "PopulationParameter.h"
#include "IndividualParameter.h"
#include "Covariate.h"
#include "Variable.h"
#include "DerivativeVariable.h"
#include "ObservationModel.h"
#include "IndependentVariable.h"
#include "RandomVariable.h"
#include "Distribution.h"
#include "ColumnMapping.h"
#include "Dataset.h"
#include "Interventions.h"
#include "Observations.h"
#include "Arms.h"
#include "DesignSpaces.h"
#include "RAstGenerator.h"

namespace Consolidator
{
    class Indenter
    {
        private:
            int indentationLevel = 0;
            std::vector<std::string> rows;
            std::string getIndentation();

        public:
            void addRow(std::string str);
            void addBlock(std::string str);
            void addBlock(std::vector<std::string> strs);
            void addRowIndent(std::string str);
            void addRowOutdent(std::string str);
            void openIndent();
            void closeIndent();
            std::string createString();
    };

    class Derivatives
    {
        private:
            std::vector<std::string> x0;
            std::vector<std::string> y0;
            std::vector<std::string> y;
            std::vector<std::string> x;

        public:
            void addDerivative(std::string y, std::string x, std::string y0, std::string x0);
            std::vector<std::string> getSymbols();
            std::vector<std::string> getAssigns();
            std::string genInitVector();
    };
    
    class Variables
    {
        private:
            std::vector<std::string> symbols;
            std::vector<std::string> assigns;

        public:
            void addVariable(std::string symbol, std::string assign);
            std::string genStatements();
    };
}

namespace PharmML
{
    class RPharmMLGenerator : public PharmMLVisitor
    {
        private:
            RAstGenerator ast_gen;
            std::string value;
            void setValue(std::string str);
            // Helper function to reduce redundant code
            std::string accept(AstNode *);

        public:
            Consolidator::Derivatives derivatives;
            Consolidator::Variables variables;

            std::string getValue();
            static std::string formatVector(std::vector<std::string> vector, std::string prefix, std::string quote = "'", int pre_indent = 0);

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
