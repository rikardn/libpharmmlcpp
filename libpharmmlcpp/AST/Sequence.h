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

#ifndef PHARMMLCPP_SEQUENCE_H_
#define PHARMMLCPP_SEQUENCE_H_

#include <PharmML/PharmMLReader.h>
#include <AST/AstNode.h>
#include <xml/xml.h>
#include <AST/Binop.h>

namespace pharmmlcpp
{
    /**
     *  A sequence of numbers.
     *  Can consist of one of the following combinations:
     *      1. Begin, StepSize, End
     *      2. Begin, StepSize, StepNumber
     *      3. Begin, StepNumber, End
     */
    class Sequence : public AstNode
    {
        public:
            Sequence(PharmMLReader &reader, xml::Node node);
            Sequence(std::unique_ptr<AstNode> begin, std::unique_ptr<AstNode> stepSize,
                    std::unique_ptr<AstNode> stepNumber, std::unique_ptr<AstNode> end);
            Sequence(const Sequence& from);
            AstNode *getBegin();
            AstNode *getStepSize();
            AstNode *getStepNumber();
            AstNode *getEnd();
            void setBegin(std::unique_ptr<AstNode> begin);
            void setStepSize(std::unique_ptr<AstNode> stepSize);
            void setStepNumber(std::unique_ptr<AstNode> stepNumber);
            void setEnd(std::unique_ptr<AstNode> setEnd);
            void changeFormToBeginStepSizeEnd();
            void changeFromToBeginStepSizeStepNumber();
            void changeFromToBeginStepNumberEnd();
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;

        private:
            void invalidateCache();
            void checkInvariance();
            bool originalStepSize = false;
            bool originalStepNumber = false;
            bool originalEnd = false;
            std::unique_ptr<AstNode> begin;
            std::unique_ptr<AstNode> stepSize;
            std::unique_ptr<AstNode> stepNumber;
            std::unique_ptr<AstNode> end;
    };
}

#endif
