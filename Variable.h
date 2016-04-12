#ifndef PHARMML_VARIABLE_H_
#define PHARMML_VARIABLE_H_

#include "PharmMLContext.h"
#include "AbstractVisitor.h"

namespace PharmML
{
    class CommonVariable
    {
        Dependencies deps;
        AstNode *assignment = nullptr;
        std::string symbId;

        protected:
        PharmML::PharmMLContext *context;

        public:
        CommonVariable(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        AstNode *getAssignment();
        std::string getSymbId();
        virtual std::string accept(AbstractVisitor *visitor) = 0;
    };

    class Variable : public CommonVariable
    {
        public:
        Variable(PharmMLContext *context, xml::Node node) : CommonVariable(context, node) {};
        std::string accept(AbstractVisitor *visitor);
    };
}

#endif
