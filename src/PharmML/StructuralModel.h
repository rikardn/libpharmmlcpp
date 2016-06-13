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

#ifndef PHARMMLCPP_STRUCTURALMODEL_H_
#define PHARMMLCPP_STRUCTURALMODEL_H_

#include <vector>
#include <PharmML/PharmMLContext.h>
#include <PharmML/PharmMLSection.h>
#include <visitors/PharmMLVisitor.h>
#include <symbols/SymbolSet.h>
#include <symbols/Symbol.h>
#include <symbols/Variable.h>
#include <symbols/DerivativeVariable.h>
#include <PharmML/PKMacro.h>
#include <PharmML/Block.h>

namespace pharmmlcpp
{
    class StructuralModel : public Block
    {
        public:
            StructuralModel(PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            std::vector<pharmmlcpp::CommonVariable *> getVariables();
            bool hasDerivatives();
            std::vector<pharmmlcpp::CommonVariable *> getDerivatives();
            pharmmlcpp::PKMacros *getPKMacros();
            void setupRefererSymbRefs(SymbolGathering &gathering);
            void gatherSymbols(SymbolGathering &gathering) override;
            void gatherMacros(MacroGathering &gatherer);

        private:
            pharmmlcpp::PharmMLContext *context;
            std::vector<pharmmlcpp::CommonVariable *> variables;
            pharmmlcpp::PKMacros *pk_macros = nullptr;
    };
}

#endif
