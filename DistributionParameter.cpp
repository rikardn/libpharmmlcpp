#include "DistributionParameter.h"
#include <iostream>

namespace PharmML
{
    DistributionParameter::DistributionParameter(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void DistributionParameter::parse(xml::Node node) {
        this->name = node.getAttribute("name").getValue();
        this->assignment = context->factory.create(node.getChild().getChild());
    }

    std::string DistributionParameter::getName() {
        return this->name;
    }

    AstNode *DistributionParameter::getAssignment() {
        return this->assignment;
    }
}
