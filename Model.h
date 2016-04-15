#ifndef PHARMML_MODEL_H_
#define PHARMML_MODEL_H_

#include "PharmMLContext.h"
#include "IndependentVariable.h"
#include "FunctionDefinition.h"
#include "ModelDefinition.h"
#include "TrialDesign.h"
#include "xml.h"

namespace PharmML
{
    class Model
    {
        PharmMLContext *context;
        PharmML::IndependentVariable *IndependentVariable;
        std::vector<PharmML::FunctionDefinition *> FunctionDefinitions; 
        PharmML::ModelDefinition *ModelDefinition;
        PharmML::TrialDesign *TrialDesign;

        private:
        void parse(xml::Node node);

        public:
        Model(const char *filename);
        ~Model();
        void write(const char *filename);
        PharmML::IndependentVariable *getIndependentVariable();
        std::vector<PharmML::FunctionDefinition *> getFunctionDefinitions();
        PharmML::ModelDefinition *getModelDefinition();
        PharmML::TrialDesign *getTrialDesign();
    };
}

#endif
