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

#ifndef PHARMMLCPP_DERIVATIVEVARIABLE_H_
#define PHARMMLCPP_DERIVATIVEVARIABLE_H_

#include <PharmML/PharmMLReader.h>
#include <PharmML/PharmMLWriter.h>
#include <symbols/Variable.h>
#include <visitors/PharmMLVisitor.h>

namespace pharmmlcpp
{
    class DerivativeVariable : public CommonVariable
    {
        public:
            DerivativeVariable(PharmMLReader &reader, xml::Node node) : CommonVariable(reader, node) { is_derivative = true; this->parse(reader, node); };
            DerivativeVariable() { is_derivative = true; };
            std::shared_ptr<AstNode> getIndependentVariable();
            std::shared_ptr<AstNode> getInitialValue();
            std::shared_ptr<AstNode> getInitialTime();
            void setIndependentVariable(std::shared_ptr<AstNode> iv);
            void setInitialTime(std::shared_ptr<AstNode> time);
            void setInitialValue(std::shared_ptr<AstNode> value);
            void parse(PharmMLReader &reader, xml::Node node);
            xml::Node xml(PharmMLWriter &writer) override;
            void setupSymbRefs(SymbolGathering &gathering, std::string blkId) override;
            void accept(PharmMLVisitor *visitor);
            void accept(SymbolVisitor *visitor);
        
        private:
            std::shared_ptr<AstNode> independent_variable;
            std::shared_ptr<AstNode> initial_value;
            std::shared_ptr<AstNode> initial_time;
    };
}

#endif
