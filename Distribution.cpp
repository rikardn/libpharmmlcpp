#include "Distribution.h"
#include <iostream>

namespace PharmML
{
    Distribution::Distribution(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void Distribution::parse(xml::Node node) {
        this->name = node.getAttribute("name").getValue();
        std::vector<xml::Node> params = this->context->getElements(node, ".//po:Parameter");
        for (xml::Node n : params) {
            this->parameters.push_back(new PharmML::DistributionParameter(this->context, n));
        }
    }
        
    std::string Distribution::getName() {
        return this->name;
    }
    
    std::vector<PharmML::DistributionParameter *> Distribution::getDistributionParameters() {
        return this->parameters;
    }

    std::string Distribution::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
}
