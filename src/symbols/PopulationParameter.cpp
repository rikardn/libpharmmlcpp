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

#include "PopulationParameter.h"

namespace PharmML
{
    // FIXME: How to get context when created like this
    PopulationParameter::PopulationParameter(std::string symbId) {
        this->symbId = symbId;        
    }

    PopulationParameter::PopulationParameter(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->PopulationParameter::parse(node);
        this->context->symbols[this->symbId] = this;
    }

    void PopulationParameter::parse(xml::Node node) {
        this->Symbol::parse(node);
    }

    void PopulationParameter::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void PopulationParameter::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }
}
