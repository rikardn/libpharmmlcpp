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

    Covariate *cov = model->getModelDefinition()->getCovariateModel()->getCovariates()[0];
    std::cout << cov->accept(&gen) << std::endl;

    for (Variable *v : model->getModelDefinition()->getStructuralModel()->getVariables()) {
        std::cout << v->accept(&gen) << std::endl;
    }

    return 0;
}
