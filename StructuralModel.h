#ifndef PHARMML_STRUCTURALMODEL_H_
#define PHARMML_STRUCTURALMODEL_H_

#include <vector>
#include "PharmMLContext.h"
#include "Variable.h"
#include "DerivativeVariable.h"

namespace PharmML
{
    class StructuralModel
    {
        PharmML::PharmMLContext *context;
        std::vector<PharmML::CommonVariable *> variables;

        public:
        StructuralModel(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::vector<PharmML::CommonVariable *> getVariables();
    };
}

#endif
