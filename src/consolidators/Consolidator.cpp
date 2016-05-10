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

#include <consolidators/Consolidator.h>
#include <PharmML/Model.h>

namespace CPharmML
{
    Consolidator::Consolidator(PharmML::PharmMLContext *context, PharmML::Model *model) {
        this->context = context;
        // Consolidate PharmML PopulationParameter's (e.g. for init statement generation)
        for (PharmML::PopulationParameter *populationParameter : model->getModelDefinition()->getParameterModel()->getPopulationParameters()) {
            // Find RandomVariable's employing this PopulationParameter
            std::vector<PharmML::RandomVariable *> randomVariables = model->getModelDefinition()->getParameterModel()->getRandomVariables();
            std::unordered_set<PharmML::RandomVariable *> linkingRandomVariables;
            for (PharmML::RandomVariable *param : randomVariables) {
                bool links = false;
                std::unordered_set<PharmML::SymbRef *> references = param->getDependencies().getSymbRefs();
                for (PharmML::SymbRef *symbRef : references) {
                    PharmML::Symbol *resolvedSymbol = this->context->resolveSymbref(symbRef);
                    if (populationParameter == resolvedSymbol) {
                        links = true;
                    }
                }
                if (links) {
                    linkingRandomVariables.insert(param);
                }
            }
            
            // Find IndividualParameter's employing this PopulationParameter
            std::vector<PharmML::IndividualParameter *> individualParameters = model->getModelDefinition()->getParameterModel()->getIndividualParameters();
            std::unordered_set<PharmML::IndividualParameter *> linkingIndividualParameters;
            for (PharmML::IndividualParameter *param : individualParameters) {
                bool links = false;
                std::unordered_set<PharmML::SymbRef *> references = param->getDependencies().getSymbRefs();
                for (PharmML::SymbRef *symbRef : references) {
                    PharmML::Symbol *resolvedSymbol = this->context->resolveSymbref(symbRef);
                    if (populationParameter == resolvedSymbol) {
                        links = true;
                    }
                }
                if (links) {
                    linkingIndividualParameters.insert(param);
                }
            }
            
            // Create the consolidated PopulationParameter object and add it to consolidator
            CPharmML::PopulationParameter *cPopulationParameter = new PopulationParameter(populationParameter, linkingRandomVariables, linkingIndividualParameters);
            this->populationParameters.push_back(cPopulationParameter);
        }
    }
    
    std::vector<CPharmML::PopulationParameter *> Consolidator::getPopulationParameters() {
        return this->populationParameters;
    }
}
