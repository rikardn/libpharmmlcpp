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
#include <PharmML/PharmMLWriter.h>

namespace pharmmlcpp
{
    /**
     * Base class for all uniop AstNodes 
     */
    class Uniop : public AstNode
    {
        public:
            Uniop() {};
            Uniop(const Uniop &from) { if (from.child) this->child = from.child->clone(); };
            AstNode *getChild();
            void setChild(std::unique_ptr<AstNode> node);

        protected:
            std::unique_ptr<AstNode> child;
    };

    /**
     *  The natural logarithm (ln)
     */
    class UniopLog : public Uniop
    {
        public:
            UniopLog() {};
            UniopLog(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopLog(const UniopLog &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  The binary logarithm (lb)
     */
    class UniopLog2 : public Uniop
    {
        public:
            UniopLog2() {};
            UniopLog2(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopLog2(const UniopLog2 &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  The common logarithm (lg)
     */
    class UniopLog10 : public Uniop
    {
        public:
            UniopLog10() {};
            UniopLog10(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopLog10(const UniopLog10 &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  The exponential function
     */
    class UniopExp : public Uniop
    {
        public:
            UniopExp() {};
            UniopExp(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopExp(const UniopExp &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Unary minus
     */
    class UniopMinus : public Uniop
    {
        public:
            UniopMinus() {};
            UniopMinus(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopMinus(const UniopMinus &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Absolute value
     */
    class UniopAbs : public Uniop
    {
        public:
            UniopAbs() {};
            UniopAbs(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopAbs(const UniopAbs &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  The square root
     */
    class UniopSqrt : public Uniop
    {
        public:
            UniopSqrt() {};
            UniopSqrt(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopSqrt(const UniopSqrt &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  The standard logistic function 1/(1 + exp(-x))
     */
    class UniopLogistic : public Uniop
    {
        public:
            UniopLogistic() {};
            UniopLogistic(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopLogistic(const UniopLogistic &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  The logit function ln(x / (1 - x))
     */
    class UniopLogit : public Uniop
    {
        public:
            UniopLogit() {};
            UniopLogit(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopLogit(const UniopLogit &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  The pobit function sqrt(2) * erf^(-1)*(2*x - 1)
     */
    class UniopProbit : public Uniop
    {
        public:
            UniopProbit() {};
            UniopProbit(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopProbit(const UniopProbit &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  The normal cumulative distribution function
     */
    class UniopNormcdf : public Uniop
    {
        public:
            UniopNormcdf() {};
            UniopNormcdf(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopNormcdf(const UniopNormcdf &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  The factorial function
     */
    class UniopFactorial : public Uniop
    {
        public:
            UniopFactorial() {};
            UniopFactorial(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopFactorial(const UniopFactorial &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  The logarithmic of the factorial function
     */
    class UniopFactln : public Uniop
    {
        public:
            UniopFactln() {};
            UniopFactln(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopFactln(const UniopFactln &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  The gamma function
     */
    class UniopGamma : public Uniop
    {
        public:
            UniopGamma() {};
            UniopGamma(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopGamma(const UniopGamma &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  The logarithm of the gamma function
     */
    class UniopGammaln : public Uniop
    {
        public:
            UniopGammaln() {};
            UniopGammaln(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopGammaln(const UniopGammaln &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  sin(x)
     */
    class UniopSin : public Uniop
    {
        public:
            UniopSin() {};
            UniopSin(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopSin(const UniopSin &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  sinh(x)
     */
    class UniopSinh : public Uniop
    {
        public:
            UniopSinh() {};
            UniopSinh(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopSinh(const UniopSinh &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  cos(x)
     */
    class UniopCos : public Uniop
    {
        public:
            UniopCos() {};
            UniopCos(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopCos(const UniopCos &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  cosh(x)
     */
    class UniopCosh : public Uniop
    {
        public:
            UniopCosh() {};
            UniopCosh(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopCosh(const UniopCosh &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  tan(x)
     */
    class UniopTan : public Uniop
    {
        public:
            UniopTan() {};
            UniopTan(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopTan(const UniopTan &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  tanh(x)
     */
    class UniopTanh : public Uniop
    {
        public:
            UniopTanh() {};
            UniopTanh(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopTanh(const UniopTanh &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  cot(x)
     */
    class UniopCot : public Uniop
    {
        public:
            UniopCot() {};
            UniopCot(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopCot(const UniopCot &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  coth(x)
     */
    class UniopCoth : public Uniop
    {
        public:
            UniopCoth() {};
            UniopCoth(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopCoth(const UniopCoth &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  sec(x)
     */
    class UniopSec : public Uniop
    {
        public:
            UniopSec() {};
            UniopSec(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopSec(const UniopSec &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  sech(x)
     */
    class UniopSech : public Uniop
    {
        public:
            UniopSech() {};
            UniopSech(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopSech(const UniopSech &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  csc(x)
     */
    class UniopCsc : public Uniop
    {
        public:
            UniopCsc() {};
            UniopCsc(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopCsc(const UniopCsc &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  csch(x)
     */
    class UniopCsch : public Uniop
    {
        public:
            UniopCsch() {};
            UniopCsch(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopCsch(const UniopCsch &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  arcsin(x)
     */
    class UniopArcsin : public Uniop
    {
        public:
            UniopArcsin() {};
            UniopArcsin(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopArcsin(const UniopArcsin &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  arsinh(x)
     */
    class UniopArcsinh : public Uniop
    {
        public:
            UniopArcsinh() {};
            UniopArcsinh(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopArcsinh(const UniopArcsinh &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  arccos(x)
     */
    class UniopArccos : public Uniop
    {
        public:
            UniopArccos() {};
            UniopArccos(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopArccos(const UniopArccos &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  arcosh(x)
     */
    class UniopArccosh : public Uniop
    {
        public:
            UniopArccosh() {};
            UniopArccosh(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopArccosh(const UniopArccosh &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  arctan(x)
     */
    class UniopArctan : public Uniop
    {
        public:
            UniopArctan() {};
            UniopArctan(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopArctan(const UniopArctan &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  artanh(x)
     */
    class UniopArctanh : public Uniop
    {
        public:
            UniopArctanh() {};
            UniopArctanh(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopArctanh(const UniopArctanh &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  arccot(x)
     */
    class UniopArccot : public Uniop
    {
        public:
            UniopArccot() {};
            UniopArccot(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopArccot(const UniopArccot &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  arcoth(x)
     */
    class UniopArccoth : public Uniop
    {
        public:
            UniopArccoth() {};
            UniopArccoth(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopArccoth(const UniopArccoth &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  arcsec(x)
     */
    class UniopArcsec : public Uniop
    {
        public:
            UniopArcsec() {};
            UniopArcsec(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopArcsec(const UniopArcsec &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  arsech(x)
     */
    class UniopArcsech : public Uniop
    {
        public:
            UniopArcsech() {};
            UniopArcsech(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopArcsech(const UniopArcsech &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  arccsc(x)
     */
    class UniopArccsc : public Uniop
    {
        public:
            UniopArccsc() {};
            UniopArccsc(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopArccsc(const UniopArccsc &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  arcsch(x)
     */
    class UniopArccsch : public Uniop
    {
        public:
            UniopArccsch() {};
            UniopArccsch(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopArccsch(const UniopArccsch &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  The Heaviside step function
     */
    class UniopHeaviside : public Uniop
    {
        public:
            UniopHeaviside() {};
            UniopHeaviside(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopHeaviside(const UniopHeaviside &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  sgn(x)
     */
    class UniopSign : public Uniop
    {
        public:
            UniopSign() {};
            UniopSign(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopSign(const UniopSign &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  floor
     */
    class UniopFloor : public Uniop
    {
        public:
            UniopFloor() {};
            UniopFloor(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopFloor(const UniopFloor &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  ceiling
     */
    class UniopCeiling : public Uniop
    {
        public:
            UniopCeiling() {};
            UniopCeiling(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            UniopCeiling(const UniopCeiling &from) : Uniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Base class for all logic uniop AstNodes
     */
    class LogicUniop : public Uniop {
        public:
            LogicUniop() {};
            LogicUniop(const LogicUniop &from) : Uniop(from) {};
    };

    /**
     *  Is defined operator
     */
    class LogicUniopIsdefined : public LogicUniop
    {
        public:
            LogicUniopIsdefined() {};
            LogicUniopIsdefined(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            LogicUniopIsdefined(const LogicUniopIsdefined &from) : LogicUniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };

    /**
     *  Logical not
     */
    class LogicUniopNot : public LogicUniop
    {
        public:
            LogicUniopNot() {};
            LogicUniopNot(std::unique_ptr<AstNode> child) { this->child = std::move(child); };
            LogicUniopNot(const LogicUniopNot &from) : LogicUniop(from) {};
            void accept(AstNodeVisitor *visitor) override;
            std::unique_ptr<AstNode> clone() override;
            xml::Node xml(PharmMLWriter &writer) override;
    };
}

#endif
