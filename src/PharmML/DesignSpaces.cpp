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

#include "DesignSpaces.h"
#include <iostream>

namespace PharmML
{
    DesignSpace::DesignSpace(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void DesignSpace::parse(xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();

        // Get (oid) references
        std::vector<xml::Node> refs = this->context->getElements(node, "./design:InterventionRef");
        for (xml::Node ref : refs) {
            this->interventionRefs.push_back(new ObjectRef(ref));
        }
        refs = this->context->getElements(node, "./design:ObservationRef");
        for (xml::Node ref : refs) {
            this->observationRefs.push_back(new ObjectRef(ref));
        }
        refs = this->context->getElements(node, "./design:ArmRef");
        for (xml::Node ref : refs) {
            this->armRefs.push_back(new ObjectRef(ref));
        }
        xml::Node dosing_times = this->context->getSingleElement(node, "./design:DosingTimes");
        if (dosing_times.exists()) {
            this->dosingTimes = this->context->factory.create(dosing_times.getChild().getChild());
        }
    }

    xml::Node DesignSpace::xml() {
        xml::Node ds("DesignSpace");
        for (ObjectRef *ref : this->interventionRefs) {
            xml::Node iref = ds.createChild("InterventionRef");
            iref.setAttribute("oidRef", ref->getOidRef());
        }
        if (this->dosingTimes) {
            xml::Node dt = ds.createChild("DosingTimes");
            xml::Node assign("Assign", xml::Namespace::ct);
            dt.addChild(assign);
            XMLAstVisitor xml;
            this->dosingTimes->accept(&xml);
            assign.addChild(xml.getValue());
        }
        return ds;
    }

    std::string DesignSpace::getOid() {
        return this->oid;
    }

    std::vector<ObjectRef *> DesignSpace::getInterventionRefs() {
        return this->interventionRefs;
    }

    std::vector<ObjectRef *> DesignSpace::getObservationRefs() {
        return this->observationRefs;
    }

    std::vector<ObjectRef *> DesignSpace::getArmRefs() {
        return this->armRefs;
    }

    AstNode *DesignSpace::getDosingTimes() {
        return this->dosingTimes;
    }

    void DesignSpace::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    DesignSpaces::DesignSpaces(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void DesignSpaces::parse(xml::Node node) {
        // Get design parameters
        // (mdef:DesignParameterType extends mdef:CommonParameterType which is close enough to class Variable for now)
        std::vector<xml::Node> design_parameters = this->context->getElements(node, "./mdef:DesignParameter");
        for (xml::Node node : design_parameters) {
            Variable *parameter = new Variable(this->context, node);
            this->designParameters.push_back(parameter);
        }

        // Get design spaces
        std::vector<xml::Node> designSpaces = this->context->getElements(node, "./design:DesignSpace");
        for (xml::Node node : designSpaces) {
            DesignSpace *space = new DesignSpace(this->context, node);
            this->designSpaces.push_back(space);
        }
    }

    xml::Node DesignSpaces::xml() {
        xml::Node ds("DesignSpaces");
        for (DesignSpace *space : this->designSpaces) {
            ds.addChild(space->xml());
        }
        return ds;
    }

    std::vector<Variable *> DesignSpaces::getDesignParameters() {
        return this->designParameters;
    }

    std::vector<DesignSpace *> DesignSpaces::getDesignSpaces() {
        return this->designSpaces;
    }

    void DesignSpaces::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
