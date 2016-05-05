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

#include "ParameterModel.h"

namespace PharmML
{
    ParameterModel::ParameterModel(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void ParameterModel::parse(xml::Node node) {
        this->blkId = node.getAttribute("blkId").getValue();
        std::vector<xml::Node> pop_nodes = this->context->getElements(node, ".//mdef:PopulationParameter");
        for (xml::Node n : pop_nodes) {
            PharmML::PopulationParameter *pop = new PharmML::PopulationParameter(this->context, n);
            this->PopulationParameters.push_back(pop);
        }

        std::vector<xml::Node> ind_nodes = this->context->getElements(node, ".//mdef:IndividualParameter");
        for (xml::Node n : ind_nodes) {
            PharmML::IndividualParameter *ind = new PharmML::IndividualParameter(this->context, n);
            this->IndividualParameters.push_back(ind);
        }

        std::vector<xml::Node> rand_nodes = this->context->getElements(node, ".//mdef:RandomVariable");
        for (xml::Node n : rand_nodes) {
            PharmML::RandomVariable *ind = new PharmML::RandomVariable(this->context, n);
            this->RandomVariables.push_back(ind);
        }
    }

    std::vector<PopulationParameter *> ParameterModel::getPopulationParameters() {
        return this->PopulationParameters;
    }

    std::vector<IndividualParameter *> ParameterModel::getIndividualParameters() {
        return this->IndividualParameters;
    }
        
    std::vector<RandomVariable *> ParameterModel::getRandomVariables() {
        return this->RandomVariables;
    }
    
    std::string ParameterModel::getBlkId() {
        return this->blkId;
    }
}
