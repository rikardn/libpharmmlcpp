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

#include <PharmML/PharmMLContext.h>
#include <PharmML/ModelDefinition.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <xml/xml.h>

namespace PharmML
{
    ModelDefinition::ModelDefinition(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void ModelDefinition::parse(xml::Node node) {
        std::vector<xml::Node> var_nodes = this->context->getElements(node, "./mdef:VariabilityModel");
        for (xml::Node var_node : var_nodes) {
            VariabilityModel *varModel = new PharmML::VariabilityModel(this->context, var_node);
            this->VariabilityModels.push_back(varModel);
        }
        // TODO: Why are these all singular? It might be wise to support multiple models while it's still straightforward.
        xml::Node cov_node = this->context->getSingleElement(node, "./mdef:CovariateModel");
        if (cov_node.exists()) {
            this->CovariateModel = new PharmML::CovariateModel(this->context, cov_node);
        }
        xml::Node struct_node = this->context->getSingleElement(node, "./mdef:StructuralModel");
        if (struct_node.exists()) {
            this->StructuralModel = new PharmML::StructuralModel(this->context, struct_node);
        }
        xml::Node param_node = this->context->getSingleElement(node, "./mdef:ParameterModel");
        if (param_node.exists()) {
            this->ParameterModel = new PharmML::ParameterModel(this->context, param_node);
        }
        xml::Node obs_node = this->context->getSingleElement(node, "./mdef:ObservationModel");
        if (obs_node.exists()) {
            this->ObservationModel = new PharmML::ObservationModel(this->context, obs_node);
        }
    }
    
    std::vector<PharmML::VariabilityModel *> ModelDefinition::getVariabilityModels() {
        return this->VariabilityModels;
    }
    
    PharmML::CovariateModel *ModelDefinition::getCovariateModel() {
        return this->CovariateModel;
    }

    PharmML::ParameterModel *ModelDefinition::getParameterModel() {
        return this->ParameterModel;
    }

    PharmML::StructuralModel *ModelDefinition::getStructuralModel() {
        return this->StructuralModel;
    }
        
    PharmML::ObservationModel *ModelDefinition::getObservationModel() {
        return this->ObservationModel;
    }
}
