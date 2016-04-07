#ifndef PHARMML_PARAMETERMODEL_H_
#define PHARMML_PARAMETERMODEL_H_

#include "PopulationParameter.h"
#include "IndividualParameter.h"
#include "xml.h"

namespace PharmML
{
    class ParameterModel
    {
        PharmML::PharmMLContext *context;
        std::vector<PopulationParameter *> PopulationParameters;
        std::vector<IndividualParameter *> IndividualParameters;
        
        public:
        std::vector<PopulationParameter *> getPopulationParameters();
        std::vector<IndividualParameter *> getIndividualParameters();
        ParameterModel(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
    };
}

#endif
