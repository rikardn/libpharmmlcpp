#include "IndependentVariable.h"

namespace PharmML
{
    IndependentVariable::IndependentVariable(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void IndependentVariable::parse(xml::Node node) {
        this->symbId = node.getAttribute("symbId").getValue();
    } 

    std::string IndependentVariable::getSymbId() {
        return this->symbId;
    }

    void IndependentVariable::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
}
