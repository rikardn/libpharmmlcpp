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

#ifndef PHARMMLCPP_INTERVAL_H_
#define PHARMMLCPP_INTERVAL_H_

#include <AST/AstNode.h>
#include <xml/xml.h>

namespace pharmmlcpp
{
    /**
     *  An interval with two endpoints. Each endpoint can be either open or closed
     */
    class Interval : public AstNode
    {
        public:
            Interval(PharmMLReader &reader, xml::Node node);
            Interval(std::unique_ptr<AstNode> leftEndpoint, std::unique_ptr<AstNode> rightEndpoint);
            Interval(const Interval& from);
            AstNode *getLeftEndpoint();
            AstNode *getRightEndpoint();
            void setLeftEndpoint(std::unique_ptr<AstNode> node);
            void setRightEndpoint(std::unique_ptr<AstNode> node);
            bool isLeftEndpointOpenClosed();
            bool isRightEndpointOpenClosed();
            void setLeftEndpointOpenClosed(bool open);
            void setRightEndpointOpenClosed(bool open);
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;

        private:
            std::unique_ptr<AstNode> leftEndpoint;
            std::unique_ptr<AstNode> rightEndpoint;
            bool openLeftEndpoint = false;
            bool openRightEndpoint = false;
    };
}

#endif
