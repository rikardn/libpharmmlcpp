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

namespace PharmML
{
    TrialDesign::TrialDesign(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->xml_node = node;
        this->parse(node);
    }

    void TrialDesign::parse(xml::Node node) {
        std::vector<xml::Node> ext_dataset_nodes = this->context->getElements(node, "./design:ExternalDataSet");
        for (xml::Node ext_dataset_node : ext_dataset_nodes) {
            ExternalDataset* ds = new PharmML::ExternalDataset(this->context, ext_dataset_node);
            this->ExternalDatasets.push_back(ds);
        }
        xml::Node interventions_node = this->context->getSingleElement(node, "./design:Interventions");
        if (interventions_node.exists()) {
            this->Interventions = new PharmML::Interventions(this->context, interventions_node);
        }

        xml::Node observations_node = this->context->getSingleElement(node, "./design:Observations");
        if (observations_node.exists()) {
            this->Observations = new PharmML::Observations(this->context, observations_node);
        }

        xml::Node arms_node = this->context->getSingleElement(node, "./design:Arms");
        if (arms_node.exists()) {
            this->Arms = new PharmML::Arms(this->context, arms_node);
        }

        xml::Node ds_node = this->context->getSingleElement(node, "./design:DesignSpaces");
        if (ds_node.exists()) {
            this->DesignSpaces = new PharmML::DesignSpaces(this->context, ds_node);
        }
    }

    void TrialDesign::update() {
        xml::Node td("TrialDesign");
        td.setAttribute("xmlns", "http://www.pharmml.org/pharmml/0.8/TrialDesign");
        if (this->Interventions)
            td.addChild(this->Interventions->xml());
        if (this->Observations)
            td.addChild(this->Observations->xml());
        if (this->DesignSpaces)
            td.addChild(this->DesignSpaces->xml());
        if (this->Arms)
            td.addChild(this->Arms->xml());

        this->xml_node.replaceNode(td);
    }

    std::vector<PharmML::ExternalDataset *> TrialDesign::getExternalDatasets() {
        return this->ExternalDatasets;
    }

    PharmML::Interventions *TrialDesign::getInterventions() {
        return this->Interventions;
    }

    PharmML::Observations *TrialDesign::getObservations() {
        return this->Observations;
    }

    PharmML::Arms *TrialDesign::getArms() {
        return this->Arms;
    }

    PharmML::DesignSpaces *TrialDesign::getDesignSpaces() {
        return this->DesignSpaces;
    }

    void TrialDesign::setupRefererSymbRefs(SymbolGathering &gathering) {
        std::vector<ExternalDataset *> ext_datasets = this->getExternalDatasets();
        for (ExternalDataset *ext_dataset : ext_datasets) {
            ext_dataset->setupRefererSymbRefs(gathering);
        }
        this->getInterventions()->setupRefererSymbRefs(gathering);
        //~ this->getObservations()->gatherSymbRefs(symbolMap);
        //~ this->getArms()->gatherSymbRefs(symbolMap);
        //~ this->getDesignSpaces()->gatherSymbRefs(symbolMap);
    }
}
