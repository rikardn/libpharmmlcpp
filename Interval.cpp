#include "Interval.h"

namespace PharmML
{
    AstNode *Interval::getLeftEndpoint() {
        return this->LeftEndpoint;
    }

    AstNode *Interval::getRightEndpoint() {
        return this->RightEndpoint;
    }
        
    void Interval::setLeftEndpoint(AstNode *node) {
        this->LeftEndpoint = node;
    }

    void Interval::setRightEndpoint(AstNode *node) {
        this->RightEndpoint = node;
    }

    bool Interval::isLeftEndpointOpen() {
        return this->openLeftEndpoint;
    }

    bool Interval::isRightEndpointOpen() {
        return this->openRightEndpoint;
    }

    void Interval::setLeftEndpointOpen(bool open) {
        this->openLeftEndpoint = open;
    }

    void Interval::setRightEndpointOpen(bool open) {
        this->openRightEndpoint = open;
    }

    void Interval::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
}
