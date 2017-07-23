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
    /**
     *  Set a new left child
     */
    void Binop::setLeft(std::unique_ptr<AstNode> child) {
        this->left = std::move(child);
    }

    /**
     *  Get a pointer to the left child
     */
    AstNode *Binop::getLeft() {
        return this->left.get();
    }

    /**
     *  Set a new right child
     */
    void Binop::setRight(std::unique_ptr<AstNode> child) {
        this->right = std::move(child);
    }

    /**
     *  Get a pointer to the right child
     */
    AstNode *Binop::getRight() {
        return this->right.get();
    }

#define BINOP_METHODS(name, type, op) \
    std::unique_ptr<AstNode> name::clone() { \
        std::unique_ptr<AstNode> left_clone = this->left->clone(); \
        std::unique_ptr<AstNode> right_clone = this->right->clone(); \
        std::unique_ptr<name> cl = std::make_unique<name>(std::move(left_clone), std::move(right_clone)); \
        return std::move(cl); \
    } \
    \
    void name::accept(AstNodeVisitor *visitor) { \
        visitor->visit(this); \
    } \
    xml::Node name::xml(PharmMLWriter &writer) { \
        xml::Node binop(type, xml::Namespace::math); \
        binop.setAttribute("op", op); \
        binop.addChild(this->left->xml(writer)); \
        binop.addChild(this->right->xml(writer)); \
        return binop; \
    }


    BINOP_METHODS(BinopPlus, "Binop", "plus")
    BINOP_METHODS(BinopMinus, "Binop", "minus")
    BINOP_METHODS(BinopDivide, "Binop", "divide")
    BINOP_METHODS(BinopTimes, "Binop", "times")
    BINOP_METHODS(BinopPower, "Binop", "power")
    BINOP_METHODS(BinopLogx, "Binop", "logx")
    BINOP_METHODS(BinopRoot, "Binop", "root")
    BINOP_METHODS(BinopMin, "Binop", "min")
    BINOP_METHODS(BinopMax, "Binop", "max")
    BINOP_METHODS(BinopRem, "Binop", "rem")
    BINOP_METHODS(BinopAtan2, "Binop", "atan2")
    BINOP_METHODS(LogicBinopLt, "LogicBinop", "lt")
    BINOP_METHODS(LogicBinopLeq, "LogicBinop", "leq")
    BINOP_METHODS(LogicBinopGt, "LogicBinop", "gt")
    BINOP_METHODS(LogicBinopGeq, "LogicBinop", "geq")
    BINOP_METHODS(LogicBinopEq, "LogicBinop", "eq")
    BINOP_METHODS(LogicBinopNeq, "LogicBinop", "neq")
    BINOP_METHODS(LogicBinopAnd, "LogicBinop", "and")
    BINOP_METHODS(LogicBinopOr, "LogicBinop", "or")
    BINOP_METHODS(LogicBinopXor, "LogicBinop", "xor")
}
