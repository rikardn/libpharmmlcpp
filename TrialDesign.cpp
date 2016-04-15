#include "PharmMLContext.h"
#include "TrialDesign.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "xml.h"

namespace PharmML
{
    TrialDesign::TrialDesign(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void TrialDesign::parse(xml::Node node) {
        xml::Node external_dataset_node = this->context->getSingleElement(node, ".//design:ExternalDataSet");
        if (external_dataset_node.exists()) {
            this->ExternalDataset = new PharmML::ExternalDataset(this->context, external_dataset_node);
        }
        xml::Node interventions_node = this->context->getSingleElement(node, ".//design:Interventions");
        if (interventions_node.exists()) {
            this->Interventions = new PharmML::Interventions(this->context, interventions_node);
        }
        
        xml::Node observations_node = this->context->getSingleElement(node, ".//design:Observations");
        if (observations_node.exists()) {
            this->Observations = new PharmML::Observations(this->context, observations_node);
        }
        xml::Node arms_node = this->context->getSingleElement(node, ".//design:Arms");
        if (arms_node.exists()) {
            this->Arms = new PharmML::Arms(this->context, arms_node);
        }
    }

    PharmML::ExternalDataset *TrialDesign::getExternalDataset() {
        return this->ExternalDataset;
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
}
