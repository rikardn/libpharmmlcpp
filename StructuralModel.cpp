#include "StructuralModel.h"

namespace PharmML
{
    StructuralModel::StructuralModel(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void StructuralModel::parse(xml::Node node) {
        std::vector<xml::Node> array = this->context->getElements(node, ".//ct:Variable");
        for (xml::Node n : array) {
            PharmML::Variable *var = new PharmML::Variable(this->context, n);
            this->variables.push_back(var);
        }
    }

    std::vector<PharmML::Variable *> StructuralModel::getVariables() {
        return this->variables;
    }
}
