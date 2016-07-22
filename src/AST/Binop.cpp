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

namespace pharmmlcpp
{
    void Binop::setLeft(std::unique_ptr<AstNode> child) {
        this->left = std::move(child);
    }

    AstNode *Binop::getLeft() {
        return this->left.get();
    }

    void Binop::setRight(std::unique_ptr<AstNode> child) {
        this->right = std::move(child);
    }

    AstNode *Binop::getRight() {
        return this->right.get();
    }

#define BINOP_METHODS(name) \
    std::unique_ptr<AstNode> name::clone() { \
        std::unique_ptr<name> cl = std::make_unique<name>(); \
        (*cl).left = this->left->clone(); \
        (*cl).right = this->right->clone(); \
        return std::move(cl); \
    } \
    \
    void name::accept(AstNodeVisitor *visitor) { \
        visitor->visit(this); \
    }

    BINOP_METHODS(BinopPlus)
    BINOP_METHODS(BinopMinus)
    BINOP_METHODS(BinopDivide)
    BINOP_METHODS(BinopTimes)
    BINOP_METHODS(BinopPower)
    BINOP_METHODS(BinopLogx)
    BINOP_METHODS(BinopRoot)
    BINOP_METHODS(BinopMin)
    BINOP_METHODS(BinopMax)
    BINOP_METHODS(BinopRem)
    BINOP_METHODS(BinopAtan2)
    BINOP_METHODS(LogicBinopLt)
    BINOP_METHODS(LogicBinopLeq)
    BINOP_METHODS(LogicBinopGt)
    BINOP_METHODS(LogicBinopGeq)
    BINOP_METHODS(LogicBinopEq)
    BINOP_METHODS(LogicBinopNeq)
    BINOP_METHODS(LogicBinopAnd)
    BINOP_METHODS(LogicBinopOr)
    BINOP_METHODS(LogicBinopXor)
}
