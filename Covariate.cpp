#include <iostream>
#include "Covariate.h"
#include "PharmMLContext.h"
#include "AstNodeFactory.h"
#include "AstNode.h"

namespace PharmML
{
    Covariate::Covariate(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void Covariate::parse(xml::Node node) {
        xml::Node n = this->context->getSingleElement(node, ".//mdef:TransformedCovariate");
        if (n.exists()) {
            this->transformedName = n.getAttribute("symbId").getValue();
        }
        xml::Node assign = this->context->getSingleElement(node, ".//ct:Assign");
        xml::Node tree = assign.getChild();
        this->assignment = this->context->factory.create(tree);
    } 

    std::string Covariate::getTransformedName() {
        return this->transformedName;
    }

    AstNode *Covariate::getAssignment() {
        return this->assignment;
    }
    
    void Covariate::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
}
