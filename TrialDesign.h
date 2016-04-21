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

#ifndef PHARMML_TRIALDESIGN_H_
#define PHARMML_TRIALDESIGN_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "PharmMLContext.h"
#include "ExternalDataset.h"
#include "Interventions.h"
#include "Observations.h"
#include "Arms.h"
#include "DesignSpaces.h"

namespace PharmML
{
    class TrialDesign
    {
        PharmML::PharmMLContext *context;
        PharmML::Interventions *Interventions = nullptr;
        PharmML::ExternalDataset *ExternalDataset = nullptr;
        PharmML::Observations *Observations = nullptr;
        PharmML::Arms *Arms = nullptr;
        PharmML::DesignSpaces *DesignSpaces = nullptr;

        public:
        PharmML::ExternalDataset *getExternalDataset();
        PharmML::Interventions *getInterventions();
        PharmML::Observations *getObservations();
        PharmML::Arms *getArms();
        PharmML::DesignSpaces *getDesignSpaces();
        TrialDesign(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
    };
}

#endif