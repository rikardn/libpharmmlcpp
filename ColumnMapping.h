#ifndef PHARMML_COLUMNMAPPING_H_
#define PHARMML_COLUMNMAPPING_H_

#include "PharmMLContext.h"

namespace PharmML
{
    class ColumnMapping
    {
        PharmML::PharmMLContext *context;
        std::string columnIdRef;
        AstNode *assignment;

        public:
        ColumnMapping(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        AstNode *getAssignment();
        std::string getColumnIdRef();
        void accept(AbstractVisitor *visitor);
    };
}

#endif
