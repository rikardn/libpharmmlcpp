#ifndef PHARMML_LOGICCONDITIONTYPE_H_
#define PHARMML_LOGICCONDITIONTYPE_H_

#include <string>
#include "AstNode.h"
#include "AbstractVisitor.h"

namespace PharmML
{
    class LogicConditionType : public AstNode
    {
        
    };
    
    // Logic nullops (false and true)
    class LogicNullop : public LogicConditionType
    {
    
    };
    
    class LogicNullopFalse : public LogicNullop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    class LogicNullopTrue : public LogicNullop
    {
        public:
            virtual std::string accept(AbstractVisitor *visitor);
    };
    
    // Logic uniops
    class LogicUniop : public LogicConditionType
    {
        AstNode *child = nullptr;
        
        public:
        AstNode *getChild();
        void setChild(AstNode *node);
        std::string acceptChild(AbstractVisitor *visitor);
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
    
    // Logic binops
    class LogicBinop : public LogicConditionType
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
