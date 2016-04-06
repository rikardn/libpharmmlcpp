#include "FunctionCall.h"

namespace PharmML
{
    std::string FunctionCall::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    void FunctionCall::setFunctionName(AstNode *node) {
        this->FunctionName = node;
    }

    AstNode *FunctionCall::getFunctionName() {
        return this->FunctionName;
    }

    void FunctionCall::addFunctionArgument(FunctionArgument *farg) {
        this->FunctionArguments.push_back(farg);
    }

    std::vector<FunctionArgument *> FunctionCall::getFunctionArguments() {
        return this->FunctionArguments;
    }

    void FunctionArgument::setSymbId(std::string symbId) {
        this->symbId = symbId;
    }

    std::string FunctionArgument::getSymbId() {
        return this->symbId;
    }

    void FunctionArgument::setArgument(AstNode *node) {
        this->Argument = node;
    }

    AstNode *FunctionArgument::getArgument() {
        return this->Argument;
    }

    std::string FunctionArgument::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
}
