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

#ifndef CPHARMML_POPULATIONPARAMETERS_H_
#define CPHARMML_POPULATIONPARAMETERS_H_

#include <string>
#include <unordered_set>
#include <vector>
#include <symbols/PopulationParameter.h>
#include <symbols/RandomVariable.h>
#include <symbols/IndividualParameter.h>
#include <PharmML/ModellingSteps.h>
#include <PharmML/Correlation.h>

namespace CPharmML
{
    class PopulationParameter
    {
        public:
            // Construct with PopulationParameter/Correlation as base
            PopulationParameter(PharmML::PopulationParameter *populationParameter);
            PopulationParameter(PharmML::Correlation *correlation);
            
            // Add PharmML objects for consolidation with PopulationParameter
            void addRandomVariable(PharmML::RandomVariable *randomVariable);
            void addIndividualParameter(PharmML::IndividualParameter *individualParameter);
            void addParameterEstimation(PharmML::ParameterEstimation *parameterEstimation);
            
            // Get PharmML objects used to consolidate
            PharmML::PopulationParameter *getPopulationParameter();
            std::vector<PharmML::RandomVariable *> getRandomVariables();
            std::vector<PharmML::IndividualParameter *> getIndividualParameters();
            PharmML::ParameterEstimation *getParameterEstimation();
            PharmML::Correlation *getCorrelation();
            
            // Add attributes
            void addDistributionName(std::string name);
            void addDistributionParameterType(std::string name);
            void setName(std::string name);
            
            // Get attributes
            bool isVariabilityParameter();
            bool isCorrelation();
            std::string getDistributionName();
            std::string getDistributionParameterType();
            bool inDifferentParameterizations();
            std::string getName();
        
        private:
            // PharmML objects used to consolidate
            PharmML::PopulationParameter *populationParameter = nullptr;
            PharmML::Correlation *correlation = nullptr;
            std::vector<PharmML::RandomVariable *> randomVariables;
            std::vector<PharmML::IndividualParameter *> individualParameters;
            PharmML::ParameterEstimation *parameterEstimation = nullptr;
            
            bool variabilityParameter = false;
            bool correlationType = false;
            std::unordered_set<std::string> distNames;
            std::unordered_set<std::string> distParTypes;
            std::string name; // For Correlations lacking PharmML naming
    };
    
    class PopulationParameters
    {
        public:
            PopulationParameters(std::vector<PharmML::PopulationParameter *> populationParameters, std::vector<PharmML::Correlation *> correlations);
            void addRandomVariables(std::vector<PharmML::RandomVariable *> randomVariables);
            void addIndividualParameters(std::vector<PharmML::IndividualParameter *> individualParameters);
            void addEstimationStep(PharmML::EstimationStep *estimationStep);
            
            std::vector<PopulationParameter *> getPopulationParameters();
            std::vector<PopulationParameter *> getPopulationParameters(std::vector<PharmML::Correlation *> correlations);
        
        private:
            std::vector<PopulationParameter *> populationParameters;
    };
}

#endif
