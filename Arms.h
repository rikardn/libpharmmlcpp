#ifndef PHARMML_ARMS_H_
#define PHARMML_ARMS_H_

#include "PopulationParameter.h"
#include "IndividualParameter.h"
#include "RandomVariable.h"

#include "xml.h"

namespace PharmML
{
    class InterventionSequence
    {
        PharmML::PharmMLContext *context;
        std::vector<std::string> oidRefs;
        AstNode *start;
        
        public:
        InterventionSequence(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::vector <std::string> getOidRefs();
        AstNode *getStart();
        std::string accept(AbstractVisitor *visitor);
    };
    
    class ObservationSequence
    {
        PharmML::PharmMLContext *context;
        std::vector<std::string> oidRefs;
        AstNode *start;
        
        public:
        ObservationSequence(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::vector <std::string> getOidRefs();
        AstNode *getStart();
        std::string accept(AbstractVisitor *visitor);
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
        //~ std::vector<OccasionSequence *> occasionSequences;
        
        public:
        Arm(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getOid();
        std::string getOidRef();
        AstNode *getArmSize();
        AstNode *getNumSamples();
        AstNode *getNumTimes();
        AstNode *getSameTimes();
        std::vector<InterventionSequence *> getInterventionSequences();
        std::string accept(AbstractVisitor *visitor);
    };
    
    class Arms
    {
        PharmML::PharmMLContext *context;
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
        std::string accept(AbstractVisitor *visitor);
    };
}

#endif
