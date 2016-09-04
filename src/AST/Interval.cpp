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

#include "Interval.h"

namespace pharmmlcpp
{
    /**
     *  Creates a new interval from the xml PharmML code
     */
    Interval::Interval(PharmMLReader &reader, xml::Node node) {
        xml::Node left_endpoint = node.getChild();
        xml::Node right_endpoint = node.getLastChild();
        this->setLeftEndpoint(AstNodeFactory::create(reader, left_endpoint.getChild().getChild()));
        if (left_endpoint.getAttribute("type").getValue() == "open") {
            this->setLeftEndpointOpenClosed(true);
        }
        this->setRightEndpoint(AstNodeFactory::create(reader, right_endpoint.getChild().getChild()));
        if (right_endpoint.getAttribute("type").getValue() == "open") {
            this->setRightEndpointOpenClosed(true);
        }
    }

    /**
     *  Creates a new interval from two endpoints
     *  both endpoints are set to closed
     *  \param leftEndpoint the left endpoint of the interval
     *  \param rightEndpoint the right endpoint of the interval
     */
    Interval::Interval(std::unique_ptr<AstNode> leftEndpoint, std::unique_ptr<AstNode> rightEndpoint) {
        if (!leftEndpoint || !rightEndpoint) {
            throw std::invalid_argument("nullptr");
        }
        this->leftEndpoint = std::move(leftEndpoint);
        this->rightEndpoint = std::move(rightEndpoint);
    }

    /**
     *  Copy constructor
     */
    Interval::Interval(const Interval& from) {
        this->leftEndpoint = from.leftEndpoint->clone();
        this->rightEndpoint = from.rightEndpoint->clone();
        this->openLeftEndpoint = from.openLeftEndpoint;
        this->openRightEndpoint = from.openRightEndpoint;
    }

    /**
     *  Get the left endpoint of the interval
     */
    AstNode *Interval::getLeftEndpoint() {
        return this->leftEndpoint.get();
    }

    /**
     *  Get the right endpoint of the interval
     */
    AstNode *Interval::getRightEndpoint() {
        return this->rightEndpoint.get();
    }

    /**
     *  Set the left endpoint of the interval
     */
    void Interval::setLeftEndpoint(std::unique_ptr<AstNode> node) {
        if (!node) {
            throw std::invalid_argument("nullptr");
        }
        this->leftEndpoint = std::move(node);
    }

    /**
     *  Set the right endpoint of the interval
     */
    void Interval::setRightEndpoint(std::unique_ptr<AstNode> node) {
        if (!node) {
            throw std::invalid_argument("nullptr");
        }
        this->rightEndpoint = std::move(node);
    }

    /**
     *  Check if the left endpoint is open or closed.
     *  True means open and false means closed
     */
    bool Interval::isLeftEndpointOpenClosed() {
        return this->openLeftEndpoint;
    }

    /**
     *  Check if the right endpoint is open or closed.
     *  True means open and false means closed
     */
    bool Interval::isRightEndpointOpenClosed() {
        return this->openRightEndpoint;
    }

    /**
     *  Set the left endpoint open or closed.
     *  True means open and false means closed
     */
    void Interval::setLeftEndpointOpenClosed(bool open) {
        this->openLeftEndpoint = open;
    }

    /**
     *  Set the right endpoint open or closed.
     *  True means open and false means closed
     */
    void Interval::setRightEndpointOpenClosed(bool open) {
        this->openRightEndpoint = open;
    }
    
    /**
     *  Make a clone (deep copy) of this interval.
     */
    std::unique_ptr<AstNode> Interval::clone() {
        std::unique_ptr<Interval> cl = std::make_unique<Interval>(this->leftEndpoint->clone(), this->rightEndpoint->clone());
        cl->openLeftEndpoint = this->openLeftEndpoint;
        cl->openRightEndpoint = this->openRightEndpoint;
        return std::move(cl);
    }

    xml::Node Interval::xml(PharmMLWriter &writer) {
        xml::Node interval("Interval", xml::Namespace::ct);
        xml::Node le("LeftEndpoint", xml::Namespace::ct);
        if (this->openLeftEndpoint) {
            le.setAttribute("type", "open");
        }
        xml::Node re("RightEndpoint", xml::Namespace::ct);
        if (this->openRightEndpoint) {
            re.setAttribute("type", "open");
        }
        interval.addChild(le);
        interval.addChild(re);
        xml::Node le_assign("Assign", xml::Namespace::ct);
        le.addChild(le_assign);
        xml::Node re_assign("Assign", xml::Namespace::ct);
        re.addChild(re_assign);
        le_assign.addChild(this->leftEndpoint->xml(writer));
        re_assign.addChild(this->rightEndpoint->xml(writer));
        return interval;
    }

    void Interval::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }
}
