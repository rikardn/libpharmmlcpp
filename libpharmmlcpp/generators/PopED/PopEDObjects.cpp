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
#include <visitors/AstAnalyzer.h>
#include <AST/AstTransformation.h>

namespace pharmmlcpp
{
    PopEDObjects::PopEDObjects() {
        this->xt_formatter.openVector("xt = list()", 1, ", ");
        this->a_formatter.openVector("a = list()", 1, ", ");
        this->has_infusions = false;
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

    // Generates doseNames, timeNames and code from IndividualAdministration
    std::string PopEDObjects::generateIndividualAdministration(IndividualAdministration *individualAdministration) {
        Dataset *ds = individualAdministration->getDataset();
        DataColumn *idv_col = ds->getIdvColumn();
        DataColumn *dose_col = ds->getColumnFromType("dose");
        std::vector<std::shared_ptr<AstNode>> idv_data = idv_col->getData();
        std::vector<std::shared_ptr<AstNode>> dose_data = dose_col->getData();

        TextFormatter formatter;
        formatter.openVector("c()", 0, ", ");
        for (std::vector<AstNode *>::size_type i = 0; i != idv_data.size(); i++) {
            this->rast.acceptRoot(dose_data[i].get());
            std::string dose_name = "DOSE_" + std::to_string(i + 1) + "_AMT";
            std::string time_name = "DOSE_" + std::to_string(i + 1) + "_TIME";
            this->doseNames.push_back(dose_name);      // Needed for the placebo arm and elsewhere
            this->timeNames.push_back(time_name);
            formatter.add(dose_name + "=" + this->rast.getValue());
            this->rast.acceptRoot(idv_data[i].get());
            formatter.add(time_name + "=" + this->rast.getValue());
        }
        formatter.closeVector();
        formatter.noFinalNewline();
        return formatter.createString();
    }

    std::string PopEDObjects::generateAdministration(Administration *administration) {
        std::vector<std::shared_ptr<AstNode>> amounts = administration->getAmountAsVector();
        std::vector<std::shared_ptr<AstNode>> times = administration->getTimesAsVector();

        if (this->doseNames.size() == 0) {      // First visit will get dose names
            for (std::vector<AstNode *>::size_type i = 1; i <= amounts.size(); i++) {
                this->doseNames.push_back("DOSE_" + std::to_string(i) + "_AMT");
                this->timeNames.push_back("DOSE_" + std::to_string(i) + "_TIME");
            }
        }

        TextFormatter formatter;
        formatter.openVector("c()", 0, ", ");

        for (std::vector<AstNode *>::size_type i = 0; i < amounts.size(); i++) {
            this->rast.acceptRoot(amounts[i].get());
            formatter.add(this->doseNames[i] + "=" + this->rast.getValue());
            this->rast.acceptRoot(times[i].get());
            formatter.add(this->timeNames[i] + "=" + this->rast.getValue());
        }

        formatter.closeVector();
        formatter.noFinalNewline();

        return formatter.createString();
    }

    std::vector<std::string> PopEDObjects::getDoseNames() {
        return this->doseNames;
    }

    std::vector<std::string> PopEDObjects::getTimeNames() {
        return this->timeNames;
    }

    bool PopEDObjects::hasInfusions() {
        return this->has_infusions;
    }
            
    bool PopEDObjects::hasBoluses() {
        return this->has_boluses;
    }

    std::shared_ptr<AstNode> PopEDObjects::getCombinationStart() {
        return this->combination_start;
    }

    std::shared_ptr<AstNode> PopEDObjects::getInterventionStart() {
        return this->intseq_start;
    }

    std::vector<std::string> PopEDObjects::getInfFuncCalls() {
        return this->infFuncCalls;
    }

    std::unordered_map<Symbol *, std::vector<std::string>> PopEDObjects::getInfusionMap() {
        return this->infusionMap;
    }

    std::vector<std::shared_ptr<AstNode>> PopEDObjects::getDoseTimes() {
        return this->doseTimes;
    }

    int PopEDObjects::getNumObservations() {
        return this->numObservations;
    }

    std::unordered_map<Symbol *, std::vector<std::shared_ptr<AstNode>>> PopEDObjects::getBolusAmounts() {
        return this->bolusAmounts;
    }

    std::unordered_map<Symbol *, std::vector<std::shared_ptr<AstNode>>> PopEDObjects::getBolusTimes() {
        return this->bolusTimes;
    }

    void PopEDObjects::visit(Arm *object) {
        std::vector<ObservationSequence *> obs_seqs = object->getObservationSequences();

        for (ObservationSequence *obs_seq : obs_seqs) {
            std::vector<ObjectRef *> obj_refs = obs_seq->getOidRefs();
            for (ObjectRef *obj_ref : obj_refs) {
                Object *obj = obj_ref->getObject();
                obj->accept(this);
                xt_formatter.add(this->getValue());
                this->numObservations++;
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
            this->intseq_start = AstTransformation::toVector(int_seq->getStart())[0];
        }
    }

    void PopEDObjects::visit(Administration *object) {
        // Check if this contains infusion (requires infusion function output)
        if (object->getType() == "Infusion") {
            this->has_infusions = true;
            std::string inf_func_call;
            inf_func_call = object->getOid() + " <- inf_func(offset + ";
            this->rast.acceptRoot(object->getTimesAsVector()[0].get());
            inf_func_call += this->rast.getValue();
            inf_func_call += ", ";
            this->rast.acceptRoot(object->getAmount().get());
            inf_func_call += this->rast.getValue();
            inf_func_call += ", Time, ";

            if (object->getRate()) {
                this->rast.acceptRoot(object->getRate().get());
                inf_func_call += "rate=" + this->rast.getValue();
            } else {    // Have duration
                this->rast.acceptRoot(object->getDuration().get());
                inf_func_call += "duration=" + this->rast.getValue();
            }
            inf_func_call += ")";

            this->infFuncCalls.push_back(inf_func_call);

            // Add to infusionMap for future sum of all infusions to same targets
            // FIXME: The TargetMap retreival is so ugly that my eyes hurt! Also need convenience for case of pure SymbRef instead of TargetMapping
            Symbol *target = object->getTargetMapping()->getMaps()[0].symbol;
            this->infusionMap[target].push_back(object->getOid());

            auto time_vector = object->getTimesAsVector();
            this->doseTimes.insert(this->doseTimes.end(), time_vector.begin(), time_vector.end());

            return;
        } else {
            this->has_boluses = true;
            // FIXME: This is material for library method: Find all interesting boluses with adm => (amounts, times). On Arm? combination? administration?
            Symbol *target = object->getTargetMapping()->getMaps()[0].symbol;
            std::shared_ptr<AstNode> single_amount = object->getAmount();
            std::vector<std::shared_ptr<AstNode>> times = object->getTimesAsVector();
            for (std::shared_ptr<AstNode> time_node : times) {
                this->bolusTimes[target].push_back(time_node);
                this->bolusAmounts[target].push_back(single_amount);
            }
        }

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
            setValue(this->generateAdministration(object));
        }
    }

    void PopEDObjects::visit(InterventionsCombination *object) {
        SingleIntervention *singleIntervention = object->getSingleInterventions()[0];     // FIXME: Assume one and only one

        // Get the start offset for the combination
        std::shared_ptr<AstNode> start = AstTransformation::toVector(singleIntervention->getStart())[0];    // Assume only one. FIXME: What would more mean?
        this->combination_start = start;

        // Handle the first single intervention
        for (ObjectRef *objectRef : singleIntervention->getOidRefs()) {
            objectRef->getObject()->accept(this);
        }
    }

    void PopEDObjects::visit(Observation *object) {
        TextFormatter formatter;
        formatter.openVector("c()", 0, ", ");

        for (std::shared_ptr<AstNode> time_point : object->getObservationTimesAsVector()) {
            this->rast.acceptRoot(time_point.get());
            formatter.add(this->rast.getValue());
        }

        formatter.closeVector();
        formatter.noFinalNewline();
        this->setValue(formatter.createString());
    }

    // Visit IndividualObservations to generate a vector from the Dataset
    void PopEDObjects::visit(IndividualObservations *object) {
        Dataset *ds = object->getDataset();
        DataColumn *col = ds->getIdvColumn();
        std::vector<std::shared_ptr<AstNode>> data = col->getData();

        TextFormatter formatter;
        formatter.openVector("c()", 0, ", ");
        for (std::shared_ptr<AstNode> node : data) {
            this->rast.acceptRoot(node.get());
            formatter.add(this->rast.getValue());
        }
        formatter.closeVector();
        formatter.noFinalNewline();
        this->setValue(formatter.createString());
    }
}
