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

    void FunctionCall::setFunction(std::unique_ptr<SymbRef> node) {
        this->function = std::move(node);
    }

    std::vector<std::unique_ptr<FunctionArgument>>& FunctionCall::getFunctionArguments() {
        return this->functionArguments;
    }

    SymbRef *FunctionCall::getFunction() {
        return this->function.get();
    }

    /**
     *  Creates a new FunctionArgument
     *  \param symbId The name of the function argument
     *  \param value The value of the function argument
     */
    FunctionArgument::FunctionArgument(std::string symbId, std::unique_ptr<AstNode> value) {
        if (!value) {
            throw std::invalid_argument("nullptr");
        }
        this->symbId = symbId;
        this->argument = std::move(value);
    }

    /**
     *  Creates a new FunctionArgument from PharmML xml
     */
    FunctionArgument::FunctionArgument(xml::Node node) {
        this->symbId = node.getAttribute("symbId").getValue();
        this->argument = AstNodeFactory::create(node.getChild());
    }

    FunctionArgument::FunctionArgument(const FunctionArgument &from) {
        this->symbId = from.symbId;
        this->argument = from.argument->clone();
    }

    FunctionArgument &FunctionArgument::operator=(const FunctionArgument &rhs) {
        if (&rhs != this) {
            this->symbId = rhs.symbId;
            this->argument = rhs.argument->clone();
        }
        return *this;
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
        std::unique_ptr<FunctionArgument> cl = std::make_unique<FunctionArgument>(this->symbId, this->argument->clone());
        return std::move(cl);
    }

    void FunctionArgument::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }
}
