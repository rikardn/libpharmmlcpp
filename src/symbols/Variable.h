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

#ifndef PHARMML_VARIABLE_H_
#define PHARMML_VARIABLE_H_

#include <PharmML/PharmMLContext.h>
#include <visitors/PharmMLVisitor.h>

namespace PharmML
{
    class CommonVariable : public Symbol
    {
         public:
            CommonVariable(PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            AstNode *getAssignment();
            Dependencies& getDependencies();
            bool isDerivative();
            virtual void gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) = 0;
            virtual void accept(PharmMLVisitor *visitor) = 0;
            virtual void accept(SymbolVisitor *visitor) = 0;
 
         protected:
            AstNode *assignment = nullptr;
            PharmML::PharmMLContext *context;
            Dependencies deps;
            bool is_derivative;
    };

    class Variable : public CommonVariable
    {
        public:
        Variable(PharmMLContext *context, xml::Node node) : CommonVariable(context, node) { is_derivative = false; };
        void gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap);
        void accept(PharmMLVisitor *visitor);
        void accept(SymbolVisitor *visitor);
    };
}

#endif
