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

#include "Arms.h"

namespace PharmML
{
    // OccasionType class (for OccasionSequence class)
    // TODO: Occasion is also used on top-level of TrialDesign
    OccasionType::OccasionType(PharmML::PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }
    
    void OccasionType::parse(xml::Node node) {
        // Get start
        xml::Node assign = this->context->getSingleElement(node, "./design:Start/ct:Assign");
        this->start = this->context->factory.create(assign.getChild());
        
        // Get end
        assign = this->context->getSingleElement(node, "./design:End/ct:Assign");
        this->end = this->context->factory.create(assign.getChild());
    }
    
    AstNode *OccasionType::getStart() {
        return this->start;
    }
    
    AstNode *OccasionType::getEnd() {
        return this->end;
    }
    
    // InterventionSequence class
    InterventionSequence::InterventionSequence(PharmML::PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }
    
    void InterventionSequence::parse(xml::Node node) {
        // Get intervention references
        xml::Node interventionList = this->context->getSingleElement(node, "./design:InterventionList");
        std::vector<xml::Node> interventionRefs = this->context->getElements(interventionList, "./design:InterventionRef");
        for (xml::Node ref : interventionRefs) {
            this->oidRefs.push_back(new ObjectRef(ref));
        }
        
        // Get start value
        xml::Node assign = this->context->getSingleElement(node, "./design:Start/ct:Assign");
        if (assign.exists()) {
            this->start = this->context->factory.create(assign.getChild());
        }
    }
    
    xml::Node InterventionSequence::xml() {
        xml::Node iseq("InterventionSequence");
        xml::Node ilist = iseq.createChild("InterventionList");
        xml::Node child;
        for (ObjectRef *ref : this->oidRefs) {
            child = ilist.createChild("InterventionRef");
            child.setAttribute("oidRef", ref->getOidRef());
        }
        return iseq;
    }

    std::vector<ObjectRef *> InterventionSequence::getOidRefs() {
        return this->oidRefs;
    }
    
    AstNode *InterventionSequence::getStart() {
        return this->start;
    }
    
    void InterventionSequence::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
    
    // ObservationSequence class
    ObservationSequence::ObservationSequence(PharmML::PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }
    
    void ObservationSequence::parse(xml::Node node) {
        // Get observation references
        xml::Node observationList = this->context->getSingleElement(node, "./design:ObservationList");
        std::vector<xml::Node> observationRefs = this->context->getElements(observationList, "./design:ObservationRef");
        for (xml::Node ref : observationRefs) {
            this->oidRefs.push_back(ref.getAttribute("oidRef").getValue());
        }
        
        // Get start value
        xml::Node assign = this->context->getSingleElement(node, "./design:Start/ct:Assign");
        if (assign.exists()) {
            this->start = this->context->factory.create(assign.getChild());
        }
    }
   
    xml::Node ObservationSequence::xml() {
        xml::Node oseq("ObservationSequence");
        xml::Node olist = oseq.createChild("ObservationList");
        xml::Node child;
        for (std::string ref : this->oidRefs) {
            child = olist.createChild("ObservationRef");
            child.setAttribute("oidRef", ref);
        }
        return oseq;
    }

    std::vector<std::string> ObservationSequence::getOidRefs() {
        return this->oidRefs;
    }
    
    AstNode *ObservationSequence::getStart() {
        return this->start;
    }
    
    void ObservationSequence::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
    
    // OccassionSequence class
    OccasionSequence::OccasionSequence(PharmML::PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }
    
    void OccasionSequence::parse(xml::Node node) {
        xml::Node occasionList = this->context->getSingleElement(node, "./design:OccasionList");
        
        // Get variability reference
        xml::Node varRef = this->context->getSingleElement(occasionList, "./ct:VariabilityReference");
        this->variabilityReference = new VariabilityReference(this->context, varRef);
        
        // Get occasions
        std::vector<xml::Node> occasions = this->context->getElements(occasionList, "./design:Occasion");
        for (xml::Node occ : occasions) {
            this->occasions.push_back(new OccasionType(this->context, occ));
        }
    }
    
    VariabilityReference *OccasionSequence::getVariabilityReference() {
        return this->variabilityReference;
    }
    
    std::vector<OccasionType *> OccasionSequence::getOccasions() {
        return this->occasions;
    }
    
    void OccasionSequence::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
    
    // Arm class
    Arm::Arm(PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
    }
    
    void Arm::parse(xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        
        // Get arm (oid) reference
        xml::Node armRef = this->context->getSingleElement(node, "./design:ArmRef");
        if (armRef.exists()) {
            this->oidRef = armRef.getAttribute("oidRef").getValue();
        }
        
        // Get arm size
        xml::Node assign = this->context->getSingleElement(node, "./design:ArmSize/ct:Assign");
        if (assign.exists()) {
            this->armSize = this->context->factory.create(assign.getChild());
        }
        
        // Get number of samples
        assign = this->context->getSingleElement(node, "./design:NumberSamples/ct:Assign");
        if (assign.exists()) {
            this->numSamples = this->context->factory.create(assign.getChild());
        }
        
        // Get number of times
        assign = this->context->getSingleElement(node, "./design:NumberTimes/ct:Assign");
        if (assign.exists()) {
            this->numTimes = this->context->factory.create(assign.getChild());
        }
        
        // Get same times
        assign = this->context->getSingleElement(node, "./design:SameTimes/ct:Assign");
        if (assign.exists()) {
            this->sameTimes = this->context->factory.create(assign.getChild());
        }
        
        // Get intervention sequences
        std::vector<xml::Node> sequence = this->context->getElements(node, "./design:InterventionSequence");
        for (xml::Node node : sequence) {
            InterventionSequence *sequence = new InterventionSequence(this->context, node);
            this->interventionSequences.push_back(sequence);
        }
        
        // Get observation sequences
        sequence = this->context->getElements(node, "./design:ObservationSequence");
        for (xml::Node node : sequence) {
            ObservationSequence *sequence = new ObservationSequence(this->context, node);
            this->observationSequences.push_back(sequence);
        }
        
        // Get occasion sequences
        sequence = this->context->getElements(node, "./design:OccasionSequence");
        for (xml::Node node : sequence) {
            OccasionSequence *sequence = new OccasionSequence(this->context, node);
            this->occasionSequences.push_back(sequence);
        }
    }
   
    xml::Node Arm::xml() {
        xml::Node arm("Arm");
        arm.setAttribute("oid", this->oid);
        for (InterventionSequence *seq : this->interventionSequences) {
            arm.addChild(seq->xml());
        }
        for (ObservationSequence *seq : this->observationSequences) {
            arm.addChild(seq->xml());
        }
        return arm;
    }

    std::string Arm::getOidRef(){
        return this->oidRef;
    }

    AstNode *Arm::getArmSize(){
        return this->armSize;
    }

    AstNode *Arm::getNumSamples(){
        return this->numSamples;
    }

    AstNode *Arm::getNumTimes(){
        return this->numTimes;
    }

    AstNode *Arm::getSameTimes(){
        return this->sameTimes;
    }
    
    std::vector<InterventionSequence *> Arm::getInterventionSequences(){
        return this->interventionSequences;
    }
    
    std::vector<ObservationSequence *> Arm::getObservationSequences(){
        return this->observationSequences;
    }
    
    void Arm::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
    
    // Arms class
    Arms::Arms(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }
    
    void Arms::parse(xml::Node node) {
        // Get design parameters
        // (mdef:DesignParameterType extends mdef:CommonParameterType which is close enough to class Variable for now)
        std::vector<xml::Node> design_parameters = this->context->getElements(node, "./mdef:DesignParameter");
        for (xml::Node node : design_parameters) {
            Variable *parameter = new Variable(this->context, node);
            this->designParameters.push_back(parameter);
        }
        
        // Get arm size
        xml::Node assign = this->context->getSingleElement(node, "./design:ArmSize/ct:Assign");
        if (assign.exists()) {
            this->armSize = this->context->factory.create(assign.getChild());
        }
        
        // Get cost function
        assign = this->context->getSingleElement(node, "./design:CostFunction/ct:Assign");
        if (assign.exists()) {
            this->costFunction = this->context->factory.create(assign.getChild());
        }
        
        // Get number of arms
        assign = this->context->getSingleElement(node, "./design:NumberArms/ct:Assign");
        if (assign.exists()) {
            this->numArms = this->context->factory.create(assign.getChild());
        }
        
        // Get number of samples
        assign = this->context->getSingleElement(node, "./design:NumberSamples/ct:Assign");
        if (assign.exists()) {
            this->numSamples = this->context->factory.create(assign.getChild());
        }
        
        // Get number of times
        assign = this->context->getSingleElement(node, "./design:NumberTimes/ct:Assign");
        if (assign.exists()) {
            this->numTimes = this->context->factory.create(assign.getChild());
        }
        
        // Get same times
        assign = this->context->getSingleElement(node, "./design:SameTimes/ct:Assign");
        if (assign.exists()) {
            this->sameTimes = this->context->factory.create(assign.getChild());
        }
        
        // Get total cost
        assign = this->context->getSingleElement(node, "./design:TotalCost/ct:Assign");
        if (assign.exists()) {
            this->totalCost = this->context->factory.create(assign.getChild());
        }
        
        // Get total size
        assign = this->context->getSingleElement(node, "./design:TotalSize/ct:Assign");
        if (assign.exists()) {
            this->totalSize = this->context->factory.create(assign.getChild());
        }
        
        // Get the arm definitions themselves
        std::vector<xml::Node> arms = this->context->getElements(node, "./design:Arm");
        for (xml::Node node : arms) {
            Arm *arm = new Arm(this->context, node);
            this->arms.push_back(arm);
        }
    }
   
    xml::Node Arms::xml() {
        xml::Node arms("Arms");
        for (Arm *arm : this->arms) {
            arms.addChild(arm->xml());
        }
        return arms;
    }

    std::vector<Variable *> Arms::getDesignParameters() {
        return this->designParameters;
    }
    
    AstNode *Arms::getArmSize() {
        return this->armSize;
    }
    
    AstNode *Arms::getCostFunction() {
        return this->costFunction;
    }
    
    AstNode *Arms::getNumArms() {
        return this->numArms;
    }
    
    AstNode *Arms::getNumSamples() {
        return this->numSamples;
    }
    
    AstNode *Arms::getNumTimes() {
        return this->numTimes;
    }
    
    AstNode *Arms::getSameTimes() {
        return this->sameTimes;
    }
    
    AstNode *Arms::getTotalCost() {
        return this->totalCost;
    }
    
    AstNode *Arms::getTotalSize() {
        return this->totalSize;
    }
    
    std::vector <Arm *> Arms::getArms() {
        return this->arms;
    }
    
    void Arms::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
