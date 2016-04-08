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
        filename = "UseCase2.xml";
    } else {
        filename = argv[1];
    }
    Model *model = new Model(filename);

    RGenerator gen;

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

    std::cout << "# Function definitions" << std::endl;
    for (FunctionDefinition *f : model->getFunctionDefinitions()) {
        std::cout << f->accept(&gen) << std::endl;
    }
    std::cout << std::endl;

    CovariateModel *cov_mod = model->getModelDefinition()->getCovariateModel();
    if (cov_mod) {
        std::cout << "# Covariates" << std::endl;
        for (Covariate *cov : cov_mod->getCovariates()) {
            std::cout << cov->accept(&gen) << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "# Individual parameters" << std::endl;
    for (IndividualParameter *ind : model->getModelDefinition()->getParameterModel()->getIndividualParameters()) {
        std::cout << ind->accept(&gen) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "# Structural model" << std::endl;
    for (Variable *v : model->getModelDefinition()->getStructuralModel()->getVariables()) {
        std::cout << v->accept(&gen) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "# Observation model" << std::endl;
    std::cout << model->getModelDefinition()->getObservationModel()->accept(&gen) << std::endl;
    std::cout << std::endl;

    return 0;
}
