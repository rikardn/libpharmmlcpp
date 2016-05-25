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

namespace PharmML
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
            this->target_symbref = new PharmML::SymbRef(symbref);
        } else if (mapping.exists()) {
            this->target_mapping = new PharmML::TargetMapping(this->context, mapping);
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

    void Administration::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void Administration::accept(ObjectVisitor *visitor) {
        visitor->visit(this);
    }

    // IndividualAdministration class
    IndividualAdministration::IndividualAdministration(PharmML::PharmMLContext *context, xml::Node node) {
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
            PharmML::ColumnMapping *map = new PharmML::ColumnMapping(this->context, map_node);
            this->columnMappings.push_back(map);
        }

        // Get dataset
        xml::Node ds_node = this->context->getSingleElement(node, "./ds:DataSet");
        PharmML::Dataset *ds = new PharmML::Dataset(this->context, ds_node);
        this->dataset = ds;
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

    // Interventions class
    Interventions::Interventions(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void Interventions::parse(xml::Node node) {
        // Get administrations (treatments)
        std::vector<xml::Node> adm_nodes = this->context->getElements(node, "./design:Administration");
        for (xml::Node node : adm_nodes) {
            PharmML::Administration *adm = new PharmML::Administration(this->context, node);
            this->administrations.push_back(adm);
        }

        // Get individual administrations (time-dependent administration information on subject level)
        std::vector<xml::Node> ind_adm_nodes = this->context->getElements(node, "./design:IndividualAdministration");
        for (xml::Node node : ind_adm_nodes) {
            PharmML::IndividualAdministration *adm = new PharmML::IndividualAdministration(this->context, node);
            this->individualAdministrations.push_back(adm);
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

    void Interventions::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
