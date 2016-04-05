#ifndef PHARMML_INDIVIDUALPARAMETER_H_
#define PHARMML_INDIVIDUALPARAMETER_H_

#include "PharmMLContext.h"
#include "xml.h"
#include "AstNode.h"

namespace PharmML
{
    class IndividualParameter
    {
        PharmMLContext *context;
        std::string symbId;
        std::string transformation;
        AstNode *PopulationValue;
        AstNode *RandomEffects;
        AstNode *FixedEffect = nullptr;
        AstNode *Covariate = nullptr;

        public:
        IndividualParameter(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getSymbId();
        std::string getTransformation();
        AstNode *getPopulationValue();
        AstNode *getRandomEffects();
        AstNode *getFixedEffect();
        AstNode *getCovariate();
        std::string accept(AbstractVisitor *visitor);
    };
}

#endif
