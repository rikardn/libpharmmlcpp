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
        xml::Node intervention_node = this->context->getSingleElement(node, ".//design:Interventions");
        if (intervention_node.exists()) {
            this->Intervention = new PharmML::Intervention(this->context, intervention_node);
        }
        
        //~ std::vector<xml::Node> array = this->context->getElements(node, ".//design:Observations/design:Observation");
        //~ for (xml::Node node : array) {
            //~ PharmML::Observation *obs = new PharmML::Observation(this->context, node);
            //~ this->Observations.push_back(obs);
        //~ }
        //~ xml::Node arm_node = this->context->getSingleElement(node, ".//mdef:Arms");
        //~ if (arm_node.exists()) {
            //~ this->Arm = new PharmML::Arm(this->context, arm_node);
        //~ }
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
