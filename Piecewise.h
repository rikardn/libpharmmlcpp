#ifndef PHARMML_PIECEWISE_H_
#define PHARMML_PIECEWISE_H_

#include <vector>
#include "AstNode.h"
#include "xml.h"
#include "AbstractVisitor.h"

namespace PharmML
{
    class Piece : public AstNode
    {
        AstNode *expression;
        AstNode *condition;
        bool otherwise = false;

        public:
        Piece();
        void setExpression(AstNode *expression);
        void setCondition(AstNode *condition);
        void setOtherwise();
        AstNode *getExpression();
        AstNode *getCondition();
        bool isOtherwise();
        std::string accept(AbstractVisitor *visitor);
    };

    class Piecewise : public AstNode
    {
        std::vector<Piece *> pieces;

        public:
        Piecewise();
        void addPiece(Piece *piece);
        std::vector<Piece *> getPieces();
        std::string accept(AbstractVisitor *visitor);
    };
}

#endif
