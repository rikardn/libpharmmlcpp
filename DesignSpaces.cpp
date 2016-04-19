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
            this->interventionRefs.push_back(ref.getAttribute("oidRef").getValue());
        }
        refs = this->context->getElements(node, "./design:ObservationRef");
        for (xml::Node ref : refs) {
            this->observationRefs.push_back(ref.getAttribute("oidRef").getValue());
        }
        refs = this->context->getElements(node, "./design:ArmRef");
        for (xml::Node ref : refs) {
            this->armRefs.push_back(ref.getAttribute("oidRef").getValue());
        }
        xml::Node dosing_times = this->context->getSingleElement(node, "./design:DosingTimes");
        if (dosing_times.exists()) {
            this->dosingTimes = this->context->factory.create(dosing_times.getChild().getChild());
        }
    }
    
    std::string DesignSpace::getOid() {
        return this->oid;
    }
    
    std::vector<std::string> DesignSpace::getInterventionRefs() {
        return this->interventionRefs;
    }
    
    std::vector<std::string> DesignSpace::getObservationRefs() {
        return this->observationRefs;
    }
    
    std::vector<std::string> DesignSpace::getArmRefs() {
        return this->armRefs;
    }
        
    AstNode *DesignSpace::getDosingTimes() {
        return this->dosingTimes;
    }
    
    void DesignSpace::accept(AbstractVisitor *visitor) {
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
    
    std::vector<Variable *> DesignSpaces::getDesignParameters() {
        return this->designParameters;
    }
    
    std::vector<DesignSpace *> DesignSpaces::getDesignSpaces() {
        return this->designSpaces;
    }
    
    void DesignSpaces::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
}
