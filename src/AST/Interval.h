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

#include <AST/AstNodeFactory.h>
#include <AST/AstNode.h>
#include <xml/xml.h>

namespace pharmmlcpp
{
    class Interval : public AstNode
    {
        public:
            Interval(xml::Node node);
            AstNode *getLeftEndpoint();
            AstNode *getRightEndpoint();
            void setLeftEndpoint(AstNode *node);
            void setRightEndpoint(AstNode *node);
            bool isLeftEndpointOpen();
            bool isRightEndpointOpen();
            void setLeftEndpointOpen(bool open);
            void setRightEndpointOpen(bool open);
            void accept(AstNodeVisitor *visitor);

        private:
            AstNode *LeftEndpoint = nullptr;
            AstNode *RightEndpoint = nullptr;
            bool openLeftEndpoint = false;
            bool openRightEndpoint = false;
    };
}

#endif
