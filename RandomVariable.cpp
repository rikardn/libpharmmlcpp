#include "RandomVariable.h"

namespace PharmML
{
    RandomVariable::RandomVariable(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void RandomVariable::parse(xml::Node node) {
        this->symbId = node.getAttribute("symbId").getValue();
        xml::Node n = this->context->getSingleElement(node, ".//ct:VariabilityReference");
        if (n.exists()) {
            this->VariabilityReference = this->context->factory.create(n.getChild());
        }
    }

    std::string RandomVariable::getSymbId() {
        return this->symbId;
    }

    AstNode *RandomVariable::getVariabilityReference() {
        return this->VariabilityReference;
    }

    std::string RandomVariable::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
}
