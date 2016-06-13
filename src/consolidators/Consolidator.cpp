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
    Consolidator::Consolidator(pharmmlcpp::PharmMLContext *context, pharmmlcpp::Model *model) {
        this->context = context;
        this->logger = std::make_shared<pharmmlcpp::Logger>("Post");

        // Consolidate the different aspects of the model
        this->consolidatePopulationParameters(model);
        this->consolidateCovariates(model);
        this->consolidateVariabilityModels(model);
    }

    void Consolidator::consolidatePopulationParameters(pharmmlcpp::Model *model) {
        // Consolidate PharmML PopulationParameter's and Correlation's into a wrapping object (with convenience functions)
        if (!model->getModelDefinition()->getParameterModel()) {
            return;
        }
        std::vector<pharmmlcpp::PopulationParameter *> pop_params = model->getModelDefinition()->getParameterModel()->getPopulationParameters();
        // FIXME: This will soon be cleaned
        for (auto param : model->getModelDefinition()->getParameterModel()->getParameters()) {
            pop_params.push_back(param);
        }

        std::vector<pharmmlcpp::Correlation *> corrs = model->getModelDefinition()->getParameterModel()->getCorrelations();
        CPharmML::PopulationParameters *cpop_params = new PopulationParameters(pop_params, corrs);

        // Merge in information from PharmML RandomVariable's, IndividualParameter's and EstimationStep's
        std::vector<pharmmlcpp::RandomVariable *> rand_vars = model->getModelDefinition()->getParameterModel()->getRandomVariables();
        std::vector<pharmmlcpp::IndividualParameter *> ind_params = model->getModelDefinition()->getParameterModel()->getIndividualParameters();
        std::vector<pharmmlcpp::EstimationStep *> est_steps = model->getModellingSteps()->getEstimationSteps();
        std::vector<pharmmlcpp::OptimalDesignStep *> od_steps = model->getModellingSteps()->getOptimalDesignSteps();
        cpop_params->addRandomVariables(rand_vars);
        cpop_params->addIndividualParameters(ind_params);
        if (est_steps.size() > 0) {     // No estimation steps
            cpop_params->addEstimationStep(est_steps[0]); // TODO: Plurality support!
        }
        if (od_steps.size() > 0) {
            cpop_params->addOptimalDesignStep(od_steps[0]);
        }

        // TODO: Add plurality support for multiple parameter models
        this->populationParameters.push_back(cpop_params);
    }

    void Consolidator::consolidateCovariates(pharmmlcpp::Model *model) {
        if (!model->getModelDefinition()->getCovariateModel()) {
            return;
        }
        // Get ExternalDataset's (holding both ColumnMapping's and ColumnDefinition's)
        std::vector<pharmmlcpp::ColumnMapping *> col_maps;
        std::vector<pharmmlcpp::ColumnDefinition *> col_defs;
        pharmmlcpp::ExternalDataset *first_ext_dataset;
        std::vector<pharmmlcpp::ExternalDataset *> ext_datasets = model->getTrialDesign()->getExternalDatasets();
        if (!ext_datasets.empty()) {
            // FIXME: This is a common pattern. Pluralness MUST be handled everywhere!
            // (In this case, it's the reference to the oid of the ExternalDataset from EstimationStep that decides)
            first_ext_dataset = ext_datasets[0];

            // Get all ColumnMapping's and ColumnDefinition's (for first dataset)
            col_maps = first_ext_dataset->getColumnMappings();
            col_defs = first_ext_dataset->getDataset()->getDefinition()->getColumnDefinitions();
        }

        // Consolidate PharmML Covariate's (e.g. for collected output as in MDL)
        pharmmlcpp::CovariateModel *cov_model = model->getModelDefinition()->getCovariateModel();
        if (cov_model) {
            std::vector<pharmmlcpp::Covariate *> top_covs = cov_model->getCovariates();
            for (pharmmlcpp::Covariate *top_cov : top_covs) {
                // Create list of this covariate and transformed covariates contained within it
                std::vector<pharmmlcpp::Covariate *> covs = top_cov->getTransformations();
                covs.insert(covs.begin(), top_cov);
                for (pharmmlcpp::Covariate *cov : covs) {
                    // Create new consolidated covariate
                    CPharmML::Covariate *ccov = new Covariate(cov);

                    // Find ColumnMapping's refering this Covariate
                    for (pharmmlcpp::ColumnMapping *col_map : col_maps) {
                        pharmmlcpp::Symbol *cov_symbol = ccov->getCovariate();
                        if (col_map->referencedSymbols.hasSymbol(cov_symbol)) {
                            ccov->addColumnMapping(col_map);
                        }
                    }

                    // Find ColumnDefinition's refering earlier added ColumnMapping
                    for (pharmmlcpp::ColumnDefinition *col_def : col_defs) {
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

    void Consolidator::consolidateVariabilityModels(pharmmlcpp::Model *model) {
        if (model->getModelDefinition()->getVariabilityModels().empty()) {
            return;
        }

        this->variabilityModels = new VariabilityModels();
        // VariabilityModels assumes a maximum of one model of each type (parameter/residual error)
        std::vector<pharmmlcpp::VariabilityModel *> vmods = model->getModelDefinition()->getVariabilityModels();
        for (pharmmlcpp::VariabilityModel *vmod : vmods) {
            this->variabilityModels->addVariabilityModel(vmod);
        }
        // Add RandomVariable's
        std::vector<pharmmlcpp::RandomVariable *> rvars = model->getModelDefinition()->getParameterModel()->getRandomVariables();
        for (pharmmlcpp::RandomVariable *rvar : rvars) {
            this->variabilityModels->addRandomVariable(rvar);
        }
        // Add Correlation's (consolidated, because they know their generated name)
        std::vector<pharmmlcpp::Correlation *> corrs = model->getModelDefinition()->getParameterModel()->getCorrelations();
        for (pharmmlcpp::Correlation *corr : corrs) {
            this->variabilityModels->addCorrelation(corr);
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
}
