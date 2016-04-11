#include "LogicConditionType.h"

namespace PharmML
{
    // TODO: Move to constants when implemented
    std::string NullValue::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    //
    
    // Logic nullops (false and true)
    
    std::string LogicNullopFalse::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string LogicNullopTrue::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    // Logic uniops
    AstNode *LogicUniop::getChild() {
        return this->child;
    }

    void LogicUniop::setChild(AstNode *node) {
        delete child;
        this->child = node;
    }

    std::string LogicUniop::acceptChild(AbstractVisitor *visitor) {
        return this->child->accept(visitor);
    }
    
    std::string LogicUniopIsdefined::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string LogicUniopNot::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    // Logic binops
    void LogicBinop::setLeft(AstNode *child) {
        this->left = child;
    }

    AstNode *LogicBinop::getLeft() {
        return this->left;
    }

    void LogicBinop::setRight(AstNode *child) {
        this->right = child;
    }

    AstNode *LogicBinop::getRight() {
        return this->right;
    }

    std::string LogicBinop::acceptLeft(AbstractVisitor *visitor) {
        return this->left->accept(visitor);
    }

    std::string LogicBinop::acceptRight(AbstractVisitor *visitor) {
        return this->right->accept(visitor);
    }

    std::string LogicBinop::infix(AbstractVisitor *visitor, const char *op) {
        return "(" + this->acceptLeft(visitor) + op + this->acceptRight(visitor) + ")";
    }
    
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
