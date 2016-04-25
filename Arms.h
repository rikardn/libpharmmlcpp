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

#include "Variable.h"
#include "symbols.h"
#include "xml.h"

namespace PharmML
{
    class VariabilityReference
    {
        PharmML::PharmMLContext *context;
        SymbRef *symbRef = nullptr;
        SymbRef *mappedSymbRef = nullptr;
        
        public:
        VariabilityReference(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        SymbRef *getSymbRef();
        SymbRef *getMappedSymbRef();
        //~ void accept(AstNodeVisitor *visitor);
    };
    
    class OccasionType
    {
        PharmML::PharmMLContext *context;
        AstNode *start = nullptr;
        AstNode *end = nullptr;
        
        public:
        OccasionType(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        AstNode *getStart();
        AstNode *getEnd();
        //~ void accept(AstNodeVisitor *visitor);
    };
    
    class InterventionSequence
    {
        PharmML::PharmMLContext *context;
        std::vector<std::string> oidRefs;
        AstNode *start = nullptr;
        
        public:
        InterventionSequence(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        xml::Node xml();
        std::vector<std::string> getOidRefs();
        AstNode *getStart();
        void accept(PharmMLVisitor *visitor);
    };
    
    class ObservationSequence
    {
        PharmML::PharmMLContext *context;
        std::vector<std::string> oidRefs;
        AstNode *start = nullptr;
        
        public:
        ObservationSequence(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        xml::Node xml();
        std::vector <std::string> getOidRefs();
        AstNode *getStart();
        void accept(PharmMLVisitor *visitor);
    };
    
    class OccasionSequence
    {
        PharmML::PharmMLContext *context;
        VariabilityReference *variabilityReference;
        std::vector<OccasionType *> occasions;
        
        public:
        OccasionSequence(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        VariabilityReference *getVariabilityReference();
        std::vector<OccasionType *> getOccasions();
        void accept(PharmMLVisitor *visitor);
    };
    
    class Arm
    {
        PharmML::PharmMLContext *context;
        std::string oid;
        std::string oidRef;
        AstNode *armSize = nullptr;
        AstNode *numSamples = nullptr;
        AstNode *numTimes = nullptr;
        AstNode *sameTimes = nullptr;
        std::vector<InterventionSequence *> interventionSequences;
        std::vector<ObservationSequence *> observationSequences;
        std::vector<OccasionSequence *> occasionSequences;
        
        public:
        Arm(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        xml::Node xml();
        std::string getOid();
        std::string getOidRef();
        AstNode *getArmSize();
        AstNode *getNumSamples();
        AstNode *getNumTimes();
        AstNode *getSameTimes();
        std::vector<InterventionSequence *> getInterventionSequences();
        std::vector<ObservationSequence *> getObservationSequences();
        void accept(PharmMLVisitor *visitor);
    };
    
    class Arms
    {
        PharmML::PharmMLContext *context;
        xml::Node xml_node;
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
        void update();
        void accept(PharmMLVisitor *visitor);
    };
}

#endif
