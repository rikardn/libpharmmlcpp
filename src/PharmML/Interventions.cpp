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

#include "Interventions.h"
#include <iostream>
#include <AST/AstTransformation.h>

namespace pharmmlcpp
{
    // TODO: Maybe move this somewhere? Variable.cpp (this is a reference, however)?
    // TODO: Maybe the name can be changed to something more general if needed
    SteadyStateParameter::SteadyStateParameter(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void SteadyStateParameter::parse(xml::Node node) {
        xml::Node symbol = this->context->getSingleElement(node, ".//ct:SymbRef");
        symbRef = new SymbRef(symbol);

        xml::Node assign = this->context->getSingleElement(node, ".//ct:Assign");
        if (assign.exists()) {
            xml::Node tree = assign.getChild();
            this->assignment = this->context->factory.create(tree);
        }
    }

    AstNode *SteadyStateParameter::getAssignment() {
        return this->assignment;
    }

    AstNode *SteadyStateParameter::getSymbRef() {
        return this->symbRef;
    }

    void SteadyStateParameter::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    // Administration class
    Administration::Administration(PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
    }

    void Administration::parse(xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        xml::Node dose = node.getChild();
        this->type = dose.getName();

        // Get dose amount (pure Assign)
        xml::Node amount = this->context->getSingleElement(dose, "./design:DoseAmount");
        xml::Node assign = this->context->getSingleElement(dose, "./design:DoseAmount/ct:Assign");
        if (assign.exists()) {
            xml::Node tree = assign.getChild();
            this->amount = this->context->factory.create(tree);
        }

        // Get dose target (TargetMapping or SymbRef)
        this->target_type = amount.getAttribute("inputTarget").getValue(); // Can be "parameter", "derivativeVariable", "variable" and "admType"
        xml::Node symbref = this->context->getSingleElement(dose, "./design:DoseAmount/ct:SymbRef");
        xml::Node mapping = this->context->getSingleElement(dose, "./design:DoseAmount/design:TargetMapping");
        if (symbref.exists()) {
            this->target_symbref = new pharmmlcpp::SymbRef(symbref);
        } else if (mapping.exists()) {
            this->target_mapping = new pharmmlcpp::TargetMapping(this->context, mapping);
        }

        // Get dose times/steady state
        xml::Node times = this->context->getSingleElement(dose, "./design:DosingTimes");
        xml::Node steady = this->context->getSingleElement(dose, "./design:SteadyState");
        if (times.exists()) {
            xml::Node assign = this->context->getSingleElement(times, "./ct:Assign");
            xml::Node tree = assign.getChild();
            this->times = this->context->factory.create(tree);
        } else if (steady.exists()) {
            this->steady = new SteadyStateParameter(this->context, steady);
        }

        // Get duration/rate for infusion type
        if (this->type == "Infusion") {
            xml::Node duration = this->context->getSingleElement(dose, "./design:duration");
            xml::Node rate = this->context->getSingleElement(dose, "./design:rate");
            if (duration.exists()) {
                // TODO: Support <Duration>
                this->duration = nullptr;
            } else if (rate.exists()) {
                // TODO: Support <Rate>
                this->rate = nullptr;
            }
        }
    }

    xml::Node Administration::xml() {
        xml::Node adm("Administration");
        adm.setAttribute("oid", this->oid);
        xml::Node type = adm.createChild(this->type);
        xml::Node da = type.createChild("DoseAmount");
        da.setAttribute("inputTarget", this->target_type);
        if (this->target_mapping) {
            // TODO: Support TargetMapping
        }
        if (this->target_symbref) {
            XMLAstVisitor xml;
            this->target_symbref->accept(&xml);
            da.addChild(xml.getValue());
        }
        if (this->amount) {
            XMLAstVisitor xml;
            this->amount->accept(&xml);
            da.addChild(xml.getValue());
        }
        return adm;
    }

    std::string Administration::getType() {
        return this->type;
    }

    std::string Administration::getTargetType() {
        return this->target_type;
    }

    AstNode *Administration::getAmount() {
        return this->amount;
    }

    std::vector<AstNode *> Administration::getAmountAsVector() {
        return AstTransformation::toVector(this->amount);
    }

    std::vector<AstNode *> Administration::getTimesAsVector() {
        return AstTransformation::toVector(this->times);
    }

    TargetMapping *Administration::getTargetMapping() {
        return this->target_mapping;
    }

    SymbRef *Administration::getTargetSymbRef() {
        return this->target_symbref;
    }

    AstNode *Administration::getTimes() {
        return this->times;
    }

    AstNode *Administration::getSteady() {
        return this->steady;
    }

    AstNode *Administration::getDuration() {
        return this->duration;
    }

    AstNode *Administration::getRate() {
        return this->rate;
    }

    void Administration::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        if (this->getTargetSymbRef()) {
            this->setupAstSymbRefs(this->getTargetSymbRef(), gathering, blkId);
        }
    }

    void Administration::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void Administration::accept(ObjectVisitor *visitor) {
        visitor->visit(this);
    }

    // IndividualAdministration class
    IndividualAdministration::IndividualAdministration(pharmmlcpp::PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
    }

    void IndividualAdministration::parse(xml::Node node) {
        // Get intervention (oid) reference (for which individual times/amounts will be defined)
        xml::Node ref_node = this->context->getSingleElement(node, "./design:InterventionRef");
        if (ref_node.exists()) {
            this->oidRef = new ObjectRef(ref_node);
        }

        // Get column mappings
        std::vector<xml::Node> map_nodes = this->context->getElements(node, "./design:ColumnMapping");
        for (xml::Node map_node : map_nodes) {
            pharmmlcpp::ColumnMapping *map = new pharmmlcpp::ColumnMapping(this->context, map_node);
            this->columnMappings.push_back(map);
        }

        // Get dataset
        xml::Node ds_node = this->context->getSingleElement(node, "./ds:DataSet");
        pharmmlcpp::Dataset *ds = new pharmmlcpp::Dataset(this->context, ds_node);
        this->dataset = ds;

        // Check that all individual administrations have an independent variable and a dose column
        pharmmlcpp::DataColumn *idv_col = ds->getIdvColumn();
        if (!idv_col) {     // No idv column was found
            this->context->logger.error("Missing idv column in IndividualAdministration", this);
        }
        pharmmlcpp::DataColumn *dose_col = ds->getColumnFromType("dose");
        if (!dose_col) {
            this->context->logger.error("Missing dose column in IndividualAdministration", this);
        }
    }

    void IndividualAdministration::gatherObjectRefs(std::unordered_map<std::string, Object *> &oidMap) {
        if (this->oidRef) {
            this->oidRef->setObject(oidMap[this->oidRef->getOidRef()]);
        }
    }

    xml::Node IndividualAdministration::xml() {
        xml::Node ia("IndividualAdministration");
        xml::Node iref = ia.createChild("InterventionRef");
        iref.setAttribute("oidRef", this->oidRef->getOidRef());
        for (ColumnMapping *cm : this->columnMappings) {
            ia.addChild(cm->xml());
        }
        ia.addChild(this->dataset->xml());
        return ia;
    }

    ObjectRef *IndividualAdministration::getOidRef() {
        return this->oidRef;
    }

    std::vector<ColumnMapping *> IndividualAdministration::getColumnMappings() {
        return this->columnMappings;
    }

    Dataset *IndividualAdministration::getDataset() {
        return this->dataset;
    }

    void IndividualAdministration::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    // SingleIntervention
    SingleIntervention::SingleIntervention(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void SingleIntervention::parse(xml::Node node) {
        std::vector<xml::Node> interventionref_nodes = this->context->getElements(node, "./design:InterventionRef");
        for (xml::Node interventionref_node: interventionref_nodes) {
            ObjectRef *ref = new ObjectRef(interventionref_node);
            this->oidRefs.push_back(ref);
        }
        xml::Node start_node = this->context->getSingleElement(node, "./design:Start");
        if (start_node.exists()) {
            this->start = this->context->factory.create(start_node.getChild().getChild());
        }
        xml::Node end_node = this->context->getSingleElement(node, "./design:End");
        if (end_node.exists()) {
            this->end = this->context->factory.create(end_node.getChild().getChild());
        }
    }

    std::vector<ObjectRef *> SingleIntervention::getOidRefs() {
        return this->oidRefs;
    }

    AstNode *SingleIntervention::getStart() {
        return this->start;
    }

    AstNode *SingleIntervention::getEnd() {
        return this->end;
    }

    // InterventionsCombination
    InterventionsCombination::InterventionsCombination(PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
    }
    
    void InterventionsCombination::parse(xml::Node node) {
        this->Object::parse(node);
        xml::Node relative_node = this->context->getSingleElement(node, "./design:Relative");
        if (relative_node.exists()) {
            this->relative = this->context->factory.create(relative_node.getChild());
        }
        std::vector<xml::Node> interventions_nodes = this->context->getElements(node, "./design:Interventions");
        for (xml::Node intervention_node : interventions_nodes) {
            SingleIntervention *intervention = new SingleIntervention(this->context, intervention_node);
            this->singleInterventions.push_back(intervention);
        }
    }

    std::vector<SingleIntervention *> InterventionsCombination::getSingleInterventions() {
        return this->singleInterventions;
    }

    AstNode *InterventionsCombination::getRelative() {
        return this->relative;
    }

    void InterventionsCombination::gatherObjectRefs(std::unordered_map<std::string, Object *> &oidMap) {
        for (SingleIntervention *si : this->singleInterventions) {
            for (ObjectRef *ref : si->getOidRefs()) {
                ref->setObject(oidMap[ref->getOidRef()]);
            }
        }
    }

    void InterventionsCombination::accept(ObjectVisitor *visitor) {
        visitor->visit(this);
    }

    // Interventions class
    Interventions::Interventions(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void Interventions::parse(xml::Node node) {
        // Get administrations (treatments)
        std::vector<xml::Node> adm_nodes = this->context->getElements(node, "./design:Administration");
        for (xml::Node node : adm_nodes) {
            pharmmlcpp::Administration *adm = new pharmmlcpp::Administration(this->context, node);
            this->administrations.push_back(adm);
        }

        // Get individual administrations (time-dependent administration information on subject level)
        std::vector<xml::Node> ind_adm_nodes = this->context->getElements(node, "./design:IndividualAdministration");
        for (xml::Node node : ind_adm_nodes) {
            pharmmlcpp::IndividualAdministration *adm = new pharmmlcpp::IndividualAdministration(this->context, node);
            this->individualAdministrations.push_back(adm);
        }

        // Get interventions combinations
        std::vector<xml::Node> int_comb_nodes = this->context->getElements(node, "./design:InterventionsCombination");
        for (xml::Node node : int_comb_nodes) {
            InterventionsCombination *comb = new InterventionsCombination(this->context, node);
            this->interventionsCombinations.push_back(comb);
        }
    }

    xml::Node Interventions::xml() {
        xml::Node inter("Interventions");
        for (Administration *adm : this->administrations) {
            inter.addChild(adm->xml());
        }
        for (IndividualAdministration *iadm : this->individualAdministrations) {
            inter.addChild(iadm->xml());
        }
        return inter;
    }

    std::vector <Administration *> Interventions::getAdministrations() {
        return administrations;
    }

    std::vector <IndividualAdministration *> Interventions::getIndividualAdministrations() {
        return individualAdministrations;
    }

    std::vector<InterventionsCombination *> Interventions::getInterventionsCombinations() {
        return this->interventionsCombinations;
    }

    void Interventions::setupRefererSymbRefs(SymbolGathering &gathering) {
        for (Administration *adm : this->getAdministrations()) {
            adm->setupSymbRefs(gathering, "");      // Default to global namespace
        }
    }

    void Interventions::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
