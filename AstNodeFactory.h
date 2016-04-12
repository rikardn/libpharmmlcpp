#ifndef PHARMML_ASTNODEFACTORY_H_
#define PHARMML_ASTNODEFACTORY_H_

#include "xml.h"
#include "AstNode.h"
#include "Dependencies.h"

namespace PharmML
{
    class AstNodeFactory
    {
        public:
        AstNode *create(xml::Node node, Dependencies *deps = nullptr);
    };
}

#endif
