#include <iostream>
#include "Model.h"
#include "Scalar.h"
#include "RGenerator.h"
#include "Variable.h"
#include "PopulationParameter.h"

using namespace PharmML;

int main(int argc, char **argv)
{
    const char *filename;
    if (argc < 2) {
        //~ filename = "Executable_Simeoni_2004_oncology_TGI.xml";
        filename = "Executable_Simeoni_2004_oncology_TGI_trialdesign.xml";
    } else {
        filename = argv[1];
    }
    Model *model = new Model(filename);

    RGenerator gen;
    
    // Parameter definitions output
    std::cout << "# Parameter definitions" << std::endl;
    std::cout << model->getIndependentVariable()->accept(&gen) << std::endl;
    std::cout << "PopulationParameters = c(";
    bool first = true;
    for (PopulationParameter *p : model->getModelDefinition()->getParameterModel()->getPopulationParameters()) {
        if (first) {
            first = false;
        } else {
            std::cout << ", ";
        }
        std::cout << p->accept(&gen);
    }
    std::cout << ")" << std::endl;

    for (RandomVariable *r : model->getModelDefinition()->getParameterModel()->getRandomVariables()) {
        std::cout << r->accept(&gen) << std::endl;
    }
    std::cout << std::endl;
    
    // Function definitions output
    std::cout << "# Function definitions" << std::endl;
    for (FunctionDefinition *f : model->getFunctionDefinitions()) {
        std::cout << f->accept(&gen) << std::endl;
    }
    std::cout << std::endl;
    
    // Covariates output
    CovariateModel *cov_mod = model->getModelDefinition()->getCovariateModel();
    if (cov_mod) {
        std::cout << "# Covariates" << std::endl;
        for (Covariate *cov : cov_mod->getCovariates()) {
            std::cout << cov->accept(&gen) << std::endl;
        }
        std::cout << std::endl;
    }
    
    // Individual parameters output
    std::cout << "# Individual parameters" << std::endl;
    for (IndividualParameter *ind : model->getModelDefinition()->getParameterModel()->getIndividualParameters()) {
        std::cout << ind->accept(&gen) << std::endl;
    }
    std::cout << std::endl;
    
    // Data column mapping output
    std::cout << "# Data column mappings" << std::endl;
    for (ColumnMapping *col : model->getTrialDesign()->getExternalDataset()->getColumnMappings()) {
        std::cout << col->accept(&gen) << std::endl;
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
        std::cout << v->accept(&gen) << std::endl;
    }
    std::cout << std::endl;
    
    // Observation model output
    std::cout << "# Observation model" << std::endl;
    std::cout << model->getModelDefinition()->getObservationModel()->accept(&gen) << std::endl;
    std::cout << std::endl;
    
    // Interventions output
    Intervention *intervention = model->getTrialDesign()->getIntervention();
    std::cout << "# Interventions" << std::endl;
    if (intervention) {
        std::vector<std::string> adm_oids;
        std::vector<Administration *> administrations = intervention->getAdministrations();
        for (Administration *adm : administrations) {
            std::cout << adm->accept(&gen) << std::endl;
            adm_oids.push_back(adm->getOid());
        }
        std::cout << "administration_oids = c(";
        for (std::string oid : adm_oids) {
            std::cout << "'" << oid << "'";
        }
        std::cout << ")" << std::endl;
    }
    std::cout << std::endl;
    
    // Observations output
    Observation *observation = model->getTrialDesign()->getObservation();

    return 0;
}
