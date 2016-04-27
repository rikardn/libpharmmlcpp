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

#ifndef PHARMML_POPEDGENERATOR_H_
#define PHARMML_POPEDGENERATOR_H_

#include <string>
#include "AstNodeVisitor.h"
#include "PopulationParameter.h"
#include "IndividualParameter.h"
#include "Variable.h"
#include "RAstGenerator.h"
#include "RPharmMLGenerator.h"

namespace PharmML
{
    class PopEDGenerator : public PharmMLVisitor
    {
        private:
            RAstGenerator ast_gen;
            RPharmMLGenerator r_gen;
            std::string value;
            void setValue(std::string str);
            // Helper function to reduce redundant code
            std::string formatVector(std::vector<std::string> vector, std::string prefix, std::string quote = "'", int pre_indent = 0);
            std::string accept(AstNode *);

        public:
            std::string getValue();
            std::string genParameterModelFunc();
    };
}

#endif
