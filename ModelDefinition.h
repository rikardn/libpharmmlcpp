#ifndef PHARMML_MODELDEFINITION_H_
#define PHARMML_MODELDEFINITION_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "PharmMLContext.h"
#include "CovariateModel.h"
#include "StructuralModel.h"

namespace PharmML
{
    class ModelDefinition
    {
        PharmML::PharmMLContext *context;
        PharmML::CovariateModel *CovariateModel;
        PharmML::StructuralModel *StructuralModel;

        public:
        PharmML::CovariateModel *getCovariateModel();
        PharmML::StructuralModel *getStructuralModel();
        ModelDefinition(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
    };
}

#endif
