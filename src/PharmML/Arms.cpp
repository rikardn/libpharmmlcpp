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

namespace pharmmlcpp
{
    // OccasionType class (for OccasionSequence class)
    // TODO: Occasion is also used on top-level of TrialDesign
    OccasionType::OccasionType(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void OccasionType::parse(PharmMLReader &reader, xml::Node node) {
        // Get start
        xml::Node assign = reader.getSingleElement(node, "./design:Start/ct:Assign");
        this->start = AstNode::create(reader, assign.getChild());

        // Get end
        assign = reader.getSingleElement(node, "./design:End/ct:Assign");
        this->end = AstNode::create(reader, assign.getChild());
    }

    std::shared_ptr<AstNode> OccasionType::getStart() {
        return this->start;
    }

    std::shared_ptr<AstNode> OccasionType::getEnd() {
        return this->end;
    }

    // InterventionSequence class
    InterventionSequence::InterventionSequence(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void InterventionSequence::parse(PharmMLReader &reader, xml::Node node) {
        // Get intervention references
        xml::Node interventionList = reader.getSingleElement(node, "./design:InterventionList");
        std::vector<xml::Node> interventionRefs = reader.getElements(interventionList, "./design:InterventionRef");
        for (xml::Node ref : interventionRefs) {
            this->oidRefs.push_back(new ObjectRef(ref));
        }

        // Get start value
        xml::Node assign = reader.getSingleElement(node, "./design:Start/ct:Assign");
        if (assign.exists()) {
            this->start = AstNode::create(reader, assign.getChild());
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

    std::shared_ptr<AstNode> InterventionSequence::getStart() {
        return this->start;
    }

    void InterventionSequence::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    // ObservationSequence class
    ObservationSequence::ObservationSequence(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void ObservationSequence::parse(PharmMLReader &reader, xml::Node node) {
        // Get observation references
        xml::Node observationList = reader.getSingleElement(node, "./design:ObservationList");
        std::vector<xml::Node> observationRefs = reader.getElements(observationList, "./design:ObservationRef");
        for (xml::Node ref : observationRefs) {
            this->oidRefs.push_back(new ObjectRef(ref));
        }

        // Get start value
        xml::Node assign = reader.getSingleElement(node, "./design:Start/ct:Assign");
        if (assign.exists()) {
            this->start = AstNode::create(reader, assign.getChild());
        }
    }

    xml::Node ObservationSequence::xml() {
        xml::Node oseq("ObservationSequence");
        xml::Node olist = oseq.createChild("ObservationList");
        xml::Node child;
        for (ObjectRef *ref : this->oidRefs) {
            child = olist.createChild("ObservationRef");
            child.setAttribute("oidRef", ref->getOidRef());
        }
        return oseq;
    }

    std::vector<ObjectRef *> ObservationSequence::getOidRefs() {
        return this->oidRefs;
    }

    std::shared_ptr<AstNode> ObservationSequence::getStart() {
        return this->start;
    }

    void ObservationSequence::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    // OccassionSequence class
    OccasionSequence::OccasionSequence(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void OccasionSequence::parse(PharmMLReader &reader, xml::Node node) {
        xml::Node occasionList = reader.getSingleElement(node, "./design:OccasionList");

        // Get variability reference
        xml::Node varRef = reader.getSingleElement(occasionList, "./ct:VariabilityReference");
        this->variabilityReference = new VariabilityReference(reader, varRef);

        // Get occasions
        std::vector<xml::Node> occasions = reader.getElements(occasionList, "./design:Occasion");
        for (xml::Node occ : occasions) {
            this->occasions.push_back(new OccasionType(reader, occ));
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
    Arm::Arm(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->parse(reader, node);
    }

    void Arm::parse(PharmMLReader &reader, xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();

        // Get arm (oid) reference
        xml::Node armRef = reader.getSingleElement(node, "./design:ArmRef");
        if (armRef.exists()) {
            this->oidRef = armRef.getAttribute("oidRef").getValue();
        }

        // Get arm size
        xml::Node assign = reader.getSingleElement(node, "./design:ArmSize/ct:Assign");
        if (assign.exists()) {
            this->armSize = AstNode::create(reader, assign.getChild());
        }

        // Get number of samples
        assign = reader.getSingleElement(node, "./design:NumberSamples/ct:Assign");
        if (assign.exists()) {
            this->numSamples = AstNode::create(reader, assign.getChild());
        }

        // Get number of times
        assign = reader.getSingleElement(node, "./design:NumberTimes/ct:Assign");
        if (assign.exists()) {
            this->numTimes = AstNode::create(reader, assign.getChild());
        }

        // Get same times
        assign = reader.getSingleElement(node, "./design:SameTimes/ct:Assign");
        if (assign.exists()) {
            this->sameTimes = AstNode::create(reader, assign.getChild());
        }

        // Get intervention sequences
        std::vector<xml::Node> sequence = reader.getElements(node, "./design:InterventionSequence");
        for (xml::Node node : sequence) {
            InterventionSequence *sequence = new InterventionSequence(reader, node);
            this->interventionSequences.push_back(sequence);
        }

        // Get observation sequences
        sequence = reader.getElements(node, "./design:ObservationSequence");
        for (xml::Node node : sequence) {
            ObservationSequence *sequence = new ObservationSequence(reader, node);
            this->observationSequences.push_back(sequence);
        }

        // Get occasion sequences
        sequence = reader.getElements(node, "./design:OccasionSequence");
        for (xml::Node node : sequence) {
            OccasionSequence *sequence = new OccasionSequence(reader, node);
            this->occasionSequences.push_back(sequence);
        }
    }

    void Arm::gatherObjectRefs(std::unordered_map<std::string, Object *> &oidMap) {
        for (InterventionSequence *is : this->interventionSequences) {
            for (ObjectRef *ref : is->getOidRefs()) {
                ref->setObject(oidMap[ref->getOidRef()]);
            }
        }
        for (ObservationSequence *os : this->observationSequences) {
            for (ObjectRef *ref : os->getOidRefs()) {
                ref->setObject(oidMap[ref->getOidRef()]);
            }
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

    std::shared_ptr<AstNode> Arm::getArmSize(){
        return this->armSize;
    }

    std::shared_ptr<AstNode> Arm::getNumSamples(){
        return this->numSamples;
    }

    std::shared_ptr<AstNode> Arm::getNumTimes(){
        return this->numTimes;
    }

    std::shared_ptr<AstNode> Arm::getSameTimes(){
        return this->sameTimes;
    }

    std::vector<InterventionSequence *> Arm::getInterventionSequences(){
        return this->interventionSequences;
    }

    std::vector<ObservationSequence *> Arm::getObservationSequences(){
        return this->observationSequences;
    }

    void Arm::accept(ObjectVisitor *visitor) {
        visitor->visit(this);
    }

    void Arm::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    // Arms class
    Arms::Arms(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void Arms::parse(PharmMLReader &reader, xml::Node node) {
        // Get design parameters
        // (mdef:DesignParameterType extends mdef:CommonParameterType which is close enough to class Variable for now)
        std::vector<xml::Node> design_parameters = reader.getElements(node, "./mdef:DesignParameter");
        for (xml::Node node : design_parameters) {
            Variable *parameter = new Variable(reader, node);
            this->designParameters.push_back(parameter);
        }

        // Get arm size
        xml::Node assign = reader.getSingleElement(node, "./design:ArmSize/ct:Assign");
        if (assign.exists()) {
            this->armSize = AstNode::create(reader, assign.getChild());
        }

        // Get cost function
        assign = reader.getSingleElement(node, "./design:CostFunction/ct:Assign");
        if (assign.exists()) {
            this->costFunction = AstNode::create(reader, assign.getChild());
        }

        // Get number of arms
        assign = reader.getSingleElement(node, "./design:NumberArms/ct:Assign");
        if (assign.exists()) {
            this->numArms = AstNode::create(reader, assign.getChild());
        }

        // Get number of samples
        assign = reader.getSingleElement(node, "./design:NumberSamples/ct:Assign");
        if (assign.exists()) {
            this->numSamples = AstNode::create(reader, assign.getChild());
        }

        // Get number of times
        assign = reader.getSingleElement(node, "./design:NumberTimes/ct:Assign");
        if (assign.exists()) {
            this->numTimes = AstNode::create(reader, assign.getChild());
        }

        // Get same times
        assign = reader.getSingleElement(node, "./design:SameTimes/ct:Assign");
        if (assign.exists()) {
            this->sameTimes = AstNode::create(reader, assign.getChild());
        }

        // Get total cost
        assign = reader.getSingleElement(node, "./design:TotalCost/ct:Assign");
        if (assign.exists()) {
            this->totalCost = AstNode::create(reader, assign.getChild());
        }

        // Get total size
        assign = reader.getSingleElement(node, "./design:TotalSize/ct:Assign");
        if (assign.exists()) {
            this->totalSize = AstNode::create(reader, assign.getChild());
        }

        // Get the arm definitions themselves
        std::vector<xml::Node> arms = reader.getElements(node, "./design:Arm");
        for (xml::Node node : arms) {
            Arm *arm = new Arm(reader, node);
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

    std::shared_ptr<AstNode> Arms::getArmSize() {
        return this->armSize;
    }

    std::shared_ptr<AstNode> Arms::getCostFunction() {
        return this->costFunction;
    }

    std::shared_ptr<AstNode> Arms::getNumArms() {
        return this->numArms;
    }

    std::shared_ptr<AstNode> Arms::getNumSamples() {
        return this->numSamples;
    }

    std::shared_ptr<AstNode> Arms::getNumTimes() {
        return this->numTimes;
    }

    std::shared_ptr<AstNode> Arms::getSameTimes() {
        return this->sameTimes;
    }

    std::shared_ptr<AstNode> Arms::getTotalCost() {
        return this->totalCost;
    }

    std::shared_ptr<AstNode> Arms::getTotalSize() {
        return this->totalSize;
    }

    std::vector <Arm *> Arms::getArms() {
        return this->arms;
    }

    void Arms::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
