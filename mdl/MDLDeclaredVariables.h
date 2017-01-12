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

#ifndef PHARMMLCPP_MDLDECLAREDVARIABLES_H_
#define PHARMMLCPP_MDLDECLAREDVARIABLES_H_

#include <generators/TextFormatter.h>
#include <string>
#include <set>

namespace pharmmlcpp
{
    class MDLDeclaredVariables
    {
        public:
            void add(std::string name, std::string type);
            std::string generateMDL();

        private:
            std::set<std::string> declarations;
    };
}

#endif
