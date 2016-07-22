/* libpharmmlcpp - Library to handle PharmML
 * Copyright (C) 2016 Rikard Nordgren and Gunnar Yngman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * his library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "FunctionCall.h"

namespace pharmmlcpp
{
    void FunctionCall::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    std::unique_ptr<AstNode> FunctionCall::clone() {
        std::unique_ptr<FunctionCall> cl;
        return std::move(cl);
    }

    void FunctionCall::setFunction(SymbRef *node) {
        this->function = node;
    }

    SymbRef *FunctionCall::getFunction() {
        return this->function;
    }

    void FunctionCall::addFunctionArgument(FunctionArgument *farg) {
        this->functionArguments.push_back(farg);
    }

    std::vector<FunctionArgument *> FunctionCall::getFunctionArguments() {
        return this->functionArguments;
    }

    void FunctionArgument::setSymbId(std::string symbId) {
        this->symbId = symbId;
    }

    std::string FunctionArgument::getSymbId() {
        return this->symbId;
    }

    void FunctionArgument::setArgument(std::unique_ptr<AstNode> node) {
        this->argument = std::move(node);
    }

    AstNode *FunctionArgument::getArgument() {
        return this->argument.get();
    }

    std::unique_ptr<AstNode> FunctionArgument::clone() {
        std::unique_ptr<FunctionArgument> cl;
        return std::move(cl);
    }

    void FunctionArgument::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }
}
