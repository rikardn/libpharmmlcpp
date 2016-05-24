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
    void BinopPlus::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void BinopMinus::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void BinopDivide::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void BinopTimes::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void BinopPower::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void BinopLogx::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void BinopRoot::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void BinopMin::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void BinopMax::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void BinopRem::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void BinopAtan2::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    // Logic binops
    void LogicBinopLt::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void LogicBinopLeq::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void LogicBinopGt::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void LogicBinopGeq::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void LogicBinopEq::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void LogicBinopNeq::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void LogicBinopAnd::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void LogicBinopOr::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    void LogicBinopXor::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }
}
