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

#include <iostream>
#include <generators/PopED/PopEDAstGenerator.h>
#include <symbols/PopulationParameter.h>
#include <generators/PopED/PopEDSymbols.h>
#include <generators/PopED/PopEDErrorAstGenerator.h>

namespace PharmML
{
    void PopEDErrorAstGenerator::visit(FunctionCall *node) {
        node->getFunction()->accept(this);
        std::string function_name = this->getValue();

        bool first = true;
        std::string argument_list;
        for (FunctionArgument *arg : node->getFunctionArguments()) {
            if (!first) {
                argument_list += ", ";
            }
            if (function_name == "proportionalError" && first) {
                first = false;
                argument_list += "proportional=1";
            } else {
                arg->accept(this);
                argument_list += this->getValue();
            }
        }
        this->setValue(function_name + "(" + argument_list + ")");
    }
}
