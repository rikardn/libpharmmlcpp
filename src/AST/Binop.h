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

#ifndef PHARMMLCPP_BINOP_H_
#define PHARMMLCPP_BINOP_H_

#include <AST/AstNode.h>
#include <visitors/AstNodeVisitor.h>

namespace pharmmlcpp
{
    class Binop : public AstNode
    {
        public:
            void setLeft(std::unique_ptr<AstNode> child);
            AstNode *getLeft();
            void setRight(std::unique_ptr<AstNode> child);
            AstNode *getRight();

        protected:
            std::unique_ptr<AstNode> left;
            std::unique_ptr<AstNode> right;
    };

    // Ordinary binops
    class BinopPlus : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class BinopMinus : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class BinopDivide : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class BinopTimes : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class BinopPower : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class BinopLogx : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class BinopRoot : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class BinopMin : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class BinopMax : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class BinopRem : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class BinopAtan2 : public Binop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    // Logic binops
    class LogicBinop : public Binop
    {

    };

    class LogicBinopLt : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class LogicBinopLeq : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class LogicBinopGt : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class LogicBinopGeq : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class LogicBinopEq : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class LogicBinopNeq : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class LogicBinopAnd : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class LogicBinopOr : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class LogicBinopXor : public LogicBinop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };
}

#endif
