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

#ifndef PHARMMLCPP_DESIGNSPACES_H_
#define PHARMMLCPP_DESIGNSPACES_H_

#include <xml/xml.h>
#include <PharmML/PharmMLContext.h>
#include <symbols/Variable.h>
#include <AST/AstNode.h>
#include <visitors/XMLAstVisitor.h>

namespace pharmmlcpp
{
    class DesignSpace : public Referer
    {
        public:
            DesignSpace(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            xml::Node xml();
            std::string getOid();
            std::vector<ObjectRef *> getInterventionRefs();
            std::vector<ObjectRef *> getObservationRefs();
            std::vector<ObjectRef *> getArmRefs();
            std::vector<SymbRef *> getSymbRefs();
            std::shared_ptr<AstNode> getAssignment();
            std::shared_ptr<AstNode> getDosingTimes();
            void setupSymbRefs(SymbolGathering &gathering, std::string blkId);
            void accept(PharmMLVisitor *visitor);

        private:
            std::string oid;
            std::vector<ObjectRef *> interventionRefs;
            std::vector<ObjectRef *> observationRefs;
            std::vector<ObjectRef *> armRefs;
            std::vector<SymbRef *> symbRefs;
            std::shared_ptr<AstNode> dosingTimes;
            std::shared_ptr<AstNode> assignment;
    };

    class DesignSpaces
    {
        public:
            DesignSpaces(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            xml::Node xml();
            std::vector<Variable *> getDesignParameters();
            std::vector<DesignSpace *> getDesignSpaces();
            DesignSpace *getDesignSpaceFromSymbol(Symbol *symbol);
            void setupRefererSymbRefs(SymbolGathering &gathering);
            void accept(PharmMLVisitor *visitor);

        private:
            std::vector<Variable *> designParameters;
            std::vector<DesignSpace *> designSpaces;

    };
}

#endif
