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

#ifndef PHARMMLCPP_INTERVENTIONS_H_
#define PHARMMLCPP_INTERVENTIONS_H_

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
#include <symbols/Symbol.h>

#include <PharmML/ColumnMapping.h> // TODO: Consider (for TargetMapping class; separate src file?)

namespace pharmmlcpp
{
    // TODO: Maybe move this somewhere?
    class SteadyStateParameter : public AstNode
    {
        public:
            SteadyStateParameter(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            AstNode *getAssignment();
            AstNode *getSymbRef();
            void accept(AstNodeVisitor *visitor);

        private:
            AstNode *assignment = nullptr;
            SymbRef *symbRef = nullptr;
    };

    class Administration : public Object, public Referer, public PharmMLSection
    {
        public:
            Administration(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            void postParse();
            void gatherObjectRefs(std::unordered_map<std::string, Object *> &oidMap) {};
            xml::Node xml();
            std::string getType();
            std::string getTargetType();
            AstNode *getAmount();
            std::vector<AstNode *> getAmountAsVector();
            pharmmlcpp::TargetMapping *getTargetMapping();
            pharmmlcpp::SymbRef *getTargetSymbRef();
            AstNode *getTimes();
            std::vector<AstNode *> getTimesAsVector();
            AstNode *getSteady();
            AstNode *getDuration();
            AstNode *getRate();
            void setupSymbRefs(SymbolGathering &gathering, std::string blkId);
            void setupTargetMappings(SymbolGathering &gathering);
            void accept(PharmMLVisitor *visitor);
            void accept(ObjectVisitor *visitor);

        private:
            std::string type;
            std::string target_type;
            AstNode *amount = nullptr;
            pharmmlcpp::SymbRef *target_symbref = nullptr;
            pharmmlcpp::TargetMapping *target_mapping = nullptr;
            AstNode *times = nullptr;
            AstNode *steady = nullptr;
            AstNode *duration = nullptr;
            AstNode *rate = nullptr;
    };

    class IndividualAdministration : public ObjectReferer, public PharmMLSection
    {
        public:
            IndividualAdministration(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            void gatherObjectRefs(std::unordered_map<std::string, Object *> &oidMap);
            xml::Node xml();
            ObjectRef *getOidRef();
            std::vector<ColumnMapping *> getColumnMappings();
            Dataset *getDataset();
            void accept(PharmMLVisitor *visitor);

        private:
            ObjectRef *oidRef;
            std::vector<pharmmlcpp::ColumnMapping *> columnMappings;
            Dataset *dataset;
    };

    class SingleIntervention
    {
        public:
            SingleIntervention(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            std::vector<ObjectRef *> getOidRefs();
            AstNode *getStart();
            AstNode *getEnd();

        private:
            std::vector<ObjectRef *> oidRefs;
            AstNode *start;
            AstNode *end;
    };

    class InterventionsCombination : public Object, public PharmMLSection
    {
        public:
            InterventionsCombination(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            std::vector<SingleIntervention *> getSingleInterventions();
            AstNode *getRelative();
            void gatherObjectRefs(std::unordered_map<std::string, Object *> &oidMap);
            void accept(ObjectVisitor *visitor);

        private:
            std::vector<SingleIntervention *> singleInterventions;
            AstNode *relative;
    };

    class Interventions
    {
        public:
            Interventions(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            xml::Node xml();
            std::vector<Administration *> getAdministrations();
            std::vector<IndividualAdministration *> getIndividualAdministrations();
            std::vector<InterventionsCombination *> getInterventionsCombinations();
            void setupRefererSymbRefs(SymbolGathering &gathering);
            void setupTargetMappings(SymbolGathering &gathering);
            void accept(PharmMLVisitor *visitor);

        private:
            std::vector<Administration *> administrations;
            std::vector<IndividualAdministration *> individualAdministrations;
            std::vector<InterventionsCombination *> interventionsCombinations;
    };
}

#endif
