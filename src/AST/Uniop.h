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

#ifndef PHARMML_UNIOP_H_
#define PHARMML_UNIOP_H_

#include <string>
#include <AST/AstNode.h>
#include <visitors/AstNodeVisitor.h>

namespace PharmML
{
    class Uniop : public AstNode
    {
        public:
            AstNode *getChild();
            void setChild(AstNode *node);
        
        private:
            AstNode *child = nullptr;
    };

    // Ordinary uniops
    class UniopLog : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopLog2 : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopLog10 : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopExp : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopMinus : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopAbs : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopSqrt : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopLogistic : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopLogit : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopProbit : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopNormcdf : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopFactorial : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopFactln : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopGamma : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopGammaln : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopSin : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopSinh : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopCos : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopCosh : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopTan : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopTanh : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopCot : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopCoth : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopSec : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopSech : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopCsc : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopCsch : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopArcsin : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopArcsinh : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopArccos : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopArccosh : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopArctan : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopArctanh : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopArccot : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopArccoth : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopArcsec : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopArcsech : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopArccsc : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopArccsch : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopHeaviside : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopSign : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopFloor : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class UniopCeiling : public Uniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    // Logic uniops
    class LogicUniop : public Uniop {

    };

    class LogicUniopIsdefined : public LogicUniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };

    class LogicUniopNot : public LogicUniop
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };
}

#endif
