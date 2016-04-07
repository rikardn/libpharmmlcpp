#ifndef PHARMML_INDEPENDENTVARIABLE_H_H
#define PHARMML_INDEPENDENTVARIABLE_H_H

#include "PharmMLContext.h"

namespace PharmML
{
    class IndependentVariable
    {
        PharmMLContext *context;
        std::string symbId;
        
        public:
        IndependentVariable(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getSymbId();
        std::string accept(AbstractVisitor *visitor);
    };
}

#endif
