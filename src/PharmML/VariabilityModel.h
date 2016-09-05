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

#ifndef PHARMMLCPP_VARIABILITYMODEL_H_
#define PHARMMLCPP_VARIABILITYMODEL_H_

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <xml/xml.h>
#include <PharmML/PharmMLContext.h>
#include <symbols/VariabilityLevel.h>
#include <PharmML/Block.h>

namespace pharmmlcpp
{
    class VariabilityModel : public Block
    {
        public:
            VariabilityModel(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            std::string getName();
            bool onResidualError();
            bool onParameter();
            std::vector<pharmmlcpp::VariabilityLevel *> getVariabilityLevels();
            std::vector<pharmmlcpp::VariabilityLevel *> getVariabilityLevelHierarchy();
            void gatherSymbols(SymbolGathering &gathering);

        private:
            std::string name;
            std::string type;
            std::vector<pharmmlcpp::VariabilityLevel *> variabilityLevels;
    };
}

#endif
