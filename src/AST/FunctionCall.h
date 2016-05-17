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

#ifndef PHARMML_FUNCTIONCALL_H_
#define PHARMML_FUNCTIONCALL_H_

#include <vector>
#include <unordered_set>
#include "AstNode.h"
#include "symbols/Symbol.h"

namespace PharmML
{
    class FunctionCall : public AstNode, public Referer
    {
        SymbRef *FunctionName;
        std::vector<FunctionArgument *> FunctionArguments;

        public:
            void setFunctionName(SymbRef *node);
            SymbRef *getFunctionName();
            void addFunctionArgument(FunctionArgument *farg);
            std::vector<FunctionArgument *> getFunctionArguments();
            void gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap);
            virtual void accept(AstNodeVisitor *visitor);
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
            virtual void accept(AstNodeVisitor *visitor);
    };
}

#endif
