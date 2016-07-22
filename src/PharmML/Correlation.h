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

#ifndef PHARMMLCPP_CORRELATION_H_
#define PHARMMLCPP_CORRELATION_H_

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
            Correlation(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);

            VariabilityReference *getVariabilityReference();
            bool isPairwise();
            std::vector<SymbRef *> getPairwiseSymbRefs();
            std::string getPairwiseType();
            std::shared_ptr<AstNode> getPairwiseAssignment();
            bool hasPureSymbRefAssigment();

            /* Referer base class (referencedSymbols) contains all symbols while correlatedSymbols
             * below only those that refer to the correlated random variables (to not mix them up): */
            SymbolSet correlatedSymbols;

            void setupSymbRefs(SymbolGathering &gathering, std::string blkId);
            void accept(PharmMLVisitor *visitor);

        private:
            VariabilityReference *variabilityReference;
            std::vector<SymbRef *> pairwiseSymbRefs;
            std::string pairwiseType;
            std::shared_ptr<AstNode> pairwiseAssignment;
            bool pure_symbref_assignment;
            std::string matrixType;
    };
}

#endif
