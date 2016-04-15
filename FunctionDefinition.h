#ifndef PHARMML_FUNCTIONDEFINITON_H_
#define PHARMML_FUNCTIONDEFINITON_H_

#include "PharmMLContext.h"
#include "AstNode.h"

namespace PharmML
{
    class FunctionDefinition
    {
        PharmMLContext *context;
        std::string symbId;
        AstNode *assignment;
        std::vector<std::string> arguments;
        void parse(xml::Node node);

        public:
        FunctionDefinition(PharmMLContext *context, xml::Node node);
        std::string getSymbId();
        std::vector<std::string> getArguments();
        AstNode *getAssignment();
        void accept(AbstractVisitor *visitor);
    };
}

#endif
