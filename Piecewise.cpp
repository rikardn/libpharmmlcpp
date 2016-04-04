#include "Piecewise.h"

namespace PharmML
{
    Piece::Piece() {

    }

    void Piece::setExpression(AstNode *expression) {
        this->expression = expression;
    }

    void Piece::setCondition(AstNode *condition) {
        this->condition = condition;   
    }

    AstNode *Piece::getExpression() {
        return this->expression;
    }

    AstNode *Piece::getCondition() {
        return this->condition;
    }

    std::string Piece::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    Piecewise::Piecewise() {

    }

    void Piecewise::addPiece(Piece *piece) {
        this->pieces.push_back(piece);
    }

    std::vector<Piece *> Piecewise::getPieces() {
        return this->pieces;
    }

    std::string Piecewise::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
}
