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

#ifndef PHARMMLCPP_TRIALDESIGN_H_
#define PHARMMLCPP_TRIALDESIGN_H_

#include <vector>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <PharmML/PharmMLContext.h>
#include <PharmML/ExternalDataset.h>
#include <PharmML/Interventions.h>
#include "Observations.h"
#include "Arms.h"
#include "DesignSpaces.h"
#include <symbols/Variable.h>

namespace pharmmlcpp
{
    class TrialDesign
    {
        public:
            TrialDesign(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            void update();
            std::vector<ExternalDataset *> getExternalDatasets();
            Interventions *getInterventions();
            Observations *getObservations();
            Arms *getArms();
            DesignSpaces *getDesignSpaces();
            std::vector<DesignParameter *> getDesignParameters();

            SymbolSet getOptimizationParameters();

            void gatherSymbols(SymbolGathering &gatherer);
            void setupRefererSymbRefs(SymbolGathering &gatherer);
            void setupTargetMappings(SymbolGathering &gatherer);
            void setupTargetMappings(MacroGathering &gatherer);

        private:
            pharmmlcpp::PharmMLContext *context;
            xml::Node xml_node;
            Interventions *interventions = nullptr;
            std::vector<ExternalDataset *> externalDatasets;
            Observations *observations = nullptr;
            Arms *arms = nullptr;
            DesignSpaces *designSpaces = nullptr;
            std::vector<DesignParameter *> designParameters;
    };
}

#endif
