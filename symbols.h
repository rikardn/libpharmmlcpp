#ifndef PHARMML_SYMBOLS_H_
#define PHARMML_SYMBOLS_H_

#include <string>
#include "xml.h"
#include "AstNode.h"

namespace PharmML
{
    class SymbRef : public AstNode
    {
        std::string symbIdRef;

        public:
        SymbRef(std::string ref);
        SymbRef(xml::Node node);
        virtual std::string accept(AbstractVisitor *visitor);
        std::string toString();
    };

    class ColumnRef : public AstNode
    {
        std::string columnIdRef;

        public:
        ColumnRef(std::string ref);
        ColumnRef(xml::Node node);
        virtual std::string accept(AbstractVisitor *visitor);
        std::string toString();
    };
}


#endif
