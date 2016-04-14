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
        xml::Node intervention_node = this->context->getSingleElement(node, ".//design:Interventions");
        if (intervention_node.exists()) {
            this->Intervention = new PharmML::Intervention(this->context, intervention_node);
        }
        
        xml::Node observation_node = this->context->getSingleElement(node, ".//design:Observations");
        if (observation_node.exists()) {
            this->Observation = new PharmML::Observation(this->context, observation_node);
        }
        //~ xml::Node arm_node = this->context->getSingleElement(node, ".//mdef:Arms");
        //~ if (arm_node.exists()) {
            //~ this->Arm = new PharmML::Arm(this->context, arm_node);
        //~ }
    }

    PharmML::ExternalDataset *TrialDesign::getExternalDataset() {
        return this->ExternalDataset;
    }

    PharmML::Intervention *TrialDesign::getIntervention() {
        return this->Intervention;
    }

    //~ PharmML::Observation *TrialDesign::getObservation() {
        //~ return this->Observation;
    //~ }

    //~ PharmML::Arm *TrialDesign::getArm() {
        //~ return this->Arm;
    //~ }
}
