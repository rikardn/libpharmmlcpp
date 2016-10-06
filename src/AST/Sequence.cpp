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

#include "Sequence.h"

namespace pharmmlcpp
{
    /**
     *  Creates a new interval from the xml PharmML code
     */
    Sequence::Sequence(PharmMLReader &reader, xml::Node node) {
        xml::Node begin_node = reader.getSingleElement(node, "./ct:Begin");
        if (begin_node.exists()) {
            this->begin = AstNode::create(reader, begin_node.getChild());
        }
        xml::Node stepSize_node = reader.getSingleElement(node, "./ct:StepSize");
        if (stepSize_node.exists()) {
            this->stepSize = AstNode::create(reader, stepSize_node.getChild());
        }
        xml::Node stepNumber_node = reader.getSingleElement(node, "./ct:StepNumber");
        if (stepNumber_node.exists()) {
            this->stepNumber = AstNode::create(reader, stepNumber_node.getChild());
        }
        xml::Node end_node = reader.getSingleElement(node, "./ct:End");
        if (end_node.exists()) {
            this->end = AstNode::create(reader, end_node);
        }
        this->checkInvariance();
    }

    /**
     *  Creates a new sequence
     *  \param begin The starting point of the sequence 
     *  \param stepSize Size of one step in the sequence (used together with begin and end or begin and stepnumber)
     *  \param stepNumber The number of steps in the sequence (used together with begin and stepsize or begin and end)
     *  \param end The end point of the sequence
     */
    Sequence::Sequence(std::unique_ptr<AstNode> begin, std::unique_ptr<AstNode> stepSize,
            std::unique_ptr<AstNode> stepNumber, std::unique_ptr<AstNode> end) {
        this->begin = std::move(begin);
        this->stepSize = std::move(stepSize);
        this->stepNumber = std::move(stepNumber);
        this->end = std::move(end);
        this->checkInvariance();
        if (this->stepSize) {
            this->originalStepSize = true;
        }
        if (this->stepNumber) {
            this->originalStepNumber = true;
        }
        if (this->end) {
            this->originalEnd = true;
        }
    }

    void Sequence::checkInvariance() {
        // Check for all valid combinations. Use deMorgan to unentangle
        if (!begin || !((stepSize && !stepNumber && end) || (stepSize && stepNumber && !end) || (!stepSize && stepNumber && end))) {
            throw std::invalid_argument("not supported combination in sequence");
        }
    }

    /**
     *  Copy constructor
     */
    Sequence::Sequence(const Sequence& from) {
        this->begin = from.begin->clone();
        if (from.originalStepSize) {
            this->originalStepSize = true;
            this->stepSize = from.stepSize->clone();
        }
        if (from.originalStepNumber) {
            this->originalStepNumber = true;
            this->stepNumber = from.stepNumber->clone();
        }
        if (from.originalEnd) {
            this->originalEnd = true;
            this->end = from.end->clone();
        }
    }

    /**
     *  Make a clone (deep copy) of this sequence.
     */
    std::unique_ptr<AstNode> Sequence::clone() {
        std::unique_ptr<AstNode> begin_clone = this->begin->clone();
        std::unique_ptr<AstNode> stepSize_clone;
        std::unique_ptr<AstNode> stepNumber_clone;
        std::unique_ptr<AstNode> end_clone;
        if (this->originalStepSize) {
            stepSize_clone = this->stepSize->clone();
        }
        if (this->originalStepNumber) {
            stepNumber_clone = this->stepNumber->clone();
        }
        if (this->originalEnd) {
            end_clone = this->end->clone();
        }
        std::unique_ptr<Sequence> cl =
            std::make_unique<Sequence>(std::move(begin_clone), std::move(stepSize_clone), std::move(stepNumber_clone), std::move(end_clone));
        return std::move(cl);
    }

    xml::Node Sequence::xml(PharmMLWriter &writer) {
        xml::Node sequence("Sequence", xml::Namespace::ct);
        xml::Node begin("Begin", xml::Namespace::ct);
        begin.addChild(this->begin->xml(writer));
        sequence.addChild(begin);
        if (this->originalStepSize) {
            xml::Node stepSize("StepSize", xml::Namespace::ct);
            stepSize.addChild(this->stepSize->xml(writer));
            sequence.addChild(stepSize);
        }
        if (this->originalStepNumber) {
            xml::Node stepNumber("StepNumber", xml::Namespace::ct);
            stepNumber.addChild(this->stepNumber->xml(writer));
            sequence.addChild(stepNumber);
        }
        if (this->originalEnd) {
            xml::Node end("End", xml::Namespace::ct);
            end.addChild(this->end->xml(writer));
            sequence.addChild(end);
        }
        return sequence;
    }

    /**
     *  Get the starting point of the sequence
     */
    AstNode *Sequence::getBegin() {
        return this->begin.get();
    }

    /**
     *  Get the step size of the sequence
     *  If the sequence did not have a defined step size from construction one will be calculated.
     */
    AstNode *Sequence::getStepSize() {
        if (!this->stepSize) {
            // StepSize = (End - Begin) / StepNumber
            std::unique_ptr<BinopMinus> minus = std::make_unique<BinopMinus>(this->end->clone(), this->begin->clone());
            std::unique_ptr<BinopDivide> div = std::make_unique<BinopDivide>(std::move(minus), this->stepNumber->clone());
            this->stepSize = std::move(div);
        }
        return this->stepSize.get();
    }

    /**
     *  Get the number of steps of the sequence
     *  If the sequence did not have a defined number of steps from construction one will be calculated.
     */
    AstNode *Sequence::getStepNumber() {
        if (!this->stepNumber) {
            // StepNumber = (End - Begin) / StepSize
            std::unique_ptr<BinopMinus> minus = std::make_unique<BinopMinus>(this->end->clone(), this->begin->clone());
            std::unique_ptr<BinopDivide> div = std::make_unique<BinopDivide>(std::move(minus), this->stepSize->clone());
            this->stepNumber = std::move(div);
        }
        return this->stepNumber.get();
    }

    /**
     *  Get the end point of the sequence
     *  If the sequence did not have a defined end point from construction one will be calculated.
     */
    AstNode *Sequence::getEnd() {
        if (!this->end) {
            // End = Begin + StepNumber * StepSize
            std::unique_ptr<BinopTimes> times = std::make_unique<BinopTimes>(this->stepNumber->clone(), this->stepSize->clone());
            std::unique_ptr<BinopPlus> plus = std::make_unique<BinopPlus>(this->begin->clone(), std::move(times));
            this->end = std::move(plus);
        }
        return this->end.get();
    }

    /**
     *  Set a starting point for the sequence
     */
    void Sequence::setBegin(std::unique_ptr<AstNode> begin) {
        this->begin = std::move(begin);
        this->invalidateCache();
    }

    /**
     *  Set a step size for the sequence.
     *  Throws if the sequence was not constructed with a step size.
     *  In that case the form of the sequence must be changed first
     *  \sa changeFormToBeginStepSizeEnd(), changeFromToBeginStepSizeStepNumber() 
     */
    void Sequence::setStepSize(std::unique_ptr<AstNode> stepSize) {
        if (this->originalStepSize) {
            this->stepSize = std::move(stepSize);
            this->invalidateCache();
        } else {
            throw std::invalid_argument("Already have Begin, StepNumber and End in Sequence, cannot set StepSize");
        }
    }

    /**
     *  Set a number of steps for the sequence.
     *  Throws if the sequence was not constructed with a number of steps.
     *  In that case the form of the sequence must be changed first
     *  \sa changeFromToBeginStepSizeStepNumber(), changeFromToBeginStepNumberEnd()
     */
    void Sequence::setStepNumber(std::unique_ptr<AstNode> stepNumber) {
        if (this->originalStepNumber) {
            this->stepNumber = std::move(stepNumber);
            this->invalidateCache();
        } else {
            throw std::invalid_argument("Already have Begin, StepSize and End in Sequence, cannot set StepNumber");
        }
    }

    /**
     *  Set an end point for the sequence.
     *  Throws if the sequence was not constructed with an end point.
     *  In that case the form of the sequence must be changed first
     *  \sa changeFormToBeginStepSizeEnd(), changeFromToBeginStepNumberEnd()
     */
    void Sequence::setEnd(std::unique_ptr<AstNode> end) {
        if (this->originalEnd) {
            this->end = std::move(end);
            this->invalidateCache();
        } else {
            throw std::invalid_argument("Already have Begin, StepSize and StepNumber in Sequence, cannot set End");
        }
    }

    void Sequence::invalidateCache() {
        if (!this->originalStepSize) {
            this->stepSize = nullptr;
        } else if (!this->originalStepNumber) {
            this->stepNumber = nullptr;
        } else if (!this->originalEnd) {
            this->end = nullptr;
        }
    }

    /**
     *  Change the form of the sequence to have a start point, a step size and an end point
     */
    void Sequence::changeFormToBeginStepSizeEnd() {
        this->originalStepSize = true;
        this->originalEnd = true;
        this->originalStepNumber = false;
        if (!this->stepSize) {
            this->getStepSize();
        }
        if (!this->end) {
            this->getEnd();
        }
    }

    /**
     *  Change the form of the sequence to have a start point, a step size and a number of steps
     */
    void Sequence::changeFromToBeginStepSizeStepNumber() {
        this->originalStepSize = true;
        this->originalStepNumber = true;
        this->originalEnd = false;
        if (!this->stepSize) {
            this->getStepSize();
        }
        if (!this->end) {
            this->getEnd();
        }
    }

    /**
     *  Change the form of the sequence to have a start point, a number of steps and an end point
     */
    void Sequence::changeFromToBeginStepNumberEnd() {
        this->originalStepSize = false;
        this->originalStepNumber = true;
        this->originalEnd = true;
        if (!this->stepNumber) {
            this->getStepNumber();
        }
        if (!this->end) {
            this->getEnd();
        }
    }

    void Sequence::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }
}
