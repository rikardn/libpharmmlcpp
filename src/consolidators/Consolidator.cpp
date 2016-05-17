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
        this->consolidateVariabilityModels(model);
        this->consolidateFunctions(model);
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
        
        std::vector<PharmML::VariabilityModel *> vmods = model->getModelDefinition()->getVariabilityModels();
        for (PharmML::VariabilityModel *vmod : vmods) {
            std::vector<PharmML::VariabilityLevel *> vlevels = vmod->getVariabilityLevels();
            for (PharmML::VariabilityLevel *vlevel : vlevels) {
                this->allSymbols.addSymbol(vlevel);
            }
        }
        
        PharmML::CovariateModel *cm = model->getModelDefinition()->getCovariateModel();
        if (cm) {
            std::vector<PharmML::Covariate *> covs = cm->getCovariates();
            for (PharmML::Covariate *cov : covs) {
                this->allSymbols.addSymbol(cov);
                // Remember to add the transformations (e.g. logtWT in UC2)
                for (PharmML::Covariate *transformation : cov->getTransformations()) {
                    this->allSymbols.addSymbol(transformation);
                }
            }
        }
        
        PharmML::ObservationModel *om = model->getModelDefinition()->getObservationModel();
        if (om) {
            this->allSymbols.addSymbol(om);
        }
        
        std::vector<PharmML::FunctionDefinition *> funs = model->getFunctionDefinitions();
        for (PharmML::FunctionDefinition *fun : funs) {
            this->allSymbols.addSymbol(fun);
            for (PharmML::FunctionArgumentDefinition *arg : fun->getArguments()) {
                this->allSymbols.addSymbol(arg);
            }
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
        model->gatherSymbRefs(symbIdMap);
        
        // FIXME: Hackish, Observation should probably be a Symbol. The PharmML structure is a bit entagled need more time to think about this...
        //~ PharmML::SymbRef *observation = model->getModelDefinition()->getObservationModel()->getOutput();
        //~ observation->setSymbol(symbIdMap[observation->toString()]);
        // Now it's a Symbol as it should have been from the start!
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
        // Get ExternalDataset's (holding both ColumnMapping's and ColumnDefinition's)
        std::vector<PharmML::ColumnMapping *> col_maps;
        std::vector<PharmML::ColumnDefinition *> col_defs;
        PharmML::ExternalDataset *first_ext_dataset;
        std::vector<PharmML::ExternalDataset *> ext_datasets = model->getTrialDesign()->getExternalDatasets();
        if (!ext_datasets.empty()) {
            // FIXME: This is a common pattern. Pluralness MUST be handled everywhere!
            // (In this case, it's the reference to the oid of the ExternalDataset from EstimationStep that decides)
            first_ext_dataset = ext_datasets[0];
            
            // Get all ColumnMapping's and ColumnDefinition's (for first dataset)
            col_maps = first_ext_dataset->getColumnMappings();
            col_defs = first_ext_dataset->getDataset()->getDefinition()->getColumnDefinitions();
        }
        
        // Consolidate PharmML Covariate's (e.g. for collected output as in MDL)
        PharmML::CovariateModel *cov_model = model->getModelDefinition()->getCovariateModel();
        if (cov_model) {
            std::vector<PharmML::Covariate *> top_covs = cov_model->getCovariates();
            for (PharmML::Covariate *top_cov : top_covs) {
                // Create list of this covariate and transformed covariates contained within it
                std::vector<PharmML::Covariate *> covs = top_cov->getTransformations();
                covs.insert(covs.begin(), top_cov);
                for (PharmML::Covariate *cov : covs) {
                    // Create new consolidated covariate
                    CPharmML::Covariate *ccov = new Covariate(cov);
                    
                    // Find ColumnMapping's refering this Covariate
                    for (PharmML::ColumnMapping *col_map : col_maps) {
                        PharmML::Symbol *cov_symbol = ccov->getCovariate();
                        if (col_map->referencedSymbols.hasSymbol(cov_symbol)) {
                            ccov->addColumnMapping(col_map);
                        }
                    }
                    
                    // Find ColumnDefinition's refering earlier added ColumnMapping
                    for (PharmML::ColumnDefinition *col_def : col_defs) {
                        std::string id = ccov->getColumnId();
                        if (col_def->getId() == id) {
                            ccov->addColumnDefinition(col_def);
                        }
                    }
                    
                    // Add the finished consolidated Covariate object
                    this->covariates.push_back(ccov);
                }
            }
        }
    }
    
    void Consolidator::consolidateVariabilityModels(PharmML::Model *model) {
        this->variabilityModels = new VariabilityModels();
        // VariabilityModels assumes a maximum of one model of each type (parameter/residual error)
        std::vector<PharmML::VariabilityModel *> vmods = model->getModelDefinition()->getVariabilityModels();
        for (PharmML::VariabilityModel *vmod : vmods) {
            this->variabilityModels->addVariabilityModel(vmod);
        }
        // Add RandomVariable's
        std::vector<PharmML::RandomVariable *> rvars = model->getModelDefinition()->getParameterModel()->getRandomVariables();
        for (PharmML::RandomVariable *rvar : rvars) {
            this->variabilityModels->addRandomVariable(rvar);
        }
    }
    
    void Consolidator::consolidateFunctions(PharmML::Model *model) {
        this->functions = new Functions();
        std::vector<PharmML::FunctionDefinition *> funs = model->getFunctionDefinitions();
        for (PharmML::FunctionDefinition *fun : funs) {
            this->functions->addFunctionDefinition(fun);
        }
    }

    std::vector<CPharmML::PopulationParameter *> Consolidator::getPopulationParameters() {
        return this->populationParameters;
    }
    
    std::vector<CPharmML::Covariate *> Consolidator::getCovariates() {
        return this->covariates;
    }
    
    CPharmML::VariabilityModels *Consolidator::getVariabilityModels() {
        return this->variabilityModels;
    }
    
    CPharmML::Functions *Consolidator::getFunctions() {
        return this->functions;
    }
}
