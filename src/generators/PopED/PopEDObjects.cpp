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

namespace PharmML
{
    PopEDObjects::PopEDObjects() {
        this->xt_formatter.openVector("xt = list()", 0, ", ");
    }

    std::string PopEDObjects::getDatabaseAdditions() {
        xt_formatter.closeVector();
        return xt_formatter.createString();
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
