#ifndef PHARMML_TrialDesign_H_
#define PHARMML_TrialDesign_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "PharmMLContext.h"
#include "ExternalDataset.h"
#include "Intervention.h"
#include "Observations.h"
#include "Arms.h"

namespace PharmML
{
    class TrialDesign
    {
        PharmML::PharmMLContext *context;
        PharmML::Intervention *Intervention = nullptr;
        PharmML::ExternalDataset *ExternalDataset = nullptr;
        PharmML::Observations *Observations = nullptr;
        PharmML::Arms *Arms = nullptr;

        public:
        PharmML::ExternalDataset *getExternalDataset();
        PharmML::Intervention *getIntervention();
        PharmML::Observations *getObservations();
        PharmML::Arms *getArms();
        TrialDesign(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
    };
}

#endif
