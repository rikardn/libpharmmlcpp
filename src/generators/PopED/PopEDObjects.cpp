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

#include <generators/PopED/PopEDObjects.h>
#include <vector>
#include <PharmML/Arms.h>
#include <PharmML/Observations.h>
#include <iostream>

namespace PharmML
{
    PopEDObjects::PopEDObjects() {
        this->xt_formatter.openVector("xt = list()", 1, ", ");
        this->a_formatter.openVector("a = list()", 1, ", ");
    }

    TextFormatter& PopEDObjects::getDatabaseXT() {
        xt_formatter.closeVector();
        return xt_formatter;
    }

    TextFormatter& PopEDObjects::getDatabaseA() {
        a_formatter.closeVector();
        return a_formatter;
    }

    void PopEDObjects::setIndividualAdministrations(std::vector<IndividualAdministration *> individualAdministrations) {
        this->individualAdministrations = individualAdministrations;
    }

    std::string PopEDObjects::generateIndividualAdministration(IndividualAdministration *individualAdministration) {
        Dataset *ds = individualAdministration->getDataset();
        DataColumn *idv_col = ds->getIdvColumn();
        DataColumn *dose_col = ds->getColumnFromType("dose");
        std::vector<AstNode *> idv_data = idv_col->getData();
        std::vector<AstNode *> dose_data = dose_col->getData();

        TextFormatter formatter;
        formatter.openVector("c()", 0, ", ");
        for (std::vector<AstNode *>::size_type i = 0; i != idv_data.size(); i++) {
            dose_data[i]->accept(&this->rast);
            std::string dose_name = "DOSE_" + std::to_string(i) + "_AMT";
            std::string time_name = "DOSE_" + std::to_string(i) + "_TIME";
            this->doseNames.push_back(dose_name);      // Needed for the placebo arm
            this->timeNames.push_back(time_name);
            formatter.add(dose_name + "=" + this->rast.getValue());
            idv_data[i]->accept(&this->rast);
            formatter.add(time_name + "=" + this->rast.getValue());
        }
        formatter.closeVector();
        formatter.noFinalNewline();
        return formatter.createString();
    }

    void PopEDObjects::visit(Arm *object) {
        std::vector<ObservationSequence *> obs_seqs = object->getObservationSequences();

        for (ObservationSequence *obs_seq : obs_seqs) {
            std::vector<ObjectRef *> obj_refs = obs_seq->getOidRefs();
            for (ObjectRef *obj_ref : obj_refs) {
                Object *obj = obj_ref->getObject();
                obj->accept(this);
                xt_formatter.add(this->getValue());
            }
        }

        std::vector<InterventionSequence *> int_seqs = object->getInterventionSequences();

        if (int_seqs.size() == 0) {     // No intervention i.e. placebo. Currently assumes the a[1] = ..AMT, a[2] = ..TIME structure
            TextFormatter placebo_formatter;
            placebo_formatter.openVector("c()", 0, ", ");
   
            for (std::vector<AstNode *>::size_type i = 0; i != this->doseNames.size(); i++) {
                placebo_formatter.add(this->doseNames[i] + "=0");
                placebo_formatter.add(this->timeNames[i] + "=0");
            }

            placebo_formatter.closeVector();
            placebo_formatter.noFinalNewline();
            a_formatter.add(placebo_formatter.createString());
        }

        for (InterventionSequence *int_seq : int_seqs) {
            std::vector<ObjectRef *> obj_refs = int_seq->getOidRefs();
            for (ObjectRef *obj_ref : obj_refs) {
                Object *obj = obj_ref->getObject();
                obj->accept(this);
                if (!this->getValue().empty()) {
                    a_formatter.add(this->getValue());
                }
            }
        }
    }

    void PopEDObjects::visit(Administration *object) {
        // Check if this is being refered to by an IndividualAdministration
        IndividualAdministration *found_ia = nullptr;
        for (IndividualAdministration *ia : this->individualAdministrations) {
            if (ia->getOidRef()->getObject() == object) {
                found_ia = ia;
            }
        }

        if (found_ia) {
            setValue(this->generateIndividualAdministration(found_ia));
        } else {
            setValue("");
        }
    }

    // Visit IndividualObservations to generate a vector from the Dataset
    void PopEDObjects::visit(IndividualObservations *object) {
        Dataset *ds = object->getDataset();
        DataColumn *col = ds->getIdvColumn();
        std::vector<AstNode *> data = col->getData();

        TextFormatter formatter;
        formatter.openVector("c()", 0, ", ");
        for (AstNode *node : data) {
            node->accept(&this->rast);
            formatter.add(this->rast.getValue());
        }
        formatter.closeVector();
        formatter.noFinalNewline();
        this->setValue(formatter.createString());
    }
}