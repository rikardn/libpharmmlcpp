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
    SteadyStateParameter::SteadyStateParameter(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void SteadyStateParameter::parse(PharmMLReader &reader, xml::Node node) {
        xml::Node symbol = reader.getSingleElement(node, ".//ct:SymbRef");
        symbRef = new SymbRef(reader, symbol);

        xml::Node assign = reader.getSingleElement(node, ".//ct:Assign");
        if (assign.exists()) {
            xml::Node tree = assign.getChild();
            this->assignment = AstNode::create(reader, tree);
        }
    }

    xml::Node SteadyStateParameter::xml(PharmMLWriter &writer) {
        xml::Node ssp("SteadyStateParameter", xml::Namespace::ct);
            // FIXME: This is neither correct nor complete
        return ssp;
    }

    std::unique_ptr<AstNode> SteadyStateParameter::clone() {
        std::unique_ptr<SteadyStateParameter> cl;
        return std::move(cl);
    }

    std::shared_ptr<AstNode> SteadyStateParameter::getAssignment() {
        return this->assignment;
    }

    AstNode *SteadyStateParameter::getSymbRef() {
        return this->symbRef;
    }

    void SteadyStateParameter::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    // Administration class
    Administration::Administration(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->parse(reader, node);
    }

    void Administration::parse(PharmMLReader &reader, xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        xml::Node dose = node.getChild();
        this->type = dose.getName();

        // Get dose amount (pure Assign)
        xml::Node amount = reader.getSingleElement(dose, "./design:DoseAmount");
        xml::Node assign = reader.getSingleElement(dose, "./design:DoseAmount/ct:Assign");
        if (assign.exists()) {
            xml::Node tree = assign.getChild();
            this->amount = AstNode::create(reader, tree);
        }

        // Get dose target (TargetMapping or SymbRef)
        this->target_type = amount.getAttribute("inputTarget").getValue(); // Can be "parameter", "derivativeVariable", "variable" and "admType"
        xml::Node symbref = reader.getSingleElement(dose, "./design:DoseAmount/ct:SymbRef");
        xml::Node mapping = reader.getSingleElement(dose, "./design:DoseAmount/design:TargetMapping");
        if (symbref.exists()) {
            this->target_symbref = new SymbRef(reader, symbref);
        } else if (mapping.exists()) {
            this->target_mapping = new TargetMapping(reader, mapping);
        }

        // Get dose times/steady state
        xml::Node times = reader.getSingleElement(dose, "./design:DosingTimes");
        xml::Node steady = reader.getSingleElement(dose, "./design:SteadyState");
        if (times.exists()) {
            xml::Node assign = reader.getSingleElement(times, "./ct:Assign");
            xml::Node tree = assign.getChild();
            this->times = AstNode::create(reader, tree);
        } else if (steady.exists()) {
            this->steady = std::make_unique<SteadyStateParameter>(reader, steady);
        }

        // Get duration/rate for infusion type
        if (this->type == "Infusion") {
            xml::Node duration = reader.getSingleElement(dose, "./design:Duration");
            xml::Node rate = reader.getSingleElement(dose, "./design:Rate");
            if (duration.exists()) {
                // TODO: Support <Duration>
                this->duration = nullptr;
            } else if (rate.exists()) {
                // TODO: Support <Rate>
                this->rate = AstNode::create(reader, rate.getChild().getChild());
            }
        }
    }

    // FIXME: Not yet called
    void Administration::postParse() {
        if (this->type == "Bolus") {
            AstAnalyzer analyzer;
            this->amount->accept(&analyzer);
            int amount_length = analyzer.getLength();
            analyzer.reset();
            this->times->accept(&analyzer);
            int times_length = analyzer.getLength();
            if (times_length != 1 && amount_length != 1 && times_length != amount_length) {
               return; // FIXME: Error handling here
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

    std::shared_ptr<AstNode> Administration::getAmount() {
        return this->amount;
    }

    std::vector<std::shared_ptr<AstNode>> Administration::getAmountAsVector() {
        return AstTransformation::toVector(this->amount);
    }

    std::vector<std::shared_ptr<AstNode>> Administration::getTimesAsVector() {
        return AstTransformation::toVector(this->times);
    }

    TargetMapping *Administration::getTargetMapping() {
        return this->target_mapping;
    }

    SymbRef *Administration::getTargetSymbRef() {
        return this->target_symbref;
    }

    std::shared_ptr<AstNode> Administration::getTimes() {
        return this->times;
    }

    std::shared_ptr<AstNode> Administration::getSteady() {
        return this->steady;
    }

    std::shared_ptr<AstNode> Administration::getDuration() {
        return this->duration;
    }

    std::shared_ptr<AstNode> Administration::getRate() {
        return this->rate;
    }

    void Administration::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        if (this->getTargetSymbRef()) {
            this->setupAstSymbRefs(this->getTargetSymbRef(), gathering, blkId);
        }
        if (this->amount) {
            this->setupAstSymbRefs(this->amount.get(), gathering, blkId);
        }
        if (this->times) {
            this->setupAstSymbRefs(this->times.get(), gathering, blkId);
        }
    }

    void Administration::setupTargetMappings(SymbolGathering &gathering) {
        if (this->getTargetMapping()) {
            this->getTargetMapping()->setupSymbolRefs(gathering);
        }
    }

    void Administration::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void Administration::accept(ObjectVisitor *visitor) {
        visitor->visit(this);
    }

    // IndividualAdministration class
    IndividualAdministration::IndividualAdministration(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->parse(reader, node);
    }

    void IndividualAdministration::parse(PharmMLReader &reader, xml::Node node) {
        // Get intervention (oid) reference (for which individual times/amounts will be defined)
        xml::Node ref_node = reader.getSingleElement(node, "./design:InterventionRef");
        if (ref_node.exists()) {
            this->oidRef = new ObjectRef(ref_node);
        }

        // Get column mappings
        std::vector<xml::Node> map_nodes = reader.getElements(node, "./design:ColumnMapping");
        for (xml::Node map_node : map_nodes) {
            pharmmlcpp::ColumnMapping *map = new ColumnMapping(reader, map_node);
            this->columnMappings.push_back(map);
        }

        // Get dataset
        xml::Node ds_node = reader.getSingleElement(node, "./ds:DataSet");
        Dataset *ds = new Dataset(reader, ds_node);
        this->dataset = ds;

        // Check that all individual administrations have an independent variable and a dose column
        DataColumn *idv_col = ds->getIdvColumn();
        if (!idv_col) {     // No idv column was found
            reader.logger.error("Missing idv column in IndividualAdministration", this);
        }
        DataColumn *dose_col = ds->getColumnFromType("dose");
        if (!dose_col) {
            reader.logger.error("Missing dose column in IndividualAdministration", this);
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
    SingleIntervention::SingleIntervention(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void SingleIntervention::parse(PharmMLReader &reader, xml::Node node) {
        std::vector<xml::Node> interventionref_nodes = reader.getElements(node, "./design:InterventionRef");
        for (xml::Node interventionref_node: interventionref_nodes) {
            ObjectRef *ref = new ObjectRef(interventionref_node);
            this->oidRefs.push_back(ref);
        }
        xml::Node start_node = reader.getSingleElement(node, "./design:Start");
        if (start_node.exists()) {
            this->start = AstNode::create(reader, start_node.getChild().getChild());
        }
        xml::Node end_node = reader.getSingleElement(node, "./design:End");
        if (end_node.exists()) {
            this->end = AstNode::create(reader, end_node.getChild().getChild());
        }
    }

    std::vector<ObjectRef *> SingleIntervention::getOidRefs() {
        return this->oidRefs;
    }

    std::shared_ptr<AstNode> SingleIntervention::getStart() {
        return this->start;
    }

    std::shared_ptr<AstNode> SingleIntervention::getEnd() {
        return this->end;
    }

    // InterventionsCombination
    InterventionsCombination::InterventionsCombination(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->parse(reader, node);
    }

    void InterventionsCombination::parse(PharmMLReader &reader, xml::Node node) {
        this->Object::parse(node);
        xml::Node relative_node = reader.getSingleElement(node, "./design:Relative");
        if (relative_node.exists()) {
            this->relative = AstNode::create(reader, relative_node.getChild());
        }
        std::vector<xml::Node> interventions_nodes = reader.getElements(node, "./design:Interventions");
        for (xml::Node intervention_node : interventions_nodes) {
            SingleIntervention *intervention = new SingleIntervention(reader, intervention_node);
            this->singleInterventions.push_back(intervention);
        }
    }

    std::vector<SingleIntervention *> InterventionsCombination::getSingleInterventions() {
        return this->singleInterventions;
    }

    std::shared_ptr<AstNode> InterventionsCombination::getRelative() {
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
    Interventions::Interventions(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void Interventions::parse(PharmMLReader &reader, xml::Node node) {
        // Get administrations (treatments)
        std::vector<xml::Node> adm_nodes = reader.getElements(node, "./design:Administration");
        for (xml::Node node : adm_nodes) {
            Administration *adm = new Administration(reader, node);
            this->administrations.push_back(adm);
        }

        // Get individual administrations (time-dependent administration information on subject level)
        std::vector<xml::Node> ind_adm_nodes = reader.getElements(node, "./design:IndividualAdministration");
        for (xml::Node node : ind_adm_nodes) {
            IndividualAdministration *adm = new IndividualAdministration(reader, node);
            this->individualAdministrations.push_back(adm);
        }

        // Get interventions combinations
        std::vector<xml::Node> int_comb_nodes = reader.getElements(node, "./design:InterventionsCombination");
        for (xml::Node node : int_comb_nodes) {
            InterventionsCombination *comb = new InterventionsCombination(reader, node);
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

    void Interventions::setupTargetMappings(SymbolGathering &gathering) {
        for (Administration *adm : this->getAdministrations()) {
            adm->setupTargetMappings(gathering);
        }
    }

    void Interventions::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
