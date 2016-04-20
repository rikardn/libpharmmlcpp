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

#ifndef PHARMML_POPULATIONPARAMETER_H_
#define PHARMML_POPULATIONPARAMETER_H_

#include "PharmMLContext.h"
#include "xml.h"

namespace PharmML
{
    class PopulationParameter
    {
        PharmMLContext *context;
        std::string symbId;

        public:
        PopulationParameter(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getSymbId();
        void accept(AbstractVisitor *visitor);
    };
}

#endif
