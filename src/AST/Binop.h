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

#ifndef PHARMML_BINOP_H_
#define PHARMML_BINOP_H_

#include <AST/AstNode.h>
#include <visitors/AstNodeVisitor.h>

namespace PharmML
{
    class Binop : public AstNode
    {
        AstNode *left;
        AstNode *right;

        public:
        void setLeft(AstNode *child);
        AstNode *getLeft();
        void setRight(AstNode *child);
        AstNode *getRight();
    };

    // Ordinary binops
    class BinopPlus : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class BinopMinus : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class BinopDivide : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class BinopTimes : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class BinopPower : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class BinopLogx : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class BinopRoot : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class BinopMin : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class BinopMax : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class BinopRem : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class BinopAtan2 : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    // Logic binops
    class LogicBinop : public Binop
    {

    };

    class LogicBinopLt : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class LogicBinopLeq : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class LogicBinopGt : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class LogicBinopGeq : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class LogicBinopEq : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class LogicBinopNeq : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class LogicBinopAnd : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class LogicBinopOr : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class LogicBinopXor : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };
}

#endif
