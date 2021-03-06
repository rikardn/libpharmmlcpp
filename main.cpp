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

#include <iostream>
#include <exception>
#include <PharmML/Model.h>
#include <AST/Scalar.h>
#include <generators/R/RAstGenerator.h>
#include <generators/R/RPharmMLGenerator.h>
#include <symbols/Variable.h>
#include <symbols/PopulationParameter.h>

using namespace PharmML;

int main(int argc, char **argv)
{
    const char *filename;
    if (argc < 2) {
        filename = "Executable_Simeoni_2004_oncology_TGI.xml";
        // filename = "Executable_Simeoni_2004_oncology_TGI_trialdesign.xml";
    } else {
        filename = argv[1];
    }

    Model *model;
    try {
        model = new Model(filename);
    } 
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 5;
    }


    RPharmMLGenerator gen;
    
    // Parameter definitions output
    std::cout << "# Parameter definitions" << std::endl;
    model->getIndependentVariable()->accept(&gen);
    std::cout << gen.getValue() << std::endl;
    std::cout << "PopulationParameters <- c(";
    bool first = true;
    for (PopulationParameter *p : model->getModelDefinition()->getParameterModel()->getPopulationParameters()) {
        if (first) {
            first = false;
        } else {
            std::cout << ", ";
        }
        p->accept(&gen);
        std::cout << gen.getValue(); 
    }
    std::cout << ")" << std::endl;

    for (RandomVariable *r : model->getModelDefinition()->getParameterModel()->getRandomVariables()) {
        r->accept(&gen);
        std::cout << gen.getValue() << std::endl;
    }
    std::cout << std::endl;
    
    // Variability levels output
    std::vector<VariabilityModel *> var_mods = model->getModelDefinition()->getVariabilityModels();
    for (auto &it : var_mods) {
        std::cout << "# Variability model:" << std::endl;
        if (it->onResidualError()) {
            std::cout << "# Residual error" << std::endl;
        } else if (it->onParameter()){
            std::cout << "# Parameter variability" << std::endl;
        }
        for (VariabilityLevel *var_level : it->getVariabilityLevels()) {
            var_level->accept(&gen);
            std::cout << gen.getValue();
        }
        std::cout << std::endl;
    }
    
    // Function definitions output
    std::cout << "# Function definitions" << std::endl;
    std::vector<std::string> function_defs = gen.genFunctionDefinitions(model);
    for (std::string function_def : function_defs) {
        std::cout << function_def << std::endl;
    }
    
    // Covariates output
    CovariateModel *cov_mod = model->getModelDefinition()->getCovariateModel();
    if (cov_mod) {
        std::cout << "# Covariates" << std::endl;
        for (Covariate *cov : cov_mod->getCovariates()) {
            cov->accept(&gen);
            std::cout << gen.getValue() << std::endl;
        }
        std::cout << std::endl;
    }
    
    // Individual parameters output
    std::cout << "# Individual parameters" << std::endl;
    for (IndividualParameter *ind : model->getModelDefinition()->getParameterModel()->getIndividualParameters()) {
        ind->accept(&gen);
        std::cout << gen.getValue() << std::endl;
    }
    std::cout << std::endl;
    
    // Structural model output
    std::cout << "# Structural model" << std::endl;
    // Separate dependency resolution for variables. Should be moved to other place.
    std::vector<CommonVariable *> vars = model->getModelDefinition()->getStructuralModel()->getVariables();
    std::vector<CommonVariable *> ordered;
    ordered.push_back(vars[0]);
    bool inserted;
    for (int i = 1; i < vars.size(); i++) {
        inserted = false;
        for (auto j = ordered.begin(); j < ordered.end(); j++) {
            if (ordered[j - ordered.begin()]->getDependencies().hasDependency(vars[i]->getSymbId())) {
                ordered.insert(j, vars[i]);
                inserted = true;
                break;
            }
        } 
        if (!inserted) {
            ordered.push_back(vars[i]);
        }
    }
    for (CommonVariable *v : ordered) {
        v->accept(&gen);
        std::cout << gen.getValue() << std::endl;
    }
    std::cout << std::endl;
    
    // Observation model output
    std::cout << "# Observation model" << std::endl;
    model->getModelDefinition()->getObservationModel()->accept(&gen); 
    std::cout << gen.getValue() << std::endl;
    std::cout << std::endl;
    
    // TRIAL DESIGN output
    TrialDesign *td = model->getTrialDesign();
    if (td) {
        std::cout << "# [TRIAL DESIGN]" << std::endl;
        
        // External datasets
        std::vector<ExternalDataset *> ext_dss = td->getExternalDatasets();
        for (ExternalDataset *ext_ds : ext_dss) {
            std::cout << "## External dataset" << std::endl;
            ext_ds->accept(&gen);
            std::cout << gen.getValue() << std::endl;
        }
        
        // Interventions output
        Interventions *interventions = td->getInterventions();
        if (interventions) {
            std::cout << "## Interventions" << std::endl;
            interventions->accept(&gen);
            std::cout << gen.getValue() << std::endl;
        }
        
        // Observations output
        Observations *obs = td->getObservations();
        if (obs) {
            std::cout << "## Observations" << std::endl;
            obs->accept(&gen);
            std::cout << gen.getValue() << std::endl;
        }
        
        // Arms output
        Arms *arms = td->getArms();
        if (arms) {
            std::cout << "## Arms" << std::endl;
            arms->accept(&gen);
            std::cout << gen.getValue() << std::endl;
        }
        
        // DesignSpaces output
        DesignSpaces *ds = td->getDesignSpaces();
        if (ds) {
            std::cout << "## Design spaces" << std::endl;
            ds->accept(&gen);
            std::cout << gen.getValue() << std::endl;
        }
    }

    return 0;
}
