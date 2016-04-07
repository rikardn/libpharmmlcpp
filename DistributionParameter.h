#ifndef PHARMML_DISTRIBUTIONPARAMETER_H_
#define PHARMML_DISTRIBUTIONPARAMETER_H_

#include "PharmMLContext.h"
#include "AstNode.h"

namespace PharmML
{
    class DistributionParameter
    {
        PharmMLContext *context;
        std::string name;
        AstNode *assignment;

        public:
        DistributionParameter(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getName();
        AstNode *getAssignment();
    };
}

#endif
