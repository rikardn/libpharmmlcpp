#ifndef PHARMML_DISTRIBUTION_H_
#define PHARMML_DISTRIBUTION_H_

#include "PharmMLContext.h"
#include "DistributionParameter.h"
#include "Scalar.h"
#include "symbols.h"

namespace PharmML
{
    class Distribution
    {
        PharmMLContext *context;
        std::string name;
        std::vector<PharmML::DistributionParameter *> parameters;

        public:
        Distribution(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getName();
        std::vector<PharmML::DistributionParameter *> getDistributionParameters();
        std::string accept(AbstractVisitor *visitor);
    };
}

#endif
