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
    
    // TODO: Move elsewhere (Dataset.h when implemented)
    // TODO: Subclass for MapType? Subclass for SymbRef (see usage in Administration)?
    TargetMapping::TargetMapping(PharmMLContext *context, std::string type, xml::Node node) {
        this->context = context;
        this->type = type;
        this->blkIdRef = node.getAttribute("blkIdRef").getValue();
        xml::Node map = this->context->getSingleElement(node, ".//ds:Map");
        // Not sure type differentiates attribute used in MapType (only empirical observation)
        if (type == "parameter") {
            // Not seen example of but in schema
        } else if (type == "derivativeVaribel") {
            this->ref = map.getAttribute("modelSymbol").getValue();
        } else if (type == "variable") {
            // Not seen example of but in schema
        } else if (type == "admType") {
            this->ref = map.getAttribute("admNumber").getValue();
        }
        // Not sure when used in MapType
        //~ this->ref = map.getAttribute("dataSymbol").getValue();
    }
    
    std::string TargetMapping::getType() {
        return this->type;
    }
    
    std::string TargetMapping::getBlkIdRef() {
        return this->blkIdRef;
    }
    std::string TargetMapping::getRef() {
        return this->ref;
    }
    
    void TargetMapping::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }
    
    // Administration class
    Administration::Administration(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->xml_node = node;
        this->parse(node);
    }
   
    void Administration::update() {
        xml::Node adm("Administration");
        adm.setAttribute("oid", this->oid);
        xml::Node type = adm.createChild(this->type);
        this->xml_node.replaceNode(adm);
    }

    void Administration::parse(xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        xml::Node dose = node.getChild();
        this->type = dose.getName();

        // Get dose amount
        xml::Node amount = this->context->getSingleElement(dose, ".//design:DoseAmount");
        xml::Node assign = this->context->getSingleElement(dose, ".//design:DoseAmount/ct:Assign");
        if (assign.exists()) {
            xml::Node tree = assign.getChild();
            this->amount = this->context->factory.create(tree);
        }

        // Get dose target ('target' should probably be of parent class of SymbRef/TargetMapping).
        std::string targetType = amount.getAttribute("inputTarget").getValue();
        xml::Node symbref = this->context->getSingleElement(dose, ".//design:DoseAmount/ct:SymbRef");
        xml::Node mapping = this->context->getSingleElement(dose, ".//design:DoseAmount/design:TargetMapping");
        if (symbref.exists()) {
            this->target = new SymbRef(symbref);
        } else if (mapping.exists()) {
            this->target = new TargetMapping(this->context, targetType, mapping);
        }

        // Get dose times/steady state
        xml::Node times = this->context->getSingleElement(dose, ".//design:DosingTimes");
        xml::Node steady = this->context->getSingleElement(dose, ".//design:SteadyState");
        if (times.exists()) {
            xml::Node assign = this->context->getSingleElement(times, ".//ct:Assign");
            xml::Node tree = assign.getChild();
            this->times = this->context->factory.create(tree);
        } else if (steady.exists()) {
            this->steady = new SteadyStateParameter(this->context, steady);
        }
        
        // Get duration/rate for infusion type
        if (this->type == "Infusion") {
            xml::Node duration = this->context->getSingleElement(dose, ".//design:duration");
            xml::Node rate = this->context->getSingleElement(dose, ".//design:rate");
            if (duration.exists()) {
                // TODO: Support <Duration>
                this->duration = nullptr;
            } else if (rate.exists()) {
                // TODO: Support <Rate>
                this->rate = nullptr;
            }
        }
    }
    
    std::string Administration::getOid() {
        return this->oid;
    }
    
    std::string Administration::getType() {
        return this->type;
    }
    
    AstNode *Administration::getTarget() {
        return this->target;
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
    
    // Interventions class
    Interventions::Interventions(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }
    
    void Interventions::parse(xml::Node node) {
        std::vector<xml::Node> array = this->context->getElements(node, ".//design:Administration");
        for (xml::Node n : array) {
            PharmML::Administration *adm = new PharmML::Administration(this->context, n);
            this->Administrations.push_back(adm);
        }
    }
    
    std::vector <Administration *> Interventions::getAdministrations() {
        return Administrations;
    }
    
    void Interventions::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
