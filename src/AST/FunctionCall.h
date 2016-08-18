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

#ifndef PHARMMLCPP_FUNCTIONCALL_H_
#define PHARMMLCPP_FUNCTIONCALL_H_

#include <vector>
#include <unordered_set>
#include "AstNode.h"
#include "symbols/Symbol.h"
#include <AST/AstNodeFactory.h>

namespace pharmmlcpp
{
    /**
     *  A function call with name of function and argument list
     */
    class FunctionCall : public AstNode
    {
        public:
            FunctionCall(std::unique_ptr<SymbRef> function);
            FunctionCall(xml::Node node);
            FunctionCall(const FunctionCall &from);
            void setFunction(std::unique_ptr<SymbRef> node);
            SymbRef *getFunction();
            std::vector<std::unique_ptr<FunctionArgument>>& getFunctionArguments();
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;

        private:
            std::unique_ptr<SymbRef> function;
            std::vector<std::unique_ptr<FunctionArgument>> functionArguments;
    };

    /**
     *  A function argument for a function call. Really a key value pair.
     */
    class FunctionArgument : public AstNode
    {
        public:
            FunctionArgument(std::string symbId, std::unique_ptr<AstNode> value);
            FunctionArgument(xml::Node xml);
            FunctionArgument(const FunctionArgument &from);
            void setSymbId(std::string symbId);
            std::string getSymbId();
            void setArgument(std::unique_ptr<AstNode> node);
            AstNode *getArgument();
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
        
        private:
            std::string symbId;
            std::unique_ptr<AstNode> argument;
    };
}

#endif
