#ifndef PHARMML_FUNCTIONCALL_H_
#define PHARMML_FUNCTIONCALL_H_

#include <vector>
#include "AstNode.h"

namespace PharmML
{
    class FunctionCall : public AstNode
    {
        AstNode *FunctionName;
        std::vector<FunctionArgument *> FunctionArguments;

        public:
            void setFunctionName(AstNode *node);
            AstNode *getFunctionName();
            void addFunctionArgument(FunctionArgument *farg);
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class FunctionArgument : public AstNode
    {
        std::string symbId;
        AstNode *Argument;

        public:
            void setSymbId(std::string symbId);
            void setArgument(AstNode *node);
            virtual std::string accept(AbstractVisitor *visitor);
    };
}

#endif
