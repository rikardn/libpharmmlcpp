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

#include "RandomVariable.h"

namespace PharmML
{
    RandomVariable::RandomVariable(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void RandomVariable::parse(xml::Node node) {
        this->symbId = node.getAttribute("symbId").getValue();
        xml::Node n = this->context->getSingleElement(node, ".//ct:VariabilityReference");
        if (n.exists()) {
            this->VariabilityReference = this->context->factory.create(n.getChild());
        }
        xml::Node dist_node = this->context->getSingleElement(node, ".//mdef:Distribution");
        if (dist_node.exists()) {
            this->Distribution = new PharmML::Distribution(context, dist_node.getChild());
        }

    }

    std::string RandomVariable::getSymbId() {
        return this->symbId;
    }

    AstNode *RandomVariable::getVariabilityReference() {
        return this->VariabilityReference;
    }

    PharmML::Distribution *RandomVariable::getDistribution() {
        return this->Distribution;
    }

    void RandomVariable::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }
}
