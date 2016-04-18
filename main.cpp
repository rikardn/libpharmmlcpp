#include <iostream>
#include <exception>
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

    Model *model;
    try {
        model = new Model(filename);
    } 
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 5;
    }


    RGenerator gen;
    
    // Parameter definitions output
    std::cout << "# Parameter definitions" << std::endl;
    model->getIndependentVariable()->accept(&gen);
    std::cout << gen.getValue() << std::endl;
    std::cout << "PopulationParameters = c(";
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
    
    // Function definitions output
    std::cout << "# Function definitions" << std::endl;
    for (FunctionDefinition *f : model->getFunctionDefinitions()) {
        f->accept(&gen);
        std::cout << gen.getValue() << std::endl;
    }
    std::cout << std::endl;
    
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
    
    // Data column mapping output
    std::cout << "# Data column mappings" << std::endl;
    if (model->getTrialDesign()->getExternalDataset()) {
        for (ColumnMapping *col : model->getTrialDesign()->getExternalDataset()->getColumnMappings()) {
            col->accept(&gen);
            std::cout << gen.getValue() << std::endl;
        }
        std::cout << std::endl;
    }
    
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
    
    // Interventions output
    Interventions *interventions = model->getTrialDesign()->getInterventions();
    std::cout << "## Interventions" << std::endl;
    if (interventions) {
        interventions->accept(&gen);
        std::cout << gen.getValue() << std::endl;
    }
    
    // Observations output
    Observations *obs = model->getTrialDesign()->getObservations();
    std::cout << "## Observations" << std::endl;
    if (obs) {
        obs->accept(&gen);
        std::cout << gen.getValue() << std::endl;
    }
    
    // Arms output
    Arms *arms = model->getTrialDesign()->getArms();
    std::cout << "## Arms" << std::endl;
    if (arms) {
        arms->accept(&gen);
        std::cout << gen.getValue() << std::endl;
    }
    
    // DesignSpaces output
    DesignSpaces *ds = model->getTrialDesign()->getDesignSpaces();
    std::cout << "## Design spaces" << std::endl;
    if (ds) {
        ds->accept(&gen);
        std::cout << gen.getValue() << std::endl;
    }
    std::cout << "We have a serious problem here. Apparently not all objects" << std::endl;
    std::cout << "have proper oids. That didn't occur to me before. We need to" << std::endl;
    std::cout << "implement an index number as alternative when this happens" << std::endl;
    std::cout << "and that index must be resolved to e.g. an R vector whenever" << std::endl;
    std::cout << "no oid is defined. Like this maybe:" << std::endl;
    std::cout << " design_spaces[0] = list(intervention_refs = c('admin2'), observation_refs = c(), arm_refs = c()))" << std::endl;
    std::cout << " design_spaces[1] = list(intervention_refs = c(), observation_refs = c('window1'), arm_refs = c()))" << std::endl;
    std::cout << "..." << std::endl;
    std::cout << "We will probably also need it for other objects that may lack oids" << std::endl;

    model->write("out.xml");

    return 0;
}
