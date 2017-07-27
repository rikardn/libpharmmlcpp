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

#include "MDLDeclaredVariables.h"

namespace pharmmlcpp
{
    void MDLDeclaredVariables::add(std::string name, std::string type) {
        declarations.insert(name + "::" + type);
    }

    std::string MDLDeclaredVariables::generateMDL() {
        if (!this->declarations.empty()) {
            TextFormatter form;
            form.openIndent();  // To follow parent indentation level
            form.indentAdd("DECLARED_VARIABLES {");
            for (auto &row : this->declarations) {
                form.add(row);
            }
            form.outdentAdd("}");
            form.emptyLine();
            return form.createString();
        } else {
            return "";
        }
    }
}
