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

#ifndef CPHARMML_FUNCTIONS_H_
#define CPHARMML_FUNCTIONS_H_

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <AST/AstNode.h>
#include <symbols/Symbol.h>
#include <symbols/FunctionDefinition.h>
#include <visitors/AstAnalyzer.h>

namespace CPharmML
{
    class Functions
    {
        public:
            // Add a Function
            void addFunctionDefinition(PharmML::FunctionDefinition *functionDefinition);

            std::vector<PharmML::FunctionDefinition *> getFunctionDefinitions();

            PharmML::FunctionDefinition *resolveFunctionCall(PharmML::FunctionCall *functionCall);
            std::string getLinearizedFunctionDefinition(PharmML::FunctionDefinition *functionDefinition);
            bool isStandardFunction(PharmML::FunctionDefinition *functionDefinition);
            std::string getStandardFunctionName(PharmML::FunctionDefinition *functionDefinition);
            std::unordered_map<std::string, PharmML::FunctionArgumentDefinition *> getStandardArgumentMap(PharmML::FunctionDefinition *functionDefinition);

        private:
            // PharmML objects used to consolidate
            std::vector<PharmML::FunctionDefinition *> functionDefinitions;

            std::unordered_map<PharmML::FunctionDefinition *, std::string> linearizedFunctionDefinitions;
            std::unordered_map<PharmML::FunctionDefinition *, std::string> standardFunctions;
            std::unordered_map<PharmML::FunctionDefinition *, std::unordered_map<std::string, PharmML::FunctionArgumentDefinition *>> standardArgumentMaps;

            void identifyStandardFunction(PharmML::FunctionDefinition *functionDefinition);
            void replaceString(std::string &original, const std::string &replace, const std::string &replacement);
    };
}

#endif
