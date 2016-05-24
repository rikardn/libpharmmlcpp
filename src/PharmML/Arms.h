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

#ifndef PHARMML_ARMS_H_
#define PHARMML_ARMS_H_

#include <visitors/ObjectVisitor.h>
#include <symbols/Variable.h>
#include <AST/symbols.h>
#include <xml/xml.h>
#include <symbols/VariabilityLevel.h>
#include <objects/Object.h>
#include <PharmML/PharmMLSection.h>

namespace PharmML
{
    class OccasionType
    {
        public:
            OccasionType(PharmML::PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            AstNode *getStart();
            AstNode *getEnd();
 
        private:
            PharmML::PharmMLContext *context;
            AstNode *start = nullptr;
            AstNode *end = nullptr;
            //~ void accept(AstNodeVisitor *visitor);
    };

    class InterventionSequence
    {
        public:
            InterventionSequence(PharmML::PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            xml::Node xml();
            std::vector<ObjectRef *> getOidRefs();
            AstNode *getStart();
            void accept(PharmMLVisitor *visitor);

        private:
            PharmML::PharmMLContext *context;
            std::vector<ObjectRef *> oidRefs;
            AstNode *start = nullptr;
    };

    class ObservationSequence
    {
        public:
            ObservationSequence(PharmML::PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            xml::Node xml();
            std::vector<ObjectRef *> getOidRefs();
            AstNode *getStart();
            void accept(PharmMLVisitor *visitor);

        private:
            PharmML::PharmMLContext *context;
            std::vector<ObjectRef *> oidRefs;
            AstNode *start = nullptr;
    };

    class OccasionSequence
    {
        public:
            OccasionSequence(PharmML::PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            VariabilityReference *getVariabilityReference();
            std::vector<OccasionType *> getOccasions();
            void accept(PharmMLVisitor *visitor);
        
        private:
            PharmML::PharmMLContext *context;
            VariabilityReference *variabilityReference;
            std::vector<OccasionType *> occasions;
    };

    class Arm : public Object, public PharmMLSection
    {
        public:
            Arm(PharmML::PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            void gatherObjectRefs(std::unordered_map<std::string, Object *> &oidMap);
            xml::Node xml();
            std::string getOidRef();
            AstNode *getArmSize();
            AstNode *getNumSamples();
            AstNode *getNumTimes();
            AstNode *getSameTimes();
            std::vector<InterventionSequence *> getInterventionSequences();
            std::vector<ObservationSequence *> getObservationSequences();
            void accept(PharmMLVisitor *visitor);   // FIXME: Remove this
            void accept(ObjectVisitor *visitor);

        private:
            PharmML::PharmMLContext *context;
            std::string oidRef;
            AstNode *armSize = nullptr;
            AstNode *numSamples = nullptr;
            AstNode *numTimes = nullptr;
            AstNode *sameTimes = nullptr;
            std::vector<InterventionSequence *> interventionSequences;
            std::vector<ObservationSequence *> observationSequences;
            std::vector<OccasionSequence *> occasionSequences;
    };

    class Arms
    {
        public:
            Arms(PharmML::PharmMLContext *context, xml::Node node);
            std::vector<PharmML::Variable *> getDesignParameters();
            AstNode *getArmSize();
            AstNode *getCostFunction();
            AstNode *getNumArms();
            AstNode *getNumSamples();
            AstNode *getNumTimes();
            AstNode *getSameTimes();
            AstNode *getTotalCost();
            AstNode *getTotalSize();
            std::vector<Arm *> getArms();
            void parse(xml::Node node);
            xml::Node xml();
            void accept(PharmMLVisitor *visitor);

        private:
            PharmML::PharmMLContext *context;
            std::vector<PharmML::Variable *> designParameters;
            AstNode *armSize = nullptr;
            AstNode *costFunction = nullptr;
            AstNode *numArms = nullptr;
            AstNode *numSamples = nullptr;
            AstNode *numTimes = nullptr;
            AstNode *sameTimes = nullptr;
            AstNode *totalCost = nullptr;
            AstNode *totalSize = nullptr;
            std::vector<Arm *> arms;
    };
}

#endif
