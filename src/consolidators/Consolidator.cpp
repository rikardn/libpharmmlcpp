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
        this->logger = std::make_shared<PharmML::Logger>("Post");
        
        // First, consolidate all symbols (other consolidators might require it)
        this->consolidateSymbols(model);

        // Consolidate all objects (elements having the "oid" attribute)
        this->consolidateObjects(model);
        
        // Consolidate the different aspects of the model
        this->consolidatePopulationParameters(model);
        this->consolidateCovariates(model);
        this->consolidateVariabilityModels(model);
        this->consolidateFunctions(model);
        this->consolidatePKMacros(model);
        
        this->consolidateTrialDesign(model);
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
    }
   
    void Consolidator::consolidateObjects(PharmML::Model *model) {
        std::unordered_set<std::string> allOids;

        PharmML::TrialDesign *td = model->getTrialDesign();

        if (td) {
            PharmML::Arms *arms = td->getArms();
            if (arms) {
                for (PharmML::Arm *arm : arms->getArms()) {
                    // Check if oid already exists
                    if (allOids.count(arm->getOid()) == 1) {
                        this->duplicateOidError(arm->getOid(), arm);
                    }
                    allOids.insert(arm->getOid());
                    this->allObjects.insert(arm);
                }
            }
            PharmML::Observations *observations = td->getObservations();
            if (observations) {
                for (PharmML::IndividualObservations *observation : observations->getIndividualObservations()) {
                    if (allOids.count(observation->getOid()) == 1) {
                        this->duplicateOidError(observation->getOid(), observation);
                    }
                    allOids.insert(observation->getOid());
                    this->allObjects.insert(observation);
                }
            }
            PharmML::Interventions *interventions = td->getInterventions();
            if (interventions) {
                for (PharmML::Administration *admin : interventions->getAdministrations()) {
                    if (allOids.count(admin->getOid()) == 1) {
                        this->duplicateOidError(admin->getOid(), admin);
                    }
                    allOids.insert(admin->getOid());
                    this->allObjects.insert(admin);
                }
            }
        }

        // Obtain a map from all oids to Objects. Will be used to populate ObjectRefs
        std::unordered_map<std::string, PharmML::Object *> oidMap;
        for (PharmML::Object *object : this->allObjects) {
            oidMap[object->getOid()] = object;
        }

        for (PharmML::Object *object : this->allObjects) {
            object->gatherObjectRefs(oidMap);
        }

        // Populate ObjectReferer ObjectRefs
        if (td) {
            PharmML::Interventions *interventions = td->getInterventions();
            if (interventions) {
                for (PharmML::IndividualAdministration *ia : interventions->getIndividualAdministrations()) {
                    ia->gatherObjectRefs(oidMap);
                }
            }
        }
    }

    // Print an error for duplicate oid
    void Consolidator::duplicateOidError(const std::string &oid, PharmML::PharmMLSection *section) {
        this->logger->error("Duplicate oid '" + oid + "'", section);
    }

    void Consolidator::consolidatePopulationParameters(PharmML::Model *model) {
        // Consolidate PharmML PopulationParameter's and Correlation's into a wrapping object (with convenience functions)
        std::vector<PharmML::PopulationParameter *> pop_params = model->getModelDefinition()->getParameterModel()->getPopulationParameters();
        std::vector<PharmML::Correlation *> corrs = model->getModelDefinition()->getParameterModel()->getCorrelations();
        CPharmML::PopulationParameters *cpop_params = new PopulationParameters(pop_params, corrs);
        
        // Merge in information from PharmML RandomVariable's, IndividualParameter's and EstimationStep's
        std::vector<PharmML::RandomVariable *> rand_vars = model->getModelDefinition()->getParameterModel()->getRandomVariables();
        std::vector<PharmML::IndividualParameter *> ind_params = model->getModelDefinition()->getParameterModel()->getIndividualParameters();
        std::vector<PharmML::EstimationStep *> est_steps = model->getModellingSteps()->getEstimationSteps();
        cpop_params->addRandomVariables(rand_vars);
        cpop_params->addIndividualParameters(ind_params);
        if (est_steps.size() > 0) {     // No estimation steps
            cpop_params->addEstimationStep(est_steps[0]); // TODO: Plurality support!
        }
        
        // TODO: Add plurality support for multiple parameter models
        this->populationParameters.push_back(cpop_params);
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
        // Add Correlation's (consolidated, because they know their generated name)
        std::vector<PharmML::Correlation *> corrs = model->getModelDefinition()->getParameterModel()->getCorrelations();
        for (PharmML::Correlation *corr : corrs) {
            this->variabilityModels->addCorrelation(corr);
        }
    }
    
    void Consolidator::consolidateFunctions(PharmML::Model *model) {
        this->functions = new Functions();
        std::vector<PharmML::FunctionDefinition *> funs = model->getFunctionDefinitions();
        for (PharmML::FunctionDefinition *fun : funs) {
            this->functions->addFunctionDefinition(fun);
        }
    }
    
    void Consolidator::consolidatePKMacros(PharmML::Model *model) {
        std::vector<PharmML::PKMacro *> pk_macros = model->getModelDefinition()->getStructuralModel()->getPKMacros();
        this->pk_macros = new CPharmML::PKMacros(pk_macros, this->logger);
        this->pk_macros->validate();
    }

    void Consolidator::consolidateTrialDesign(PharmML::Model *model) {
        PharmML::TrialDesign *td = model->getTrialDesign();

        if (td) {   // This seems like a repetition of code from consolidateObject but it is probably best to keep separate
            PharmML::Observations *obs = td->getObservations();
            if (obs) {
                // Check that all individual observations has an independent variable
                std::vector<PharmML::IndividualObservations *> ind_obs_vector = obs->getIndividualObservations();
                for (PharmML::IndividualObservations *ind_obs : ind_obs_vector) {
                    PharmML::Dataset *ds = ind_obs->getDataset();
                    PharmML::DataColumn *col = ds->getIdvColumn();
                    if (!col) {     // No idv column was found
                        this->logger->error("Missing idv column in IndividualObservations", ind_obs);
                        return;     // FIXME: Should we really return here?
                    }
                    // FIXME: Need to check ColumnMapping and IndependentVariables also
                    // FIXME: What happens if there is no Column definitions and/or ColumnMapping. Error checking is hard!
                }
            }
            PharmML::Interventions *ints = td->getInterventions();
            if (ints) {
                // Check that all individual administrations have an independent variable and a dose column
                std::vector<PharmML::IndividualAdministration *> ind_adms = ints->getIndividualAdministrations();
                for (PharmML::IndividualAdministration *ind_adm : ind_adms) {
                    PharmML::Dataset *ds = ind_adm->getDataset();
                    PharmML::DataColumn *idv_col = ds->getIdvColumn();
                    if (!idv_col) {     // No idv column was found
                        this->logger->error("Missing idv column in IndividualAdministration", ind_adm);
                        return;
                    }
                    PharmML::DataColumn *dose_col = ds->getColumnFromType("dose");
                    if (!dose_col) {
                        this->logger->error("Missing dose column in IndividualAdministration", ind_adm);
                        return;
                    }
                }
            }
        }
    }
 
    CPharmML::PopulationParameters *Consolidator::getPopulationParameters() {
        // TODO: Plurality support for different parameter models
        return this->populationParameters[0];
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
