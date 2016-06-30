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

#ifndef PHARMMLCPP_VARIABLE_H_
#define PHARMMLCPP_VARIABLE_H_

#include <PharmML/PharmMLContext.h>
#include <visitors/PharmMLVisitor.h>

namespace pharmmlcpp
{
    class CommonVariable : public Symbol
    {
         public:
            CommonVariable(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            AstNode *getAssignment();
            bool isDerivative();
            virtual void setupSymbRefs(SymbolGathering &gathering, std::string blkId) = 0;
            virtual void accept(PharmMLVisitor *visitor) = 0;
            virtual void accept(SymbolVisitor *visitor) = 0;

         protected:
            AstNode *assignment = nullptr;
            bool is_derivative;
    };

    class Variable : public CommonVariable
    {
        public:
            Variable(PharmMLReader &reader, xml::Node node) : CommonVariable(reader, node) { is_derivative = false; };
            void setupSymbRefs(SymbolGathering &gathering, std::string blkId) override;
            void accept(PharmMLVisitor *visitor) override;
            void accept(SymbolVisitor *visitor) override;
    };

    class DesignParameter : public Variable
    {
        public:
            DesignParameter(PharmMLReader &reader, xml::Node node) : Variable(reader, node) {};
    };
}

#endif
