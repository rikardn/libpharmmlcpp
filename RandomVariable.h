#ifndef PHARMML_RANDOMVARIABLE_H_
#define PHARMML_RANDOMVARIABLE_H_

#include "PharmMLContext.h"

namespace PharmML
{
    class RandomVariable
    {
        PharmMLContext *context;
        std::string symbId;
        AstNode *VariabilityReference;

        public:
        RandomVariable(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getSymbId();
        AstNode *getVariabilityReference();
        std::string accept(AbstractVisitor *visitor);
    };
}

#endif
