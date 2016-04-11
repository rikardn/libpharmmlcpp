#include "DistributionParameter.h"
#include <iostream>

namespace PharmML
{
    DistributionParameter::DistributionParameter(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    DistributionParameter::DistributionParameter(PharmMLContext *context) {
        this->context = context;
    }

    void DistributionParameter::parse(xml::Node node) {
        this->name = node.getAttribute("name").getValue();
        this->assignment = context->factory.create(node.getChild().getChild());
    }

    std::string DistributionParameter::getName() {
        return this->name;
    }

    void DistributionParameter::setName(std::string name) {
        this->name = name;
    }

    AstNode *DistributionParameter::getAssignment() {
        return this->assignment;
    }

    void DistributionParameter::setAssignment(AstNode *assignment) {
        this->assignment = assignment;
    }
}
