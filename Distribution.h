#ifndef PHARMML_DISTRIBUTION_H_
#define PHARMML_DISTRIBUTION_H_

#include "PharmMLContext.h"

namespace PharmML
{
    class Distribution
    {
        PharmMLContext *context;
        std::string name;

        public:
        Distribution(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getName();
        std::string accept(AbstractVisitor *visitor);
    };
}

#endif
