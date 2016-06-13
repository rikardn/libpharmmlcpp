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

namespace pharmmlcpp
{
    class FunctionCall : public AstNode
    {
        public:
            void setFunction(SymbRef *node);
            SymbRef *getFunction();
            void addFunctionArgument(FunctionArgument *farg);
            std::vector<FunctionArgument *> getFunctionArguments();
            virtual void accept(AstNodeVisitor *visitor);
        
        private:
            SymbRef *function;
            std::vector<FunctionArgument *> functionArguments;
    };

    class FunctionArgument : public AstNode
    {
        public:
            void setSymbId(std::string symbId);
            std::string getSymbId();
            void setArgument(AstNode *node);
            AstNode *getArgument();
            virtual void accept(AstNodeVisitor *visitor);
        
        private:
            std::string symbId;
            AstNode *argument;
    };
}

#endif
