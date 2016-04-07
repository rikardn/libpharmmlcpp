#ifndef PHARMML_BINOP_H_
#define PHARMML_BINOP_H_

#include "AstNode.h"
#include "AbstractVisitor.h"

namespace PharmML
{
    class Binop : public AstNode
    {
        AstNode *left;
        AstNode *right;

        public:
        void setLeft(AstNode *child);
        AstNode *getLeft();
        void setRight(AstNode *child);
        AstNode *getRight();
        std::string acceptLeft(AbstractVisitor *visitor);
        std::string acceptRight(AbstractVisitor *visitor);
        std::string infix(AbstractVisitor *visitor, const char *op);
    };

    class BinopPlus : public Binop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };

    class BinopMinus : public Binop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };

    class BinopDivide : public Binop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };

    class BinopTimes : public Binop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class BinopPower : public Binop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class BinopLogx : public Binop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class BinopRoot : public Binop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class BinopMin : public Binop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class BinopMax : public Binop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class BinopRem : public Binop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class BinopAtan2 : public Binop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };

    class LogicBinop : public Binop
    {

    };

    class LogicBinopLt : public LogicBinop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };

    class LogicBinopLeq : public LogicBinop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class LogicBinopGt : public LogicBinop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class LogicBinopGeq : public LogicBinop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class LogicBinopEq : public LogicBinop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class LogicBinopNeq : public LogicBinop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class LogicBinopAnd : public LogicBinop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class LogicBinopOr : public LogicBinop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class LogicBinopXor : public LogicBinop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
}

#endif
