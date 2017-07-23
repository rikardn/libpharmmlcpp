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
#include <PharmML/PharmMLWriter.h>

namespace pharmmlcpp
{
    /**
     * Base class for all binop AstNodes 
     */
    class Binop : public AstNode
    {
        public:
            Binop(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) { this->left = std::move(left); this->right = std::move(right); };
            Binop(const Binop &from) { this->left = from.left->clone(); this->right = from.right->clone(); };
            void setLeft(std::unique_ptr<AstNode> child);
            AstNode *getLeft();
            void setRight(std::unique_ptr<AstNode> child);
            AstNode *getRight();

        protected:
            std::unique_ptr<AstNode> left;
            std::unique_ptr<AstNode> right;
    };

    /**
     *  Addition
     */
    class BinopPlus : public Binop
    {
        public:
            BinopPlus(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : Binop(std::move(left), std::move(right)) {};
            BinopPlus(const BinopPlus &from) : Binop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Subtraction
     */
    class BinopMinus : public Binop
    {
        public:
            BinopMinus(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : Binop(std::move(left), std::move(right)) {};
            BinopMinus(const BinopMinus &from) : Binop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Division
     */
    class BinopDivide : public Binop
    {
        public:
            BinopDivide(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : Binop(std::move(left), std::move(right)) {};
            BinopDivide(const BinopDivide &from) : Binop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Multiplication
     */
    class BinopTimes : public Binop
    {
        public:
            BinopTimes(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : Binop(std::move(left), std::move(right)) {};
            BinopTimes(const BinopTimes &from) : Binop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Power
     */
    class BinopPower : public Binop
    {
        public:
            BinopPower(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : Binop(std::move(left), std::move(right)) {};
            BinopPower(const BinopPower &from) : Binop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  The x:th logarithm
     */
    class BinopLogx : public Binop
    {
        public:
            BinopLogx(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : Binop(std::move(left), std::move(right)) {};
            BinopLogx(const BinopLogx &from) : Binop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  The x:th root
     */
    class BinopRoot : public Binop
    {
        public:
            BinopRoot(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : Binop(std::move(left), std::move(right)) {};
            BinopRoot(const BinopRoot &from) : Binop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Minimum
     */
    class BinopMin : public Binop
    {
        public:
            BinopMin(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : Binop(std::move(left), std::move(right)) {};
            BinopMin(const BinopMin &from) : Binop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Maximum
     */
    class BinopMax : public Binop
    {
        public:
            BinopMax(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : Binop(std::move(left), std::move(right)) {};
            BinopMax(const BinopMax &from) : Binop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Remainder
     */
    class BinopRem : public Binop
    {
        public:
            BinopRem(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : Binop(std::move(left), std::move(right)) {};
            BinopRem(const BinopRem &from) : Binop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Atan2
     */
    class BinopAtan2 : public Binop
    {
        public:
            BinopAtan2(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : Binop(std::move(left), std::move(right)) {};
            BinopAtan2(const BinopAtan2 &from) : Binop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Base class for all logic binops
     */
    class LogicBinop : public Binop
    {
        public:
            LogicBinop(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : Binop(std::move(left), std::move(right)) {};
            LogicBinop(const LogicBinop &from) : Binop(from) {};
    };

    /**
     *  Less than
     */
    class LogicBinopLt : public LogicBinop
    {
        public:
            LogicBinopLt(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : LogicBinop(std::move(left), std::move(right)) {};
            LogicBinopLt(const LogicBinopLt &from) : LogicBinop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Less than or equal
     */
    class LogicBinopLeq : public LogicBinop
    {
        public:
            LogicBinopLeq(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : LogicBinop(std::move(left), std::move(right)) {};
            LogicBinopLeq(const LogicBinopLeq &from) : LogicBinop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Greater than
     */
    class LogicBinopGt : public LogicBinop
    {
        public:
            LogicBinopGt(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : LogicBinop(std::move(left), std::move(right)) {};
            LogicBinopGt(const LogicBinopGt &from) : LogicBinop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Greater than or equal
     */
    class LogicBinopGeq : public LogicBinop
    {
        public:
            LogicBinopGeq(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : LogicBinop(std::move(left), std::move(right)) {};
            LogicBinopGeq(const LogicBinopGeq &from) : LogicBinop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Equal
     */
    class LogicBinopEq : public LogicBinop
    {
        public:
            LogicBinopEq(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : LogicBinop(std::move(left), std::move(right)) {};
            LogicBinopEq(const LogicBinopEq &from) : LogicBinop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Not equal
     */
    class LogicBinopNeq : public LogicBinop
    {
        public:
            LogicBinopNeq(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : LogicBinop(std::move(left), std::move(right)) {};
            LogicBinopNeq(const LogicBinopNeq &from) : LogicBinop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Logic and
     */
    class LogicBinopAnd : public LogicBinop
    {
        public:
            LogicBinopAnd(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : LogicBinop(std::move(left), std::move(right)) {};
            LogicBinopAnd(const LogicBinopAnd &from) : LogicBinop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Logic or
     */
    class LogicBinopOr : public LogicBinop
    {
        public:
            LogicBinopOr(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : LogicBinop(std::move(left), std::move(right)) {};
            LogicBinopOr(const LogicBinopOr &from) : LogicBinop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Logic xor
     */
    class LogicBinopXor : public LogicBinop
    {
        public:
            LogicBinopXor(std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right) : LogicBinop(std::move(left), std::move(right)) {};
            LogicBinopXor(const LogicBinopXor &from) : LogicBinop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };
}

#endif
