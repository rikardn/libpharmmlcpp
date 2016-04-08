#ifndef PHARMML_MODELDEFINITION_H_
#define PHARMML_MODELDEFINITION_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "PharmMLContext.h"
#include "CovariateModel.h"
#include "ParameterModel.h"
#include "StructuralModel.h"
#include "ObservationModel.h"

namespace PharmML
{
    class ModelDefinition
    {
        PharmML::PharmMLContext *context;
        PharmML::CovariateModel *CovariateModel = nullptr;
        PharmML::ParameterModel *ParameterModel = nullptr;
        PharmML::StructuralModel *StructuralModel = nullptr;
        PharmML::ObservationModel *ObservationModel = nullptr;

        public:
        PharmML::CovariateModel *getCovariateModel();
        PharmML::ParameterModel *getParameterModel();
        PharmML::StructuralModel *getStructuralModel();
        PharmML::ObservationModel *getObservationModel();
        ModelDefinition(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
    };
}

#endif
