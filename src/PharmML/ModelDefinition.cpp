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

#include <PharmML/ModelDefinition.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <xml/xml.h>

namespace pharmmlcpp
{
    ModelDefinition::ModelDefinition(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void ModelDefinition::parse(PharmMLReader &reader, xml::Node node) {
        std::vector<xml::Node> var_nodes = reader.getElements(node, "./mdef:VariabilityModel");
        for (xml::Node var_node : var_nodes) {
            VariabilityModel *varModel = new VariabilityModel(reader, var_node);
            this->VariabilityModels.push_back(varModel);
        }
        // TODO: Support for more than one of the following
        xml::Node cov_node = reader.getSingleElement(node, "./mdef:CovariateModel");
        if (cov_node.exists()) {
            this->covariateModel = new CovariateModel(reader, cov_node);
        }
        xml::Node struct_node = reader.getSingleElement(node, "./mdef:StructuralModel");
        if (struct_node.exists()) {
            this->structuralModel = new StructuralModel(reader, struct_node);
        }
        xml::Node param_node = reader.getSingleElement(node, "./mdef:ParameterModel");
        if (param_node.exists()) {
            this->parameterModel = new ParameterModel(reader, param_node);
        }
        xml::Node obs_node = reader.getSingleElement(node, "./mdef:ObservationModel");
        if (obs_node.exists()) {
            this->observationModel = new ObservationModel(reader, obs_node);
        }
    }


    void ModelDefinition::setupRefererSymbRefs(SymbolGathering &gathering) {
        //~ this->getVariabilityModels()->gatherSymbRefs(symbolMap);
        //~ this->getCovariateModel()->gatherSymbRefs(symbolMap);
        if (this->getParameterModel()) {
            this->getParameterModel()->setupRefererSymbRefs(gathering);
        }
        this->getStructuralModel()->setupRefererSymbRefs(gathering);
        //~ this->getObservationModel()->gatherSymbRefs(symbolMap);
    }

    std::vector<pharmmlcpp::VariabilityModel *> ModelDefinition::getVariabilityModels() {
        return this->VariabilityModels;
    }

    CovariateModel *ModelDefinition::getCovariateModel() {
        return this->covariateModel;
    }

    ParameterModel *ModelDefinition::getParameterModel() {
        return this->parameterModel;
    }

    StructuralModel *ModelDefinition::getStructuralModel() {
        return this->structuralModel;
    }

    ObservationModel *ModelDefinition::getObservationModel() {
        return this->observationModel;
    }
}
