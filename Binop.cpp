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

    // Ordinary binops
    void BinopPlus::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }

    void BinopMinus::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }

    void BinopDivide::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }

    void BinopTimes::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    void BinopPower::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    void BinopLogx::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    void BinopRoot::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    void BinopMin::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    void BinopMax::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    void BinopRem::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    void BinopAtan2::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    // Logic binops
    void LogicBinopLt::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }

    void LogicBinopLeq::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    void LogicBinopGt::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    void LogicBinopGeq::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    void LogicBinopEq::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    void LogicBinopNeq::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    void LogicBinopAnd::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    void LogicBinopOr::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    void LogicBinopXor::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
}
