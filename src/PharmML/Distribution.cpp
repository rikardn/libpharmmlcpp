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

#include "Distribution.h"
#include <iostream>

namespace pharmmlcpp
{
    Distribution::Distribution(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void Distribution::parse(xml::Node node) {
        if (node.getName() == "ProbOnto") {
            this->name = node.getAttribute("name").getValue();
            std::vector<xml::Node> params = this->context->getElements(node, ".//po:Parameter");
            for (xml::Node n : params) {
                DistributionParameter *dist_param = new pharmmlcpp::DistributionParameter(this->context, n);
                this->parameters.push_back(dist_param);
            }
        } else {
            // UncertML. Support only normal distribution and make lots of assumptions and hope that UncertML will go away.
            this->name = "Normal2";
            auto mean_param = new pharmmlcpp::DistributionParameter(this->context);
            mean_param->setAssignment(new ScalarReal(node.getChild().getChild().getChild().getText()));
            mean_param->setName("mean");
            this->parameters.push_back(mean_param);
            auto stdev_param = new pharmmlcpp::DistributionParameter(this->context);
            stdev_param->setAssignment(new SymbRef(node.getChild().getLastChild().getChild().getAttribute("varId").getValue()));
            stdev_param->setName("var");
            this->parameters.push_back(stdev_param);
        }
    }

    std::string Distribution::getName() {
        return this->name;
    }

    std::vector<pharmmlcpp::DistributionParameter *> Distribution::getDistributionParameters() {
        return this->parameters;
    }

    void Distribution::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
