#ifndef PHARMML_ASTNODE_H_
#define PHARMML_ASTNODE_H_

#include <string>
#include "AbstractVisitor.h"

namespace PharmML
{
    class AstNode
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor) = 0;
    };
}

#endif
