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

#ifndef PHARMML_PHARMMLVISITOR_H_
#define PHARMML_PHARMMLVISITOR_H_

namespace PharmML
{
    class FunctionDefinition;
    class FunctionArgumentDefinition;
    
    class PopulationParameter;
    class IndividualParameter;
    class RandomVariable;
    class VariabilityLevel;
    class Correlation;
    class Variable;
    class DerivativeVariable;
    class IndependentVariable;
    class Covariate;
    class ObservationModel;
    class Distribution;
    class ColumnMapping;
    
    class ExternalFile;
    class DataColumn;
    class Dataset;
    
    class ExternalDataset;
    
    class Interventions;
    class Administration;
    class IndividualAdministration;
    
    class Observations;
    class Observation;
    class IndividualObservations;
    class ObservationCombination;
    
    class Arms;
    class Arm;
    class InterventionSequence;
    class ObservationSequence;
    class OccasionSequence;
    
    class DesignSpaces;
    class DesignSpace;
    
    class ParameterEstimation;
    
    class PKMacro;

    class PharmMLVisitor
    {
        public:
            virtual void visit(FunctionDefinition *node) = 0;
            virtual void visit(FunctionArgumentDefinition *node) = 0;
            
            virtual void visit(PopulationParameter *node) = 0;
            virtual void visit(IndividualParameter *node) = 0;
            virtual void visit(RandomVariable *node) = 0;
            virtual void visit(VariabilityLevel *node) = 0;
            virtual void visit(Correlation *node) = 0;
            virtual void visit(Covariate *node) = 0;
            virtual void visit(IndependentVariable *node) = 0;
            virtual void visit(Variable *node) = 0;
            virtual void visit(DerivativeVariable *node) = 0;
            virtual void visit(ObservationModel *node) = 0;
            virtual void visit(Distribution *node) = 0;
            virtual void visit(ColumnMapping *node) = 0;
            
            virtual void visit(ExternalFile *node) = 0;
            virtual void visit(DataColumn *node) = 0;
            virtual void visit(Dataset *node) = 0;
            
            virtual void visit(ExternalDataset *node) = 0;
            
            virtual void visit(Interventions *node) = 0;
            virtual void visit(Administration *node) = 0;
            virtual void visit(IndividualAdministration *node) = 0;
            
            virtual void visit(Observations *node) = 0;
            virtual void visit(Observation *node) = 0;
            virtual void visit(IndividualObservations *node) = 0;
            virtual void visit(ObservationCombination *node) = 0;
            
            virtual void visit(Arms *node) = 0;
            virtual void visit(Arm *node) = 0;
            virtual void visit(InterventionSequence *node) = 0;
            virtual void visit(ObservationSequence *node) = 0;
            virtual void visit(OccasionSequence *node) = 0;
            
            virtual void visit(DesignSpaces *node) = 0;
            virtual void visit(DesignSpace *node) = 0;
            
            virtual void visit(ParameterEstimation *node) = 0;
            
            virtual void visit(PKMacro *node) = 0;
    };
}

#endif
