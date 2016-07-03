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

#ifndef PHARMMLCPP_DISTRIBUTION_H_
#define PHARMMLCPP_DISTRIBUTION_H_

#include <PharmML/PharmMLContext.h>
#include <PharmML/DistributionParameter.h>
#include <AST/Scalar.h>
#include <AST/symbols.h>
#include <visitors/PharmMLVisitor.h>

namespace pharmmlcpp
{
    class Distribution
    {
        public:
            Distribution(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            std::string getName();
            std::vector<DistributionParameter *> getDistributionParameters();
            void accept(PharmMLVisitor *visitor);

        private:
            std::string name;
            std::vector<DistributionParameter *> parameters;
    };
}

#endif
