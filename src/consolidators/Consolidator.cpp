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
        
        // Consolidate the different aspects of the model
        this->consolidatePopulationParameters(model);
        this->consolidateSymbols(model);
    }
    
    void Consolidator::consolidatePopulationParameters(PharmML::Model *model) {
        // Consolidate PharmML PopulationParameter's (e.g. for init statement generation)
        for (PharmML::PopulationParameter *pop_param : model->getModelDefinition()->getParameterModel()->getPopulationParameters()) {
            // Create new consolidated population parameter
            CPharmML::PopulationParameter *cpop_param = new PopulationParameter(pop_param);
            
            // Find RandomVariable's refering this PopulationParameter
            std::vector<PharmML::RandomVariable *> random_vars = model->getModelDefinition()->getParameterModel()->getRandomVariables();
            for (PharmML::RandomVariable *random_var : random_vars) {
                // Get distribution for variable
                PharmML::Distribution *dist = random_var->getDistribution();
                
                // Search for reference amongst all DistributionParameter's
                for (PharmML::DistributionParameter *dist_param: dist->getDistributionParameters()) {
                    std::unordered_set<PharmML::SymbRef *> refs = dist_param->getDependencies().getSymbRefs();
                    for (PharmML::SymbRef *ref : refs) {
                        PharmML::Symbol *symbol = this->context->resolveSymbref(ref);
                        if (pop_param == symbol) {
                            cpop_param->addRandomVariable(random_var);
                            cpop_param->addDistributionName(dist->getName());
                            cpop_param->addDistributionParameterType(dist_param->getName());
                            // TODO: Transformation support (AstNode containing SymbRef to PopulationParameter should at least know if it's simple)
                        }
                    }
                }
            }
            
            // Find IndividualParameter's refering this PopulationParameter
            std::vector<PharmML::IndividualParameter *> indiv_params = model->getModelDefinition()->getParameterModel()->getIndividualParameters();
            for (PharmML::IndividualParameter *indiv_param : indiv_params) {
                std::unordered_set<PharmML::SymbRef *> refs = indiv_param->getDependencies().getSymbRefs();
                for (PharmML::SymbRef *ref : refs) {
                    PharmML::Symbol *symbol = this->context->resolveSymbref(ref);
                    if (pop_param == symbol) {
                        // Add the individual parameter found
                        cpop_param->addIndividualParameter(indiv_param);
                    }
                }
            }
            
            // Find ParameterEstimation for this PopulationParameter
            // TODO: Figure out how to deal with multiple EstimationStep's
            PharmML::EstimationStep *est_step1 = model->getModellingSteps()->getEstimationSteps()[0];
            std::vector<PharmML::ParameterEstimation *> est_params = est_step1->getParameters();
            for (PharmML::ParameterEstimation *est_param : est_params) {
                PharmML::SymbRef *ref = est_param->getSymbRef();
                PharmML::Symbol *symbol = this->context->resolveSymbref(ref);
                if (pop_param == symbol) {
                    // Add the estimation parameter found
                    cpop_param->addParameterEstimation(est_param);
                    break;
                }
            }
            
            // Add the finished consolidated PopulationParameter object
            this->populationParameters.push_back(cpop_param);
        }
    }
  
    // Build the allSymbols set. Set all SymbRef to point to Symbols. Set all referencedSymbols for Symbols 
    void Consolidator::consolidateSymbols(PharmML::Model *model) {
        std::vector<PharmML::PopulationParameter *> params = model->getModelDefinition()->getParameterModel()->getPopulationParameters();
        for (PharmML::PopulationParameter *param : params) {
            this->allSymbols.addSymbol(param);
        }

        std::vector<PharmML::IndividualParameter *> ips = model->getModelDefinition()->getParameterModel()->getIndividualParameters();
        for (PharmML::IndividualParameter *ip : ips) {
            this->allSymbols.addSymbol(ip);
        }

        std::vector<PharmML::RandomVariable *> random = model->getModelDefinition()->getParameterModel()->getRandomVariables();
        for (PharmML::RandomVariable *rv : random) {
            this->allSymbols.addSymbol(rv);
        }

        std::vector<PharmML::CommonVariable *> cvs = model->getModelDefinition()->getStructuralModel()->getVariables();
        for (PharmML::CommonVariable *cv : cvs) {
            this->allSymbols.addSymbol(cv);
        }

        this->allSymbols.addSymbol(model->getIndependentVariable());

        // Obtain a map from all symbIds to Symbols. Will be used to populate SymbRefs
        std::unordered_map<std::string, PharmML::Symbol *> symbIdMap;
        for (PharmML::Symbol *symbol : this->allSymbols) {
            symbIdMap[symbol->getSymbId()] = symbol;
        }

        // Ask symbols to set all SymbRefs to point to Symbols and to update the referencedSymbols
        for (PharmML::Symbol *symbol : this->allSymbols) {
            symbol->gatherSymbRefs(symbIdMap);
        }
    }

    std::vector<CPharmML::PopulationParameter *> Consolidator::getPopulationParameters() {
        return this->populationParameters;
    }
}
