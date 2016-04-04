#ifndef PHARMML_ASTNODEFACTORY_H_
#define PHARMML_ASTNODEFACTORY_H_

#include "xml.h"
#include "AstNode.h"

namespace PharmML
{
    class AstNodeFactory
    {
        public:
        AstNode *create(xml::Node node);
    };
}

#endif
