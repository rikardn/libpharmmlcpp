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

#include <consolidators/PopulationParameters.h>

namespace CPharmML
{
    // Construct with PopulationParameter/Correlation as base
    PopulationParameter::PopulationParameter(PharmML::PopulationParameter *populationParameter) {
        this->populationParameter = populationParameter;
    }
    
    PopulationParameter::PopulationParameter(PharmML::Correlation *correlation) {
        this->correlation = correlation;
        this->correlationType = true;
    }
    
    // Add PharmML objects for consolidation with PopulationParameter
    void PopulationParameter::addRandomVariable(PharmML::RandomVariable *randomVariable) {
        this->randomVariables.push_back(randomVariable);
        this->variabilityParameter = true; // Are we sure? What if used as 'mean' in 'Normal1' for example?
    }
    
    void PopulationParameter::addIndividualParameter(PharmML::IndividualParameter *individualParameter) {
        this->individualParameters.push_back(individualParameter);
    }
    
    void PopulationParameter::addParameterEstimation(PharmML::ParameterEstimation *parameterEstimation) {
        this->parameterEstimation = parameterEstimation;
    }
    
    // Get PharmML objects used to consolidate
    PharmML::PopulationParameter *PopulationParameter::getPopulationParameter() {
        return this->populationParameter;
    }
    
    std::vector<PharmML::RandomVariable *> PopulationParameter::getRandomVariables() {
        return this->randomVariables;
    }
    
    std::vector<PharmML::IndividualParameter *> PopulationParameter::getIndividualParameters() {
        return this->individualParameters;
    }
    
    PharmML::Correlation *PopulationParameter::getCorrelation() {
        return this->correlation;
    }
    
    std::string PopulationParameter::getName() {
        return this->name;
    }
    
    // Set attributes
    void PopulationParameter::addDistributionName(std::string name) {
        this->distNames.insert(name);
    }
    
    void PopulationParameter::addDistributionParameterType(std::string name) {
        this->distParTypes.insert(name);
    }
    
    void PopulationParameter::setName(std::string name) {
        this->name = name;
    }
    
    // Get attributes
    PharmML::ParameterEstimation *PopulationParameter::getParameterEstimation() {
        return this->parameterEstimation;
    }
    
    bool PopulationParameter::isVariabilityParameter() {
        return (this->variabilityParameter);
    }
    
    bool PopulationParameter::isCorrelation() {
        return (this->correlationType);
    }
    
    std::string PopulationParameter::getDistributionName() {
        std::vector<std::string> vector(this->distNames.begin(), this->distNames.end());
        if (vector.size() == 1) {
            return vector[0];
        } else {
            return std::string("");
        }
    }
    
    std::string PopulationParameter::getDistributionParameterType() {
        std::vector<std::string> vector(this->distParTypes.begin(), this->distParTypes.end());
        if (vector.size() == 1) {
            return vector[0];
        } else {
            return std::string("");
        }
    }
    
    bool PopulationParameter::inDifferentParameterizations() {
        if (this->distNames.size() > 1 || this->distParTypes.size() > 1) {
            return true;
        } else {
            return false;
        }
    }
}
