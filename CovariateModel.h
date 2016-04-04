#ifndef PHARMML_COVARIATEMODEL_H_
#define PHARMML_COVARIATEMODEL_H_

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "xml.h"
#include "PharmMLContext.h"
#include "Covariate.h"

namespace PharmML
{
    class CovariateModel
    {
        PharmML::PharmMLContext *context;
        std::vector<PharmML::Covariate *> Covariates;

        public:
        CovariateModel(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::vector<PharmML::Covariate *> getCovariates();
    };
}

#endif
