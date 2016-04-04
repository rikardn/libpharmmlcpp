#ifndef PHARMML_VARIABLE_H_
#define PHARMML_VARIABLE_H_

#include "PharmMLContext.h"
#include "AbstractVisitor.h"

namespace PharmML
{
    class Variable
    {
        PharmML::PharmMLContext *context;
        AstNode *assignment = nullptr;
        std::string symbId;

        public:
        Variable(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        AstNode *getAssignment();
        std::string getSymbId();
        std::string accept(AbstractVisitor *visitor);
    };
}

#endif
