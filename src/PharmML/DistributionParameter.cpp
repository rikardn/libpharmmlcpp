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

#include "DistributionParameter.h"
#include <iostream>

namespace pharmmlcpp
{
    DistributionParameter::DistributionParameter(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    DistributionParameter::DistributionParameter(PharmMLReader &reader) {
    }

    void DistributionParameter::parse(PharmMLReader &reader, xml::Node node) {
        this->name = node.getAttribute("name").getValue();
        this->assignment = reader.factory.create(node.getChild().getChild());
    }

    std::string DistributionParameter::getName() {
        return this->name;
    }

    void DistributionParameter::setName(std::string name) {
        this->name = name;
    }

    std::shared_ptr<AstNode> DistributionParameter::getAssignment() {
        return this->assignment;
    }

    void DistributionParameter::setAssignment(std::shared_ptr<AstNode> assignment) {
        this->assignment = assignment;
    }
}
