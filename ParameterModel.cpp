#include "ParameterModel.h"

namespace PharmML
{
    ParameterModel::ParameterModel(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void ParameterModel::parse(xml::Node node) {
        std::vector<xml::Node> ind_nodes = this->context->getElements(node, ".//mdef:IndividualParameter");
        for (xml::Node n : ind_nodes) {
            PharmML::IndividualParameter *ind = new PharmML::IndividualParameter(this->context, n);
            this->IndividualParameters.push_back(ind);
        }
    }

    std::vector<IndividualParameter *> ParameterModel::getIndividualParameters() {
        return this->IndividualParameters;
    }
}
