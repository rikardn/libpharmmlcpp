#ifndef PHARMML_POPULATIONPARAMETER_H_
#define PHARMML_POPULATIONPARAMETER_H_

#include "PharmMLContext.h"
#include "xml.h"

namespace PharmML
{
    class PopulationParameter
    {
        PharmMLContext *context;
        std::string symbId;

        public:
        PopulationParameter(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getSymbId();
        std::string accept(AbstractVisitor *visitor);
    };
}

#endif
