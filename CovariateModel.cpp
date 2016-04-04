#include <vector>
#include <iostream>

#include "CovariateModel.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace PharmML
{
    CovariateModel::CovariateModel(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void CovariateModel::parse(xml::Node node) {
        std::vector<xml::Node> array = this->context->getElements(node, ".//mdef:Covariate");
        for (xml::Node n : array) {
            PharmML::Covariate *cov = new PharmML::Covariate(this->context, n);
            this->Covariates.push_back(cov);
        }
    }

    std::vector<PharmML::Covariate *> CovariateModel::getCovariates() {
        return this->Covariates;
    }

}
