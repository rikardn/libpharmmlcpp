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
            std::vector<FunctionArgument *> getFunctionArguments();
            virtual void accept(AbstractVisitor *visitor);
    };
    
    class FunctionArgument : public AstNode
    {
        std::string symbId;
        AstNode *Argument;

        public:
            void setSymbId(std::string symbId);
            std::string getSymbId();
            void setArgument(AstNode *node);
            AstNode *getArgument();
            virtual void accept(AbstractVisitor *visitor);
    };
}

#endif
