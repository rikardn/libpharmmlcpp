#ifndef PHARMML_MODEL_H_
#define PHARMML_MODEL_H_

#include "PharmMLContext.h"
#include "ModelDefinition.h"
#include "xml.h"

namespace PharmML
{
    class Model
    {
        PharmMLContext *context;
        PharmML::ModelDefinition *ModelDefinition;

        private:
        void parse(xml::Node node);

        public:
        Model(const char *filename);
        ~Model();
        PharmML::ModelDefinition *getModelDefinition();
    };
}

#endif
