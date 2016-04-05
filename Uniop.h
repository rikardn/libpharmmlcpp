#ifndef PHARMML_UNIOP_H_
#define PHARMML_UNIOP_H_

#include <string>
#include "Uniop.h"
#include "AstNode.h"
#include "AbstractVisitor.h"

namespace PharmML
{
    class Uniop : public AstNode
    {
        AstNode *child = nullptr;

        public:
        AstNode *getChild();
        void setChild(AstNode *node);
        std::string acceptChild(AbstractVisitor *visitor);
    };

    class UniopLog : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };

    class UniopExp : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };

    class UniopMinus : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopAbs : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
}

#endif
