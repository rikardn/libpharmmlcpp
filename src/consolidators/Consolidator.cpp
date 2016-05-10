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
            
            // Find IndividualParameter's employing this PopulationParameter
            std::vector<PharmML::IndividualParameter *> individualParameters = model->getModelDefinition()->getParameterModel()->getIndividualParameters();
            std::vector<PharmML::IndividualParameter *> linkingIndividualParameters;
            for (PharmML::IndividualParameter *param : individualParameters) {
                PharmML::AstNode *node;
                if (param->isStructured()) {
                    node = param->getPopulationValue();
                } else {
                    node = param->getAssignment();
                }
                bool links = false;
                std::vector<PharmML::SymbRef *> symbRefs;
                // TODO: Find a way to get hold on all PharmML::SymbRef's in the tree (dependencies maybe?)
                for (PharmML::SymbRef *symbRef : symbRefs) {
                    PharmML::Symbol *resolvedSymbol = this->context->resolveSymbref(symbRef);
                    if (param == resolvedSymbol) {
                        links = true;
                    }
                }
                if (links) {
                    linkingIndividualParameters.push_back(param);
                }
            }
            
            // Create the consolidated PopulationParameter object and add it to consolidator
            PopulationParameter *cPopulationParameter = new PopulationParameter(populationParameter, randomVariables);
            this->populationParameters.push_back(cPopulationParameter);
        }
    }
}
