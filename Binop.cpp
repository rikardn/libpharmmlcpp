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
        return this->acceptLeft(visitor) + op + this->acceptRight(visitor);
    }

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

    std::string LogicBinopLt::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    std::string LogicBinopLeq::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

}
