#ifndef PHARMML_OBSERVATIONS_H_
#define PHARMML_OBSERVATIONS_H_

#include "xml.h"
#include "PharmMLContext.h"
#include "AstNode.h"
#include "Variable.h"
#include "symbols.h"

namespace PharmML
{
    class Observation
    {
        PharmML::PharmMLContext *context;
        std::string oid;
        std::string oidRef;
        AstNode *number = nullptr;
        AstNode *times = nullptr;
        std::vector<PharmML::SymbRef *> continuousVariables;
        std::vector<PharmML::SymbRef *> discreteVariables;
        
        public:
        Observation(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getOid();
        std::string getOidRef();
        AstNode *getNumber();
        AstNode *getTimes();
        std::vector<PharmML::SymbRef *> getContinuousVariables();
        std::vector<PharmML::SymbRef *> getDiscreteVariables();
        std::string accept(AbstractVisitor *visitor);
    };
    
    class ObservationCombination
    {
        PharmML::PharmMLContext *context;
        std::string oid;
        std::vector<std::string> oidRefs;
        AstNode *relative = nullptr;
        
        public:
        ObservationCombination(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getOid();
        std::vector<std::string> getOidRefs();
        AstNode *getRelative();
        std::string accept(AbstractVisitor *visitor);
    };
    
    class Observations
    {
        PharmML::PharmMLContext *context;
        std::vector<PharmML::Variable *> designParameters;
        std::vector<PharmML::Observation *> observations;
        // TODO: Implement support for IndividualObservations according to schema
        // TODO: Implement support for LookupTable according to schema
        std::vector<ObservationCombination *> observationCombinations;

        public:
        Observations(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::vector<PharmML::Variable *> getDesignParameters();
        std::vector<PharmML::Observation *> getObservations();
        std::vector<PharmML::ObservationCombination *> getObservationCombinations();
        std::string accept(AbstractVisitor *visitor);
    };
}

#endif
