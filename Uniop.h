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
    
    // Ordinary uniops
    class UniopLog : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopLog2 : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopLog10 : public Uniop
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
    
    class UniopSqrt : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopLogistic : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopLogit : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopProbit : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopNormcdf : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopFactorial : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopFactln : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopGamma : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopGammaln : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopSin : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopSinh : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopCos : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopCosh : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopTan : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopTanh : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopCot : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopCoth : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopSec : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopSech : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopCsc : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopCsch : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopArcsin : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopArcsinh : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopArccos : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopArccosh : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopArctan : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopArctanh : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopArccot : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopArccoth : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopArcsec : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopArcsech : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopArccsc : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopArccsch : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopHeaviside : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopSign : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopFloor : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class UniopCeiling : public Uniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    // Logic uniops
    class LogicUniop : public Uniop {
        
    };
    
    class LogicUniopIsdefined : public LogicUniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class LogicUniopNot : public LogicUniop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
}

#endif
