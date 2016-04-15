#ifndef PHARMML_RANDOMVARIABLE_H_
#define PHARMML_RANDOMVARIABLE_H_

#include "PharmMLContext.h"
#include "Distribution.h"

namespace PharmML
{
    class RandomVariable
    {
        PharmMLContext *context;
        std::string symbId;
        AstNode *VariabilityReference;
        PharmML::Distribution *Distribution;

        public:
        RandomVariable(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getSymbId();
        AstNode *getVariabilityReference();
        PharmML::Distribution *getDistribution();
        void accept(AbstractVisitor *visitor);
    };
}

#endif
