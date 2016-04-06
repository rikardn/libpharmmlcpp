#ifndef PHARMML_OBSERVATIONMODEL_H_
#define PHARMML_OBSERVATIONMODEL_H_

#include "PharmMLContext.h"
#include "AstNode.h"
#include "xml.h"

namespace PharmML
{
    class ObservationModel
    {
        PharmML::PharmMLContext *context;
        AstNode *ErrorModel;

        public:
        ObservationModel(PharmMLContext *context, xml::Node node);
        AstNode *getErrorModel();
        void parse(xml::Node node);
    };
}

#endif
