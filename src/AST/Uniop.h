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

#ifndef PHARMMLCPP_UNIOP_H_
#define PHARMMLCPP_UNIOP_H_

#include <memory>
#include <string>
#include <AST/AstNode.h>
#include <visitors/AstNodeVisitor.h>

namespace pharmmlcpp
{
    class Uniop : public AstNode
    {
        public:
            AstNode *getChild();
            void setChild(std::unique_ptr<AstNode> node);

        protected:
            std::unique_ptr<AstNode> child;
    };

    // Ordinary uniops
    class UniopLog : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopLog2 : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopLog10 : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopExp : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopMinus : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopAbs : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopSqrt : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopLogistic : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopLogit : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopProbit : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopNormcdf : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopFactorial : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopFactln : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopGamma : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopGammaln : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopSin : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopSinh : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopCos : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopCosh : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopTan : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopTanh : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopCot : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopCoth : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopSec : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopSech : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopCsc : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopCsch : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopArcsin : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopArcsinh : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopArccos : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopArccosh : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopArctan : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopArctanh : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopArccot : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopArccoth : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopArcsec : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopArcsech : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopArccsc : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopArccsch : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopHeaviside : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopSign : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopFloor : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class UniopCeiling : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    // Logic uniops
    class LogicUniop : public Uniop {

    };

    class LogicUniopIsdefined : public LogicUniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };

    class LogicUniopNot : public LogicUniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);

        protected:
            std::unique_ptr<AstNode> clone() override;
    };
}

#endif
