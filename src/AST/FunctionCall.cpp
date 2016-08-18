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
    /**
     *  Create a FunctionCall starting with no arguments
     *  \param function A SymbRef refering to the function to call
     */
    FunctionCall::FunctionCall(std::unique_ptr<SymbRef> function) {
        this->function = std::move(function);
    }

    /**
     *  Create a FunctionCall from PharmML xml
     */
    FunctionCall::FunctionCall(xml::Node node) {
        std::vector<xml::Node> children = node.getChildren();
        xml::Node name_node = children[0];
        children.erase(children.begin());
        this->function = std::make_unique<SymbRef>(name_node);
        for (xml::Node n : children) {
            functionArguments.push_back(std::make_unique<FunctionArgument>(n));
        }
    }

    FunctionCall::FunctionCall(const FunctionCall &from) {
        this->function = std::make_unique<SymbRef>(*from.function.get());
        for (auto &arg : from.functionArguments) {
            auto new_arg = std::make_unique<FunctionArgument>(*arg.get());
            this->functionArguments.push_back(std::move(new_arg));
        }
    }

    FunctionCall &FunctionCall::operator=(const FunctionCall &rhs) {
        if (&rhs != this) {
            this->function = std::make_unique<SymbRef>(*rhs.function.get());
            for (auto &arg : rhs.functionArguments) {
                auto new_arg = std::make_unique<FunctionArgument>(*arg.get());
                this->functionArguments.push_back(std::move(new_arg));
            }
        }
        return *this;
    }

    void FunctionCall::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    /**
     *  Create a deep copy
     */
    std::unique_ptr<AstNode> FunctionCall::clone() {
        std::unique_ptr<AstNode> func = this->function->clone();
        std::unique_ptr<SymbRef> sr(static_cast<SymbRef *>(func.release()));
        std::unique_ptr<FunctionCall> cl = std::make_unique<FunctionCall>(std::move(sr));
        for (auto &arg : this->functionArguments) {
            std::unique_ptr<AstNode> arg_clone_ast = arg->clone();
            std::unique_ptr<FunctionArgument> arg_clone(static_cast<FunctionArgument *>(arg_clone_ast.release()));
            cl->functionArguments.push_back(std::move(arg_clone));
        }
        return std::move(cl);
    }

    /**
     *  Set the SymbRef pointing to the function for this call
     */
    void FunctionCall::setFunction(std::unique_ptr<SymbRef> node) {
        this->function = std::move(node);
    }

    /**
     *  Get the vector of FunctionArguments
     */
    std::vector<std::unique_ptr<FunctionArgument>>& FunctionCall::getFunctionArguments() {
        return this->functionArguments;
    }

    /**
     *  Get the SymbRef pointing to the function for this call
     */
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

    /**
     *  Set the name of this argument
     */
    void FunctionArgument::setSymbId(std::string symbId) {
        this->symbId = symbId;
    }

    /**
     *  Get the name of this argument
     */
    std::string FunctionArgument::getSymbId() {
        return this->symbId;
    }

    /**
     *  Set the actual argument
     */
    void FunctionArgument::setArgument(std::unique_ptr<AstNode> node) {
        this->argument = std::move(node);
    }

    /**
     *  Get the actual argument
     */
    AstNode *FunctionArgument::getArgument() {
        return this->argument.get();
    }

    /**
     *  Create a clone
     */
    std::unique_ptr<AstNode> FunctionArgument::clone() {
        std::unique_ptr<FunctionArgument> cl = std::make_unique<FunctionArgument>(this->symbId, this->argument->clone());
        return std::move(cl);
    }

    void FunctionArgument::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }
}
