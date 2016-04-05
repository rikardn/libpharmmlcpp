#ifndef PHARMML_PARAMETERMODEL_H_
#define PHARMML_PARAMETERMODEL_H_

#include "IndividualParameter.h"
#include "xml.h"

namespace PharmML
{
    class ParameterModel
    {
        PharmML::PharmMLContext *context;
        std::vector<IndividualParameter *> IndividualParameters;
        
        public:
        std::vector<IndividualParameter *> getIndividualParameters();
        ParameterModel(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
    };
}

#endif
