#include "Distribution.h"

namespace PharmML
{
    Distribution::Distribution(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void Distribution::parse(xml::Node node) {
        this->name = node.getAttribute("name").getValue();
        //xml::Node n = this->context->getSingleElement(node, ".//ct:VariabilityReference");
        //if (n.exists()) {
        //    this->VariabilityReference = this->context->factory.create(n.getChild());
        //}
    }
        
    std::string Distribution::getName() {
        return this->name;
    }
    
    std::string Distribution::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
}
