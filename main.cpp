#include <iostream>
#include "Model.h"
//#include "Uniop.h"
#include "Scalar.h"
#include "RGenerator.h"
#include "Variable.h"

using namespace PharmML;

int main()
{
    Model *model = new Model("UseCase2.xml");

    RGenerator gen;

    std::cout << "# Function definitions" << std::endl;
    for (FunctionDefinition *f : model->getFunctionDefinitions()) {
        std::cout << f->accept(&gen) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "# Covariates" << std::endl;
    Covariate *cov = model->getModelDefinition()->getCovariateModel()->getCovariates()[0];
    std::cout << cov->accept(&gen) << std::endl;
    std::cout << std::endl;

    std::cout << "# Structural model" << std::endl;
    for (Variable *v : model->getModelDefinition()->getStructuralModel()->getVariables()) {
        std::cout << v->accept(&gen) << std::endl;
    }

    return 0;
}
