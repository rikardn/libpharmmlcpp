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
#include <PharmML/TrialDesign.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <xml/xml.h>

namespace pharmmlcpp
{
    TrialDesign::TrialDesign(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->xml_node = node;
        this->parse(node);
    }

    void TrialDesign::parse(xml::Node node) {
        std::vector<xml::Node> ext_dataset_nodes = this->context->getElements(node, "./design:ExternalDataSet");
        for (xml::Node ext_dataset_node : ext_dataset_nodes) {
            ExternalDataset* ds = new ExternalDataset(this->context, ext_dataset_node);
            this->externalDatasets.push_back(ds);
        }
        xml::Node interventions_node = this->context->getSingleElement(node, "./design:Interventions");
        if (interventions_node.exists()) {
            this->interventions = new Interventions(this->context, interventions_node);
        }

        xml::Node observations_node = this->context->getSingleElement(node, "./design:Observations");
        if (observations_node.exists()) {
            this->observations = new Observations(this->context, observations_node);
        }

        xml::Node arms_node = this->context->getSingleElement(node, "./design:Arms");
        if (arms_node.exists()) {
            this->arms = new Arms(this->context, arms_node);
        }

        xml::Node ds_node = this->context->getSingleElement(node, "./design:DesignSpaces");
        if (ds_node.exists()) {
            this->designSpaces = new DesignSpaces(this->context, ds_node);
        }
        
        std::vector<xml::Node> dspar_nodes = this->context->getElements(node, "./design:DesignParameter");
        for (xml::Node dspar_node : dspar_nodes) {
            DesignParameter *dspar = new DesignParameter(this->context, dspar_node);
            this->designParameters.push_back(dspar);
        }
    }

    void TrialDesign::update() {
        xml::Node td("TrialDesign");
        td.setAttribute("xmlns", "http://www.pharmml.org/pharmml/0.8/TrialDesign");
        if (this->interventions)
            td.addChild(this->interventions->xml());
        if (this->observations)
            td.addChild(this->observations->xml());
        if (this->designSpaces)
            td.addChild(this->designSpaces->xml());
        if (this->arms)
            td.addChild(this->arms->xml());

        this->xml_node.replaceNode(td);
    }

    std::vector<ExternalDataset *> TrialDesign::getExternalDatasets() {
        return this->externalDatasets;
    }

    Interventions *TrialDesign::getInterventions() {
        return this->interventions;
    }

    Observations *TrialDesign::getObservations() {
        return this->observations;
    }

    Arms *TrialDesign::getArms() {
        return this->arms;
    }

    DesignSpaces *TrialDesign::getDesignSpaces() {
        return this->designSpaces;
    }
    
    std::vector<DesignParameter *> TrialDesign::getDesignParameters() {
        return this->designParameters;
    }

    void TrialDesign::setupRefererSymbRefs(SymbolGathering &gathering) {
        std::vector<ExternalDataset *> ext_datasets = this->getExternalDatasets();
        for (ExternalDataset *ext_dataset : ext_datasets) {
            ext_dataset->setupRefererSymbRefs(gathering);
        }
        if (this->getInterventions()) {
            this->getInterventions()->setupRefererSymbRefs(gathering);
        }
        //~ this->getObservations()->gatherSymbRefs(symbolMap);
        //~ this->getArms()->gatherSymbRefs(symbolMap);
        //~ this->getDesignSpaces()->gatherSymbRefs(symbolMap);
    }

    void TrialDesign::setupTargetMappings(SymbolGathering &gathering) {
        for (pharmmlcpp::ExternalDataset *ext_ds : this->externalDatasets) {
            ext_ds->setupTargetMappings(gathering);
        }
    }

    void TrialDesign::setupTargetMappings(MacroGathering &gathering) {
        for (pharmmlcpp::ExternalDataset *ext_ds : this->externalDatasets) {
            ext_ds->setupTargetMappings(gathering);
        }
    }
}
