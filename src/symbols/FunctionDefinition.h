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

#ifndef PHARMML_FUNCTIONDEFINITON_H_
#define PHARMML_FUNCTIONDEFINITON_H_

#include <PharmML/PharmMLContext.h>
#include <AST/AstNode.h>
#include <visitors/PharmMLVisitor.h>
#include <visitors/SymbolVisitor.h>

namespace PharmML
{
    class FunctionArgumentDefinition : public Symbol
    {
        public:
            FunctionArgumentDefinition(PharmMLContext *context, xml::Node node);
            
            std::string getType();
            
            void gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {};
            void accept(PharmMLVisitor *visitor);
            void accept(SymbolVisitor *visitor);

        private:
            PharmMLContext *context;
            std::string symbolType;
            void parse(xml::Node node);
    };
    
    class FunctionDefinition : public Symbol
    {
        public:
            FunctionDefinition(PharmMLContext *context, xml::Node node);
            
            std::string getType();
            std::vector<FunctionArgumentDefinition *> getArguments();
            AstNode *getDefinition();
            
            void gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap);
            void accept(PharmMLVisitor *visitor);
            void accept(SymbolVisitor *visitor);

        private:
            PharmMLContext *context;
            
            AstNode *definition = nullptr;
            std::string symbolType;
            std::vector<FunctionArgumentDefinition *> arguments;
            
            void parse(xml::Node node);
    };
}

#endif
