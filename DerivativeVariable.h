#ifndef PHARMML_DERIVATIVEVARIABLE_H_
#define PHARMML_DERIVATIVEVARIABLE_H_

#include "PharmMLContext.h"
#include "Variable.h"

namespace PharmML
{
    class DerivativeVariable : public CommonVariable
    {
        AstNode *independent_variable;
        AstNode *initial_value;
        AstNode *initial_time;

        public:
        DerivativeVariable(PharmMLContext *context, xml::Node node) : CommonVariable(context, node) { this->parse(node); };
        AstNode *getIndependentVariable();
        AstNode *getInitialValue();
        AstNode *getInitialTime();
        void parse(xml::Node node);
        void accept(AbstractVisitor *visitor); 
    };
}

#endif
