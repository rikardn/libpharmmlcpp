#ifndef PHARMML_TrialDesign_H_
#define PHARMML_TrialDesign_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "PharmMLContext.h"
#include "Intervention.h"
#include "ExternalDataset.h"
//~ #include "Observation.h"
//~ #include "Arm.h"

namespace PharmML
{
    class TrialDesign
    {
        PharmML::PharmMLContext *context;
        PharmML::Intervention *Intervention = nullptr;
        PharmML::ExternalDataset *ExternalDataset = nullptr;
        //~ std::vector<PharmML::Observation *> Observations;
        //~ PharmML::Arm *Arm = nullptr;

        public:
        PharmML::ExternalDataset *getExternalDataset();
        PharmML::Intervention *getIntervention();
        //~ PharmML::Observation *getObservation();
        //~ PharmML::Arm *getArm();
        TrialDesign(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
    };
}

#endif
