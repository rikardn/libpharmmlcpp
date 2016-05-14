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
#include <iostream>

namespace CPharmML
{
    Consolidator::Consolidator(PharmML::PharmMLContext *context, PharmML::Model *model) {
        this->context = context;
        
        // First, consolidate all symbols (other consolidators might require it)
        this->consolidateSymbols(model);
        
        // Consolidate the different aspects of the model
        this->consolidatePopulationParameters(model);
        this->consolidateCovariates(model);
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
        
        std::vector<PharmML::Covariate *> covs = model->getModelDefinition()->getCovariateModel()->getCovariates();
        for (PharmML::Covariate *cov : covs) {
            this->allSymbols.addSymbol(cov);
        }

        this->allSymbols.addSymbol(model->getIndependentVariable());

        // Obtain a map from all symbIds to Symbols. Will be used to populate SymbRefs
        std::unordered_map<std::string, PharmML::Symbol *> symbIdMap;
        for (PharmML::Symbol *symbol : this->allSymbols) {
            symbIdMap[symbol->getSymbId()] = symbol;
        }

        // Ask symbols to set all SymbRefs to point to Symbols and to update the referencedSymbols (also in Referer children)
        for (PharmML::Symbol *symbol : this->allSymbols) {
            symbol->gatherSymbRefs(symbIdMap);
        }
        
        // Ask non-symbols to set all SymbRefs to point to Symbols and to update the referencedSymbols in Referer children
        model->getModellingSteps()->gatherSymbRefs(symbIdMap);
        model->getModelDefinition()->gatherSymbRefs(symbIdMap);
        model->getTrialDesign()->gatherSymbRefs(symbIdMap);
        
        // FIXME: Hackish, Observation should probably be a Symbol. The PharmML structure is a bit entagled need more time to think about this...
        PharmML::SymbRef *observation = model->getModelDefinition()->getObservationModel()->getOutput();
        observation->setSymbol(symbIdMap[observation->toString()]);
    }
    
    void Consolidator::consolidatePopulationParameters(PharmML::Model *model) {
        // Consolidate PharmML PopulationParameter's (e.g. for init statement generation)
        for (PharmML::PopulationParameter *pop_param : model->getModelDefinition()->getParameterModel()->getPopulationParameters()) {
            // Create new consolidated population parameter
            CPharmML::PopulationParameter *cpop_param = new PopulationParameter(pop_param);
            
            // Find RandomVariable's referencing this PopulationParameter
            std::vector<PharmML::RandomVariable *> random_vars = model->getModelDefinition()->getParameterModel()->getRandomVariables();
            for (PharmML::RandomVariable *random_var : random_vars) {
                bool depends_on_pop = random_var->referencedSymbols.hasSymbol(pop_param);
                if (depends_on_pop) {
                    cpop_param->addRandomVariable(random_var);
                    
                    // Get distribution for variable
                    PharmML::Distribution *dist = random_var->getDistribution();
                    cpop_param->addDistributionName(dist->getName());
                    
                    // Find DistributionParameter's referencing this PopulationParameter
                    for (PharmML::DistributionParameter *dist_param: dist->getDistributionParameters()) {
                        bool depends_on_pop = dist_param->refersIndirectlyTo(pop_param); // Try out the new Referer system
                        if (depends_on_pop) {
                            cpop_param->addDistributionParameterType(dist_param->getName());
                            // TODO: Transformation support (AstNode containing SymbRef to PopulationParameter should at least know if it's simple)
                        }
                    }
                }
            }
            
            // Find IndividualParameter's refering this PopulationParameter
            std::vector<PharmML::IndividualParameter *> indiv_params = model->getModelDefinition()->getParameterModel()->getIndividualParameters();
            for (PharmML::IndividualParameter *indiv_param : indiv_params) {
                bool depends_on_pop = indiv_param->referencedSymbols.hasSymbol(pop_param);
                if (depends_on_pop) {
                    cpop_param->addIndividualParameter(indiv_param);
                }
            }
            
            // Find ParameterEstimation for this PopulationParameter
            // TODO: Figure out how to deal with multiple EstimationStep's
            PharmML::EstimationStep *est_step1 = model->getModellingSteps()->getEstimationSteps()[0];
            std::vector<PharmML::ParameterEstimation *> est_params = est_step1->getParameters();
            for (PharmML::ParameterEstimation *est_param : est_params) {
                bool depends_on_pop = est_param->refersIndirectlyTo(pop_param);
                if (depends_on_pop) {
                    cpop_param->addParameterEstimation(est_param);
                    break;
                }
            }
            
            // Add the finished consolidated PopulationParameter object
            this->populationParameters.push_back(cpop_param);
        }
        
        // Find Correlation (no associated PopulationParameter however)
        std::vector<PharmML::Correlation *> corrs = model->getModelDefinition()->getParameterModel()->getCorrelations();
        for(auto it = corrs.begin(); it != corrs.end(); ++it) {
            CPharmML::PopulationParameter *cpop_param = new PopulationParameter(*it);
            std::string name = "OMEGA_" + std::to_string(it - corrs.begin() + 1);
            cpop_param->setName(name);
            this->populationParameters.push_back(cpop_param);
        }
    }
    
    void Consolidator::consolidateCovariates(PharmML::Model *model) {
        PharmML::CovariateModel *cov_model = model->getModelDefinition()->getCovariateModel();
        if (cov_model) {
            std::vector<PharmML::Covariate *> covs = cov_model->getCovariates();
            for (PharmML::Covariate *cov : covs) {
                this->covariatesCollection.addCovariate(cov);
            }
        }
        
        // FIXME: getExternalDatasets(); This is a common pattern. Pluralness MUST be handled everywhere!
        // (In this case, it's the reference to the oid of the ExternalDataset from EstimationStep that decides)
        std::vector<PharmML::ExternalDataset *> ext_datasets = model->getTrialDesign()->getExternalDatasets();
        if (!ext_datasets.empty()) {
            PharmML::ExternalDataset *first_ext_dataset = ext_datasets[0];
            std::vector<PharmML::ColumnMapping *> col_maps = first_ext_dataset->getColumnMappings();
            for (PharmML::ColumnMapping *col_map : col_maps) {
                this->covariatesCollection.addColumnMapping(col_map);
            }
        }
        
        // FIXME: Same as above (what if several ExternalDataset's?)
        if (!ext_datasets.empty()) {
            PharmML::ExternalDataset *first_ext_dataset = ext_datasets[0];
            std::vector<PharmML::ColumnDefinition *> col_defs = first_ext_dataset->getDataset()->getDefinition()->getColumnDefinitions();
            for (PharmML::ColumnDefinition *col_def : col_defs) {
                if (col_def->getType() == "covariate") {
                    this->covariatesCollection.addColumnDefinition(col_def);
                }
            }
        }
    }

    std::vector<CPharmML::PopulationParameter *> Consolidator::getPopulationParameters() {
        return this->populationParameters;
    }
}
