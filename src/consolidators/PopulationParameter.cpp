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

#include <consolidators/PopulationParameter.h>

namespace CPharmML
{
    PopulationParameter::PopulationParameter(PharmML::PopulationParameter *populationParameter,
            std::unordered_set<PharmML::RandomVariable *> dependentRandomVariables,
            std::unordered_set<PharmML::IndividualParameter *> dependentIndividualParameters)
    {
        // Store the source objects
        this->populationParameter = populationParameter;
        if (!dependentRandomVariables.empty()) {
            this->type = "variability";
            this->dependentRandomVariables = dependentRandomVariables;
        } else {
            this->type = "structural";
        }
        this->dependentIndividualParameters = dependentIndividualParameters;
    }
    
    PharmML::PopulationParameter *PopulationParameter::getPopulationParameter() {
        return this->populationParameter;
    }
    
    std::unordered_set<PharmML::RandomVariable *> PopulationParameter::getDependentRandomVariables() {
        return this->dependentRandomVariables;
    }
    
    std::unordered_set<PharmML::IndividualParameter *> PopulationParameter::getDependentIndividualParameters() {
        return this->dependentIndividualParameters;
    }
    
    bool PopulationParameter::isStructuralParameter() {
        return (this->type == "structural");
    }
    
    bool PopulationParameter::isVariabilityParameter() {
        return (this->type == "variability");
    }
}
