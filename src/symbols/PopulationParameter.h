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

#ifndef PHARMMLCPP_POPULATIONPARAMETER_H_
#define PHARMMLCPP_POPULATIONPARAMETER_H_

#include <AST/symbols.h>
#include <PharmML/PharmMLReader.h>
#include <PharmML/PharmMLSection.h>
#include <xml/xml.h>
#include <visitors/PharmMLVisitor.h>

namespace pharmmlcpp
{
    class PopulationParameter : public Symbol, public PharmMLSection
    {
        public:
            PopulationParameter(std::string symbId);
            PopulationParameter(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);

            bool hasAssignment() { return this->assignment != nullptr; };
            std::shared_ptr<AstNode> getAssignment() { return this->assignment; };

            void setupSymbRefs(SymbolGathering &gathering, std::string blkId) override;
            void accept(PharmMLVisitor *visitor);
            void accept(SymbolVisitor *visitor);
        private:
            std::shared_ptr<AstNode> assignment = nullptr;
    };

    // FIXME: Inheritance should be other way around
    class Parameter : public PopulationParameter
    {
        public:
            Parameter(PharmMLReader &reader, xml::Node node) : PopulationParameter(reader, node) {};
            //void accept(SymbolVisitor *visitor);
    };
}

#endif
