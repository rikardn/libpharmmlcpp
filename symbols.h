#ifndef PHARMML_SYMBOLS_H_
#define PHARMML_SYMBOLS_H_

#include <string>
#include "AstNode.h"

namespace PharmML
{
    class SymbRef : public AstNode
    {
        std::string symbIdRef;

        public:
        SymbRef(std::string ref);
        virtual std::string accept(AbstractVisitor *visitor);
        std::string toString();
    };
}

#endif
