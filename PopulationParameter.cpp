#include "PopulationParameter.h"

namespace PharmML
{
    PopulationParameter::PopulationParameter(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void PopulationParameter::parse(xml::Node node) {
        this->symbId = node.getAttribute("symbId").getValue();
    }

    std::string PopulationParameter::getSymbId() {
        return this->symbId;
    }

    void PopulationParameter::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
}
