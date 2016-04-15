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
        xml::Node dist_node = this->context->getSingleElement(node, ".//mdef:Distribution");
        if (dist_node.exists()) {
            this->Distribution = new PharmML::Distribution(context, dist_node.getChild());
        }

    }

    std::string RandomVariable::getSymbId() {
        return this->symbId;
    }

    AstNode *RandomVariable::getVariabilityReference() {
        return this->VariabilityReference;
    }

    PharmML::Distribution *RandomVariable::getDistribution() {
        return this->Distribution;
    }

    void RandomVariable::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
}
