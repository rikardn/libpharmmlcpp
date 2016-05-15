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

#ifndef PHARMML_RPHARMMLCONSOLIDATOR_H_
#define PHARMML_RPHARMMLCONSOLIDATOR_H_

#include <string>
#include <vector>
#include <generators/TextFormatter.h>

namespace PharmML
{
    class DerivativesConsolidator
    {
        private:
            std::vector<std::string> x0;
            std::vector<std::string> y0;
            std::vector<std::string> y;
            std::vector<std::string> x;

        public:
            void addDerivative(std::string y, std::string x, std::string y0, std::string x0);
            std::vector<std::string> getSymbols();
            std::vector<std::string> getAssigns();
            std::string genInitVector();
    };
    
    class VariablesConsolidator
    {
        private:
            std::vector<std::string> symbols;
            std::vector<std::string> assigns;

        public:
            void addVariable(std::string symbol, std::string assign);
            std::string genStatements();
    };
    
    class Consolidator
    {
        public:
            DerivativesConsolidator derivs;
            VariablesConsolidator vars;
    };
}

#endif
