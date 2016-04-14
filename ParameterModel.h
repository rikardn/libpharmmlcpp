#ifndef PHARMML_PARAMETERMODEL_H_
#define PHARMML_PARAMETERMODEL_H_

#include "PopulationParameter.h"
#include "IndividualParameter.h"
#include "RandomVariable.h"

#include "xml.h"

namespace PharmML
{
    class ParameterModel
    {
        PharmML::PharmMLContext *context;
        std::string blkId;
        std::vector<PopulationParameter *> PopulationParameters;
        std::vector<IndividualParameter *> IndividualParameters;
        std::vector<RandomVariable *> RandomVariables;
        
        public:
        std::vector<PopulationParameter *> getPopulationParameters();
        std::vector<IndividualParameter *> getIndividualParameters();
        std::vector<RandomVariable *> getRandomVariables();
        ParameterModel(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
    };
}

#endif
