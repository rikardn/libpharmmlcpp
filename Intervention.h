#ifndef PHARMML_INTERVENTION_H_
#define PHARMML_INTERVENTION_H_

#include "xml.h"
#include "PharmMLContext.h"
#include "AstNode.h"

namespace PharmML
{
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
        virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class Administration
    {
        PharmML::PharmMLContext *context;
        std::string oid;
        std::string type;
        AstNode *amount;
        std::string targetType;
        AstNode *target; // Should probably be mother class of TargetMapping
        AstNode *times;
        AstNode *steady;
        AstNode *duration;
        AstNode *rate;
        
        public:
        Administration(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        //~ AstNode *getAmount();
        //~ AstNode *getTimes();
        //~ AstNode *getTarget();
        //~ std::string accept(AbstractVisitor *visitor);
    };
    
    class Intervention
    {
        PharmML::PharmMLContext *context;
        std::vector<PharmML::Administration *> Administrations;

        public:
        Intervention(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        //~ std::string accept(AbstractVisitor *visitor);
    };
}

#endif
