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

#ifndef PHARMML_DESIGNSPACES_H_
#define PHARMML_DESIGNSPACES_H_

#include <xml/xml.h>
#include <PharmML/PharmMLContext.h>
#include <symbols/Variable.h>
#include <AST/AstNode.h>
#include <visitors/XMLAstVisitor.h>

namespace PharmML
{
    class DesignSpace
    {
        PharmML::PharmMLContext *context;
        std::string oid;
        std::vector<ObjectRef *> interventionRefs;
        std::vector<ObjectRef *> observationRefs;
        std::vector<ObjectRef *> armRefs;
        AstNode *dosingTimes = nullptr;

        public:
        DesignSpace(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        xml::Node xml();
        std::string getOid();
        std::vector<ObjectRef *> getInterventionRefs();
        std::vector<ObjectRef *> getObservationRefs();
        std::vector<ObjectRef *> getArmRefs();
        AstNode *getDosingTimes();
        void accept(PharmMLVisitor *visitor);
    };

    class DesignSpaces
    {
        PharmML::PharmMLContext *context;
        std::vector<PharmML::Variable *> designParameters;
        std::vector<PharmML::DesignSpace *> designSpaces;

        public:
        DesignSpaces(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        xml::Node xml();
        std::vector<PharmML::Variable *> getDesignParameters();
        std::vector<PharmML::DesignSpace *> getDesignSpaces();
        void accept(PharmMLVisitor *visitor);
    };
}

#endif
