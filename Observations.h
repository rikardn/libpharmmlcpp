#ifndef PHARMML_OBSERVATIONS_H_
#define PHARMML_OBSERVATIONS_H_

#include "xml.h"
#include "PharmMLContext.h"
#include "AstNode.h"
#include "Variable.h"
#include "symbols.h"
#include "ColumnMapping.h"
//~ #include "ExternalDataset.h" // Check comments in this file

namespace PharmML
{
    // Observation class
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
        void accept(AbstractVisitor *visitor);
    };
    
    // IndividualObservations class
    class IndividualObservations
    {
        PharmML::PharmMLContext *context;
        std::string oid;
        std::vector<PharmML::ColumnMapping *> ColumnMappings;
        // TODO: Support ds:Dataset (data for each subject within the study)

        public:
        IndividualObservations(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getOid();
        std::vector<PharmML::ColumnMapping *> getColumnMappings();
        void accept(AbstractVisitor *visitor);
    };
    
    // ObservationCombination class
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
        void accept(AbstractVisitor *visitor);
    };
    
    // Observations class (contains objects of classes above)
    class Observations
    {
        PharmML::PharmMLContext *context;
        std::vector<PharmML::Variable *> designParameters;
        std::vector<PharmML::Observation *> simulationObservations;
        std::vector<PharmML::IndividualObservations *> datasetObservations;
        // TODO: Implement support for LookupTable according to schema
        std::vector<ObservationCombination *> observationCombinations;

        public:
        Observations(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::vector<PharmML::Variable *> getDesignParameters();
        std::vector<PharmML::Observation *> getObservations(); // Or getSimulationObservations()?
        std::vector<PharmML::IndividualObservations *> getIndividualObservations(); // Or getDatasetObservations()?
        std::vector<PharmML::ObservationCombination *> getObservationCombinations();
        void accept(AbstractVisitor *visitor);
    };
}

#endif
