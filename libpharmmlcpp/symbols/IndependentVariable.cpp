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

#include "IndependentVariable.h"

namespace pharmmlcpp
{
    IndependentVariable::IndependentVariable(PharmMLReader &reader, xml::Node node) {
        this->IndependentVariable::parse(node);
    }

    void IndependentVariable::parse(xml::Node node) {
        this->Symbol::parse(node);
    }

    xml::Node IndependentVariable::xml(PharmMLWriter &writer) {
        xml::Node iv("IndependentVariable");
        this->Symbol::xml(writer, iv);
        return iv;
    }

    void IndependentVariable::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void IndependentVariable::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }
}
