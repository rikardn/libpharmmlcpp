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

#include "PharmMLSection.h"

namespace pharmmlcpp
{
    PharmMLSection::PharmMLSection() {

    }

    PharmMLSection::PharmMLSection(xml::Node node) {
        this->xml_node = node;
    }

    xml::Node PharmMLSection::getXMLNode() {
        return this->xml_node;
    }

    void PharmMLSection::setXMLNode(xml::Node node) {
        this->xml_node = node;
    }
}
