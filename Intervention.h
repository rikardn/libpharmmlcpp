#ifndef PHARMML_INTERVENTION_H_
#define PHARMML_INTERVENTION_H_

#include "xml.h"
#include "PharmMLContext.h"
#include "AstNode.h"
#include "symbols.h"

namespace PharmML
{
    // TODO: Maybe move this somewhere?
    class SteadyStateParameter : public AstNode
    {
        AstNode *assignment = nullptr;
        SymbRef *symbRef = nullptr;

        protected:
        PharmML::PharmMLContext *context;

        public:
        SteadyStateParameter(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        AstNode *getAssignment();
        AstNode *getSymbRef();
        virtual std::string accept(AbstractVisitor *visitor);
    };
    
    // TODO: Move elsewhere (Dataset.h when implemented)
    // TODO: Subclass for MapType? Subclass for SymbRef? Container? It's a bit
    // weird to piggyback on AstNode instead of dedicated reference class
    // and it would need to be resolved
    class TargetMapping : public AstNode
    {
        PharmML::PharmMLContext *context;
        std::string type;
        std::string blkIdRef;
        std::string ref;
        
        public:
        TargetMapping(PharmMLContext *context, std::string type, xml::Node node);
        std::string getType();
        std::string getBlkIdRef();
        std::string getRef();
        virtual std::string accept(AbstractVisitor *visitor);
    };
    
    
    class Administration
    {
        PharmML::PharmMLContext *context;
        std::string oid;
        std::string type;
        AstNode *amount = nullptr;
        AstNode *target = nullptr; // Should probably be parent class of TargetMapping
        AstNode *times = nullptr;
        AstNode *steady = nullptr;
        AstNode *duration = nullptr;
        AstNode *rate = nullptr;
        
        public:
        Administration(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getOid();
        std::string getType();
        AstNode *getTarget();
        AstNode *getTimes();
        AstNode *getSteady();
        AstNode *getDuration();
        AstNode *getRate();
        std::string accept(AbstractVisitor *visitor);
    };
    
    class Intervention
    {
        PharmML::PharmMLContext *context;
        std::vector<PharmML::Administration *> Administrations;

        public:
        Intervention(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::vector<Administration *> getAdministrations();
    };
}

#endif
