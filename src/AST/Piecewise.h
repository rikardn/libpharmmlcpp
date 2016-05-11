/* libpharmmlcpp - Library to handle PharmML
 * Copyright (C) 2016 Rikard Nordgren and Gunnar Yngman
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * 
 * his library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PHARMML_PIECEWISE_H_
#define PHARMML_PIECEWISE_H_

#include <vector>
#include <AST/AstNode.h>
#include <xml/xml.h>
#include <visitors/AstNodeVisitor.h>

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
        void accept(AstNodeVisitor *visitor);
    };

    class Piecewise : public AstNode
    {
        std::vector<Piece *> pieces;

        public:
        Piecewise();
        void addPiece(Piece *piece);
        std::vector<Piece *> getPieces();
        void accept(AstNodeVisitor *visitor);
    };
}

#endif