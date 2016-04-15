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
        std::string blkId;
        std::string symbId;
        AstNode *Output;
        AstNode *ErrorModel;
        AstNode *ResidualError;

        public:
        ObservationModel(PharmMLContext *context, xml::Node node);
        std::string getSymbId();
        AstNode *getOutput();
        AstNode *getErrorModel();
        AstNode *getResidualError();
        void parse(xml::Node node);
        void accept(AbstractVisitor *visitor);
    };
}

#endif
