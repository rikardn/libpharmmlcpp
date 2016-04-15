#ifndef PHARMML_CONSTANT_H_
#define PHARMML_CONSTANT_H_

#include <string>
#include "AstNode.h"
#include "AbstractVisitor.h"

namespace PharmML
{
    class Constant : public AstNode
    {

    };
    
    // Null value (example usage: missing otherwise piece)
    class NullValue : public Constant
    {
        public:
            virtual void accept(AbstractVisitor *visitor);
    };
    
    // Boolean primitives
    class LogicFalse : public Constant
    {
        public:
            virtual void accept(AbstractVisitor *visitor);
    };
    
    class LogicTrue : public Constant
    {
        public:
            virtual void accept(AbstractVisitor *visitor);
    };
    
    // Pi and Euler's constant
    class Pi : public Constant
    {
        public:
            virtual void accept(AbstractVisitor *visitor);
    };
    
    class Exponentiale : public Constant
    {
        public:
            virtual void accept(AbstractVisitor *visitor);
    };
}

#endif
