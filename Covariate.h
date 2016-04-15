#ifndef PHARMML_COVARIATE_H_
#define PHARMML_COVARIATE_H_

#include "xml.h"
#include "PharmMLContext.h"
#include "AstNode.h"

namespace PharmML
{
    class Covariate
    {
        PharmML::PharmMLContext *context;
        std::string transformedName;
        AstNode *assignment;

        public:
        Covariate(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getTransformedName();
        AstNode *getAssignment();
        void accept(AbstractVisitor *visitor);
    };
}

#endif
