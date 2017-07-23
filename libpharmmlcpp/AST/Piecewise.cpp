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

namespace pharmmlcpp
{
    Piece::Piece() {
    }

    xml::Node Piece::xml(PharmMLWriter &writer) {
        xml::Node piece("Piece", xml::Namespace::math);
        piece.addChild(this->expression->xml(writer));
        xml::Node condition("Condition", xml::Namespace::math);
        piece.addChild(condition);
        if (this->condition) {
            condition.addChild(this->condition->xml(writer));
        }       // FIXME: Otherwise
        return piece;
    }

    void Piece::setExpression(std::unique_ptr<AstNode> expression) {
        this->expression = std::move(expression);
    }

    void Piece::setCondition(std::unique_ptr<AstNode> condition) {
        this->condition = std::move(condition);
    }

    void Piece::setOtherwise() {
        otherwise = true;
    }

    AstNode* Piece::getExpression() {
        return this->expression.get();
    }

    AstNode* Piece::getCondition() {
        return this->condition.get();
    }

    bool Piece::isOtherwise() {
        return otherwise;
    }

    void Piece::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    std::unique_ptr<AstNode> Piece::clone() {
        std::unique_ptr<Piece> cl;
        return std::move(cl);
    }

    Piecewise::Piecewise() {
    }

    xml::Node Piecewise::xml(PharmMLWriter &writer) {
        xml::Node pw("Piecewise", xml::Namespace::math);
        for (const auto &piece : this->pieces) {
            pw.addChild(piece->xml(writer));
        }
        return pw;
    }

    void Piecewise::addPiece(Piece *piece) {
        this->pieces.push_back(piece);
    }

    std::vector<Piece *> Piecewise::getPieces() {
        return this->pieces;
    }

    void Piecewise::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    std::unique_ptr<AstNode> Piecewise::clone() {
        std::unique_ptr<Piecewise> cl;
        return std::move(cl);
    }
}
