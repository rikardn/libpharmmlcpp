#ifndef PHARMML_MODEL_H_
#define PHARMML_MODEL_H_

#include "PharmMLContext.h"
#include "FunctionDefinition.h"
#include "ModelDefinition.h"
#include "xml.h"

namespace PharmML
{
    class Model
    {
        PharmMLContext *context;
        std::vector<PharmML::FunctionDefinition *> FunctionDefinitions; 
        PharmML::ModelDefinition *ModelDefinition;

        private:
        void parse(xml::Node node);

        public:
        Model(const char *filename);
        ~Model();
        std::vector<PharmML::FunctionDefinition *> getFunctionDefinitions();
        PharmML::ModelDefinition *getModelDefinition();
    };
}

#endif
