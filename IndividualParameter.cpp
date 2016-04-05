#include "IndividualParameter.h"
#include <iostream>

namespace PharmML
{
    IndividualParameter::IndividualParameter(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void IndividualParameter::parse(xml::Node node) {
        this->symbId = node.getAttribute("symbId").getValue();
        xml::Node trans = this->context->getSingleElement(node, ".//mdef:StructuredModel/mdef:Transformation");
        if (trans.exists()) {
            this->transformation = trans.getAttribute("type").getValue();
        }
        xml::Node pop = this->context->getSingleElement(node, ".//mdef:StructuredModel/mdef:LinearCovariate/mdef:PopulationValue/ct:Assign");
        if (!pop.exists()) {
            pop = this->context->getSingleElement(node, ".//mdef:StructuredModel/mdef:PopulationValue/ct:Assign");
        }
        if (pop.exists()) {
            this->PopulationValue = this->context->factory.create(pop.getChild());
        }
    }

    std::string IndividualParameter::getSymbId() {
        return this->symbId;
    }

    std::string IndividualParameter::getTransformation() {
        return this->transformation;
    }

    AstNode *IndividualParameter::getPopulationValue() {
        return this->PopulationValue;
    }

    std::string IndividualParameter::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
}
