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
    Interval::Interval(xml::Node node) {
        xml::Node left_endpoint = node.getChild();
        xml::Node right_endpoint = node.getLastChild();
        this->setLeftEndpoint(AstNodeFactory::create(left_endpoint.getChild().getChild()));
        if (left_endpoint.getAttribute("type").getValue() == "open") {
            this->setLeftEndpointOpen(true);
        }
        this->setRightEndpoint(AstNodeFactory::create(right_endpoint.getChild().getChild()));
        if (right_endpoint.getAttribute("type").getValue() == "open") {
            this->setRightEndpointOpen(true);
        }
    }

    AstNode *Interval::getLeftEndpoint() {
        return this->LeftEndpoint.get();
    }

    AstNode *Interval::getRightEndpoint() {
        return this->RightEndpoint.get();
    }

    void Interval::setLeftEndpoint(std::unique_ptr<AstNode> node) {
        this->LeftEndpoint = std::move(node);
    }

    void Interval::setRightEndpoint(std::unique_ptr<AstNode> node) {
        this->RightEndpoint = std::move(node);
    }

    bool Interval::isLeftEndpointOpen() {
        return this->openLeftEndpoint;
    }

    bool Interval::isRightEndpointOpen() {
        return this->openRightEndpoint;
    }

    void Interval::setLeftEndpointOpen(bool open) {
        this->openLeftEndpoint = open;
    }

    void Interval::setRightEndpointOpen(bool open) {
        this->openRightEndpoint = open;
    }

    std::unique_ptr<AstNode> Interval::clone() {
        std::unique_ptr<Interval> cl;
        return std::move(cl);
    }

    void Interval::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }
}
