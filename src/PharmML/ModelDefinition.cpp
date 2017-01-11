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
        std::vector<xml::Node> obs_nodes = reader.getElements(node, "./mdef:ObservationModel");
        for (xml::Node obs_node : obs_nodes) {
            std::shared_ptr<ObservationModel> obs_model = std::make_shared<ObservationModel>(reader, obs_node);
            this->observation_models.push_back(obs_model);
        }
    }

    xml::Node ModelDefinition::xml(PharmMLWriter &writer) {
        xml::Node mdef("ModelDefinition");
        mdef.setAttribute("xmlns", "http://www.pharmml.org/pharmml/0.8/ModelDefinition");
        // FIXME: Have namespace getter in PharmMLWriter

        for (const auto &vmod : this->VariabilityModels) {
            mdef.addChild(vmod->xml(writer));
        }

        if (this->covariateModel) {
            mdef.addChild(this->covariateModel->xml(writer));
        }

        if (this->parameterModel) {
            mdef.addChild(this->parameterModel->xml(writer));
        }

        if (this->structuralModel) {
            mdef.addChild(this->structuralModel->xml(writer));
        }

        return mdef;
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

    // FIXME: Depreceated! Remove! Not only one observation model anymore!
    ObservationModel *ModelDefinition::getObservationModel() {
        return this->observation_models[0].get();
    }

    std::vector<std::shared_ptr<ObservationModel>> ModelDefinition::getObservationModels() {
        return this->observation_models;
    }

}
