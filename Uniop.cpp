#include "Uniop.h"

namespace PharmML
{
    AstNode *Uniop::getChild() {
        return this->child;
    }

    void Uniop::setChild(AstNode *node) {
        delete child;
        this->child = node;
    }

    std::string Uniop::acceptChild(AbstractVisitor *visitor) {
        return this->child->accept(visitor);
    }

    std::string UniopLog::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    std::string UniopExp::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    std::string UniopMinus::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
}
