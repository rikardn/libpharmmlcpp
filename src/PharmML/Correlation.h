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

#ifndef PHARMML_CORRELATION_H_
#define PHARMML_CORRELATION_H_

#include <xml/xml.h>
#include <PharmML/PharmMLContext.h>
#include <PharmML/PharmMLSection.h>
#include <AST/AstNode.h>
#include <visitors/PharmMLVisitor.h>
#include <symbols/VariabilityLevel.h>
#include <symbols/SymbolSet.h>
#include <symbols/Symbol.h>

namespace pharmmlcpp
{
    class Correlation : public Referer, public PharmMLSection
    {
        public:
            Correlation(PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);

            pharmmlcpp::VariabilityReference *getVariabilityReference();
            bool isPairwise();
            std::vector<pharmmlcpp::SymbRef *> getPairwiseSymbRefs();
            std::string getPairwiseType();
            pharmmlcpp::AstNode *getPairwiseAssignment();
            bool hasPureSymbRefAssigment();

            /* Referer base class (referencedSymbols) contains all symbols while correlatedSymbols
             * below only those that refer to the correlated random variables (to not mix them up): */
            pharmmlcpp::SymbolSet correlatedSymbols;

            void setupSymbRefs(SymbolGathering &gathering, std::string blkId);
            void accept(PharmMLVisitor *visitor);

        private:
            pharmmlcpp::PharmMLContext *context;
            pharmmlcpp::VariabilityReference *variabilityReference;
            std::vector<pharmmlcpp::SymbRef *> pairwiseSymbRefs;
            std::string pairwiseType;
            pharmmlcpp::AstNode *pairwiseAssignment = nullptr;
            bool pure_symbref_assignment;
            std::string matrixType;
    };
}

#endif
