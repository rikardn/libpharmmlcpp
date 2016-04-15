#include "DerivativeVariable.h"
#include <iostream>

namespace PharmML
{
    void DerivativeVariable::parse(xml::Node node) {
        xml::Node iv = this->context->getSingleElement(node, ".//ct:IndependentVariable");
        if (iv.exists()) {
            this->independent_variable = this->context->factory.create(iv.getChild(), &this->deps);
        }
        xml::Node ival = this->context->getSingleElement(node, ".//ct:InitialCondition/ct:InitialValue");
        if (ival.exists()) {
            this->initial_value = this->context->factory.create(ival.getChild().getChild(), &this->deps);
        }
        xml::Node itime = this->context->getSingleElement(node, ".//ct:InitialCondition/ct:InitialTime");
        if (itime.exists()) {
            this->initial_time = this->context->factory.create(itime.getChild().getChild(), &this->deps);
        }
    }

    AstNode *DerivativeVariable::getIndependentVariable() {
        return this->independent_variable;
    }
    
    AstNode *DerivativeVariable::getInitialValue() {
        return this->initial_value;
    }
    
    AstNode *DerivativeVariable::getInitialTime() {
        return this->initial_time;
    }

    void DerivativeVariable::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
}
