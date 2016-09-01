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
    Sequence::Sequence(xml::Node node) {
        xml::Node begin = node.getChild();
        xml::Node second

        xml::Node left_endpoint = node.getChild();
        xml::Node right_endpoint = node.getLastChild();
        this->setLeftEndpoint(AstNodeFactory::create(left_endpoint.getChild().getChild()));
        if (left_endpoint.getAttribute("type").getValue() == "open") {
            this->setLeftEndpointOpenClosed(true);
        }
        this->setRightEndpoint(AstNodeFactory::create(right_endpoint.getChild().getChild()));
        if (right_endpoint.getAttribute("type").getValue() == "open") {
            this->setRightEndpointOpenClosed(true);
        }
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
        // Check for all valid combinations. Use deMorgan to unentangle
        if (!begin || !((stepSize && !stepNumber && end) || (stepSize && stepNumber && !end) || (!stepSize && stepNumber && end))) {
            throw std::invalid_argument("not supported combination in sequence");
        }
        this->begin = std::move(begin);
        this->stepSize = std::move(stepSize);
        this->stepNumber = std::move(stepNumber);
        this->end = std::move(end);
    }

    /**
     *  Copy constructor
     */
    Sequence::Sequence(const Sequence& from) {
        this->begin = from.begin->clone();
        if (this->stepSize) {
            from.stepSize->clone();
        }
        if (this->stepNumber) {
            from.stepNumber->clone();
        }
        if (this->end) {
            from.end->clone();
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
        if (this->stepSize) {
            stepSize_clone = this->stepSize->clone();
        }
        if (this->stepNumber) {
            stepNumber_clone = this->stepNumber->clone();
        }
        if (this->end) {
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
        if (this->stepSize) {
            xml::Node stepSize("StepSize", xml::Namespace::ct);
            stepSize.addChild(this->stepSize->xml(writer));
            sequence.addChild(stepSize);
        }
        if (this->stepNumber) {
            xml::Node stepNumber("StepNumber", xml::Namespace::ct);
            stepNumber.addChild(this->stepNumber->xml(writer));
            sequence.addChild(stepNumber);
        }
        if (this->end) {
            xml::Node end("End", xml::Namespace::ct);
            end.addChild(this->end->xml(writer));
            sequence.addChild(end);
        }
        return sequence;
    }

    void Sequence::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }
}
