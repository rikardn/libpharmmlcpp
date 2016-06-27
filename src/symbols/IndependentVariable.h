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

#ifndef PHARMMLCPP_INDEPENDENTVARIABLE_H_H
#define PHARMMLCPP_INDEPENDENTVARIABLE_H_H

#include <PharmML/PharmMLContext.h>
#include <visitors/PharmMLVisitor.h>

namespace pharmmlcpp
{
    class IndependentVariable : public Symbol
    {
        public:
            IndependentVariable(PharmMLReader &reader, xml::Node node);
            void parse(xml::Node node);
            void setupSymbRefs(SymbolGathering &gathering, std::string blkId) {};
            void accept(PharmMLVisitor *visitor);
            void accept(SymbolVisitor *visitor);
    };
}

#endif
