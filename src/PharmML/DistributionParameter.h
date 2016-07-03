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

#ifndef PHARMMLCPP_DISTRIBUTIONPARAMETER_H_
#define PHARMMLCPP_DISTRIBUTIONPARAMETER_H_

#include "PharmMLContext.h"
#include <AST/AstNode.h>
#include <symbols/Symbol.h> // Why? Because it's a referer!

namespace pharmmlcpp
{
    class DistributionParameter : public Referer
    {
        public:
            DistributionParameter(PharmMLReader &reader);    // Legacy UncertML constructor
            DistributionParameter(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            std::string getName();
            void setName(std::string name);
            AstNode *getAssignment();
            void setAssignment(AstNode *assignment);
            void setupSymbRefs(SymbolGathering &gathering, std::string blkId) override {};
        
        private:
            std::string name;
            AstNode *assignment;
    };
}

#endif
