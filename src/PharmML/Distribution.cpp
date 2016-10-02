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
    Distribution::Distribution(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void Distribution::parse(PharmMLReader &reader, xml::Node node) {
        std::string name = node.getName();
        if (name == "ProbOnto" || name == "MixtureComponent") {
            this->name = node.getAttribute("name").getValue();

            std::vector<xml::Node> params = reader.getElements(node, "./po:Parameter");
            for (xml::Node n : params) {
                std::shared_ptr<DistributionParameter> dist_param = std::make_shared<DistributionParameter>(reader, n);
                this->parameters.push_back(dist_param);
            }

            // Get mixture components (dist: MixtureDistribution) which just are nested Distribution objects
            std::vector<xml::Node> mix_dists = reader.getElements(node, "./po:MixtureComponent");
            for (xml::Node mix_dist : mix_dists) {
                std::shared_ptr<Distribution> dist = std::make_shared<Distribution>(reader, mix_dist);
                this->mixture_components.push_back(dist);
            }
        } else {
            // UncertML. Support only normal distribution and make lots of assumptions and hope that UncertML will go away.
            this->name = "Normal2";
            auto mean_param = std::make_shared<DistributionParameter>(reader);
            mean_param->setAssignment(std::make_shared<ScalarReal>(node.getChild().getChild().getChild().getText()));
            mean_param->setName("mean");
            this->parameters.push_back(mean_param);
            auto stdev_param = std::make_shared<DistributionParameter>(reader);
            stdev_param->setAssignment(std::make_shared<SymbRef>(node.getChild().getLastChild().getChild().getAttribute("varId").getValue()));
            stdev_param->setName("var");
            this->parameters.push_back(stdev_param);
        }
    }

    std::string Distribution::getName() {
        return this->name;
    }

    std::vector<std::shared_ptr<DistributionParameter>> Distribution::getDistributionParameters() {
        return this->parameters;
    }

    bool Distribution::hasMixtureComponents() {
        return !this->mixture_components.empty();
    }

    std::vector<std::shared_ptr<Distribution>> Distribution::getMixtureComponents() {
        return this->mixture_components;
    }

    /**
     *  Get all nested distribution parameters (e.g. also within mixture components and their distributions).
     *  Used for better setupSymbRef functionality in e.g. RandomVariable.
     */
    std::vector<std::shared_ptr<DistributionParameter>> Distribution::getAllDistributionParameters() {
        std::vector<std::shared_ptr<DistributionParameter>> all_params = this->parameters;
        for (auto const &mix_dist : this->mixture_components) {
            std::vector<std::shared_ptr<DistributionParameter>> mix_params = mix_dist->getAllDistributionParameters();
            all_params.insert(all_params.end(), mix_params.begin(), mix_params.end());
        }
        return all_params;
    }

    void Distribution::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
