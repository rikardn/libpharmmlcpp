#include <iostream>
#include "Binop.h"

namespace PharmML
{
    void Binop::setLeft(AstNode *child) {
        this->left = child;
    }

    AstNode *Binop::getLeft() {
        return this->left;
    }

    void Binop::setRight(AstNode *child) {
        this->right = child;
    }
    
    AstNode *Binop::getRight() {
        return this->right;
    }

    std::string Binop::acceptLeft(AbstractVisitor *visitor) {
        return this->left->accept(visitor);
    }

    std::string Binop::acceptRight(AbstractVisitor *visitor) {
        return this->right->accept(visitor);
    }

    std::string Binop::infix(AbstractVisitor *visitor, const char *op) {
        return "(" + this->acceptLeft(visitor) + op + this->acceptRight(visitor) + ")";
    }
    
    // Ordinary binops
    std::string BinopPlus::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    std::string BinopMinus::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    std::string BinopDivide::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    std::string BinopTimes::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string BinopPower::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string BinopLogx::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string BinopRoot::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string BinopMin::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string BinopMax::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string BinopRem::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string BinopAtan2::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    // Logic binops
    std::string LogicBinopLt::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    std::string LogicBinopLeq::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string LogicBinopGt::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string LogicBinopGeq::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string LogicBinopEq::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string LogicBinopNeq::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string LogicBinopAnd::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string LogicBinopOr::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string LogicBinopXor::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
}
