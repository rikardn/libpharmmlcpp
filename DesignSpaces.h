#ifndef PHARMML_DESIGNSPACES_H_
#define PHARMML_DESIGNSPACES_H_

#include "xml.h"
#include "PharmMLContext.h"
#include "Variable.h"

namespace PharmML
{
    class DesignSpace
    {
        PharmML::PharmMLContext *context;
        std::string oid;
        std::vector<std::string> interventionRefs;
        std::vector<std::string> observationRefs;
        std::vector<std::string> armRefs;
        
        public:
        DesignSpace(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getOid();
        std::vector<std::string> getInterventionRefs();
        std::vector<std::string> getObservationRefs();
        std::vector<std::string> getArmRefs();
        void accept(AbstractVisitor *visitor);
    };
    
    class DesignSpaces
    {
        PharmML::PharmMLContext *context;
        std::vector<PharmML::Variable *> designParameters;
        std::vector<PharmML::DesignSpace *> designSpaces;

        public:
        DesignSpaces(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::vector<PharmML::Variable *> getDesignParameters();
        std::vector<PharmML::DesignSpace *> getDesignSpaces();
        void accept(AbstractVisitor *visitor);
    };
}

#endif
