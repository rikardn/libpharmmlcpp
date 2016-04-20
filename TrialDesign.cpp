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
        
        xml::Node ds_node = this->context->getSingleElement(node, ".//design:DesignSpaces");
        if (ds_node.exists()) {
            this->DesignSpaces = new PharmML::DesignSpaces(this->context, ds_node);
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
    
    PharmML::DesignSpaces *TrialDesign::getDesignSpaces() {
        return this->DesignSpaces;
    }
}
