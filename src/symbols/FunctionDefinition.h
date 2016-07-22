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

#ifndef PHARMMLCPP_FUNCTIONDEFINITON_H_
#define PHARMMLCPP_FUNCTIONDEFINITON_H_

#include <AST/AstNode.h>
#include <visitors/PharmMLVisitor.h>
#include <visitors/SymbolVisitor.h>
#include <visitors/AstAnalyzer.h>
#include <helpers/StringTools.h>

namespace pharmmlcpp
{
    class FunctionArgumentDefinition : public Symbol
    {
        public:
            FunctionArgumentDefinition(PharmMLReader &reader, xml::Node node);

            std::string getType();

            void setupSymbRefs(SymbolGathering &gathering, std::string blkId) {};
            void accept(PharmMLVisitor *visitor);
            void accept(SymbolVisitor *visitor);

        private:
            std::string symbolType;
            void parse(PharmMLReader &reader, xml::Node node);
    };

    enum class StandardFunction { additiveError, proportionalError, combinedError1, NA };
    enum class StandardFunctionArgument { additive, proportional, prediction };
    struct EnumClassHash
    {
        template <typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };
    class FunctionDefinition : public Symbol
    {
        public:
            FunctionDefinition(PharmMLReader &reader, xml::Node node);

            std::string getType();
            std::vector<FunctionArgumentDefinition *> getArguments();
            std::shared_ptr<AstNode> getDefinition();

            void setupSymbRefs(SymbolGathering &gathering, std::string blkId);
            void accept(PharmMLVisitor *visitor);
            void accept(SymbolVisitor *visitor);

            // POSTPARSE/CONSOLIDATION
            bool isStandardFunction();
            StandardFunction getStandardFunction();
            std::unordered_map<StandardFunctionArgument, pharmmlcpp::FunctionArgumentDefinition *, EnumClassHash> getStandardFunctionArgumentMap();

        private:
            std::shared_ptr<AstNode> definition;
            std::string symbolType;
            std::vector<FunctionArgumentDefinition *> arguments;

            void parse(PharmMLReader &reader, xml::Node node);

            // POSTPARSE/CONSOLIDATION
            std::string linear_form;
            StandardFunction std_func;
            std::unordered_map<StandardFunctionArgument, pharmmlcpp::FunctionArgumentDefinition *, EnumClassHash> std_arg_map;
    };
}

#endif
