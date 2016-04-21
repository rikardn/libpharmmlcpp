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
    
    void Piece::setOtherwise() {
        otherwise = true;
    }

    AstNode *Piece::getExpression() {
        return this->expression;
    }

    AstNode *Piece::getCondition() {
        return this->condition;
    }
    
    bool Piece::isOtherwise() {
        return otherwise;
    }

    void Piece::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }

    Piecewise::Piecewise() {

    }

    void Piecewise::addPiece(Piece *piece) {
        this->pieces.push_back(piece);
    }

    std::vector<Piece *> Piecewise::getPieces() {
        return this->pieces;
    }

    void Piecewise::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
}
