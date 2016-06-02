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

#ifndef PHARMML_INTERVENTIONS_H_
#define PHARMML_INTERVENTIONS_H_

#include <xml/xml.h>
#include <PharmML/PharmMLContext.h>
#include <AST/AstNode.h>
#include <AST/symbols.h>
#include <visitors/PharmMLVisitor.h>
#include <visitors/XMLAstVisitor.h>
#include <PharmML/Dataset.h>
#include <objects/Object.h>
#include <PharmML/PharmMLSection.h>
#include <visitors/ObjectVisitor.h>
#include <PharmML/PharmMLSection.h>

#include <PharmML/ColumnMapping.h> // TODO: Consider (for TargetMapping class; separate src file?)

namespace PharmML
{
    // TODO: Maybe move this somewhere?
    class SteadyStateParameter : public AstNode
    {
        public:
            SteadyStateParameter(PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            AstNode *getAssignment();
            AstNode *getSymbRef();
            void accept(AstNodeVisitor *visitor);

        protected:
            PharmML::PharmMLContext *context;

        private:
            AstNode *assignment = nullptr;
            SymbRef *symbRef = nullptr;
    };

    class Administration : public Object, public PharmMLSection
    {
        public:
            Administration(PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            void gatherObjectRefs(std::unordered_map<std::string, Object *> &oidMap) {};
            xml::Node xml();
            std::string getType();
            std::string getTargetType();
            AstNode *getAmount();
            std::vector<AstNode *> getAmountAsVector();
            PharmML::TargetMapping *getTargetMapping();
            PharmML::SymbRef *getTargetSymbRef();
            AstNode *getTimes();
            std::vector<AstNode *> getTimesAsVector();
            AstNode *getSteady();
            AstNode *getDuration();
            AstNode *getRate();
            void accept(PharmMLVisitor *visitor);
            void accept(ObjectVisitor *visitor);

        private:
            PharmML::PharmMLContext *context;
            std::string type;
            std::string target_type;
            AstNode *amount = nullptr;
            PharmML::SymbRef *target_symbref = nullptr;
            PharmML::TargetMapping *target_mapping = nullptr;
            AstNode *times = nullptr;
            AstNode *steady = nullptr;
            AstNode *duration = nullptr;
            AstNode *rate = nullptr;
    };

    class IndividualAdministration : public ObjectReferer, public PharmMLSection
    {
        public:
            IndividualAdministration(PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            void gatherObjectRefs(std::unordered_map<std::string, Object *> &oidMap);
            xml::Node xml();
            ObjectRef *getOidRef();
            std::vector<PharmML::ColumnMapping *> getColumnMappings();
            Dataset *getDataset();
            void accept(PharmMLVisitor *visitor);

        private:
            PharmML::PharmMLContext *context;
            ObjectRef *oidRef;
            std::vector<PharmML::ColumnMapping *> columnMappings;
            Dataset *dataset;
    };

    class SingleIntervention
    {
        public:
            SingleIntervention(PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            std::vector<ObjectRef *> getOidRefs();
            AstNode *getStart();
            AstNode *getEnd();

        private:
            PharmMLContext *context;
            std::vector<ObjectRef *> oidRefs;
            AstNode *start;
            AstNode *end;
    };

    class InterventionsCombination : public Object
    {
        public:
            InterventionsCombination(PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            std::vector<SingleIntervention *> getSingleInterventions();
            AstNode *getRelative();

        private:
            PharmMLContext *context;
            std::vector<SingleIntervention *> singleInterventions;
            AstNode *relative;
    };

    class Interventions
    {
        public:
            Interventions(PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            xml::Node xml();
            std::vector<Administration *> getAdministrations();
            std::vector<IndividualAdministration *> getIndividualAdministrations();
            void accept(PharmMLVisitor *visitor);

        private:
            PharmML::PharmMLContext *context;
            std::vector<PharmML::Administration *> administrations;
            std::vector<PharmML::IndividualAdministration *> individualAdministrations;

    };
}

#endif
