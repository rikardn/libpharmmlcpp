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

        xml::Node rand = this->context->getSingleElement(node, ".//mdef:StructuredModel/mdef:RandomEffects/ct:SymbRef");
        if (rand.exists()) {
            this->RandomEffects = this->context->factory.create(rand);
        }

        xml::Node fixed = this->context->getSingleElement(node, ".//mdef:StructuredModel/mdef:LinearCovariate/mdef:Covariate/mdef:FixedEffect/ct:SymbRef");
        if (fixed.exists()) {
            this->FixedEffect = this->context->factory.create(fixed);
        }

        xml::Node cov = this->context->getSingleElement(node, ".//mdef:StructuredModel/mdef:LinearCovariate/mdef:Covariate/ct:SymbRef");
        if (cov.exists()) {
            this->Covariate = this->context->factory.create(cov);
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
    
    AstNode *IndividualParameter::getCovariate() {
        return this->Covariate;
    }

    AstNode *IndividualParameter::getRandomEffects() {
        return this->RandomEffects;
    }

    AstNode *IndividualParameter::getFixedEffect() {
        return this->FixedEffect;
    }

    std::string IndividualParameter::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
}
