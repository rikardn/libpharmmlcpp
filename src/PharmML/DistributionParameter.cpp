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

namespace PharmML
{
    DistributionParameter::DistributionParameter(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    DistributionParameter::DistributionParameter(PharmMLContext *context) {
        this->context = context;
    }

    void DistributionParameter::parse(xml::Node node) {
        this->name = node.getAttribute("name").getValue();
        this->assignment = context->factory.create(node.getChild().getChild());
    }

    std::string DistributionParameter::getName() {
        return this->name;
    }

    void DistributionParameter::setName(std::string name) {
        this->name = name;
    }

    AstNode *DistributionParameter::getAssignment() {
        return this->assignment;
    }

    void DistributionParameter::setAssignment(AstNode *assignment) {
        this->assignment = assignment;
    }
}
