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

#ifndef PHARMMLCPP_ASTNODEVISITOR_H_
#define PHARMMLCPP_ASTNODEVISITOR_H_

namespace pharmmlcpp
{
    class UniopLog;
    class UniopLog2;
    class UniopLog10;
    class UniopExp;
    class UniopMinus;
    class UniopAbs;
    class UniopSqrt;
    class UniopLogistic;
    class UniopLogit;
    class UniopProbit;
    class UniopNormcdf;
    class UniopFactorial;
    class UniopFactln;
    class UniopGamma;
    class UniopGammaln;
    class UniopSin;
    class UniopSinh;
    class UniopCos;
    class UniopCosh;
    class UniopTan;
    class UniopTanh;
    class UniopCot;
    class UniopCoth;
    class UniopSec;
    class UniopSech;
    class UniopCsc;
    class UniopCsch;
    class UniopArcsin;
    class UniopArcsinh;
    class UniopArccos;
    class UniopArccosh;
    class UniopArctan;
    class UniopArctanh;
    class UniopArccot;
    class UniopArccoth;
    class UniopArcsec;
    class UniopArcsech;
    class UniopArccsc;
    class UniopArccsch;
    class UniopHeaviside;
    class UniopSign;
    class UniopFloor;
    class UniopCeiling;

    class LogicUniopIsdefined;
    class LogicUniopNot;

    class ScalarInt;
    class ScalarReal;
    class ScalarBool;
    class ScalarString;
    class ScalarId;
    class ScalarNA;
    class ScalarNaN;
    class ScalarPlusInf;
    class ScalarMinusInf;
    class ScalarALQ;
    class ScalarBLQ;

    class BinopPlus;
    class BinopMinus;
    class BinopDivide;
    class BinopTimes;
    class BinopPower;
    class BinopLogx;
    class BinopRoot;
    class BinopMin;
    class BinopMax;
    class BinopRem;
    class BinopAtan2;

    class LogicBinopLt;
    class LogicBinopLeq;
    class LogicBinopGt;
    class LogicBinopGeq;
    class LogicBinopEq;
    class LogicBinopNeq;
    class LogicBinopAnd;
    class LogicBinopOr;
    class LogicBinopXor;

    class SymbRef;
    class SteadyStateParameter;
    class ColumnRef;
    class CatRef;

    class Pi;
    class Exponentiale;
    class NullValue;

    class Vector;

    class Piecewise;
    class Piece;

    class FunctionCall;
    class FunctionArgument;
    class Interval;
    class Sequence;
    
    class MatrixSelector;

    class AstNodeVisitor
    {
        public:
            virtual void visit(UniopLog *node) {};
            virtual void visit(UniopLog2 *node) {};
            virtual void visit(UniopLog10 *node) {};
            virtual void visit(UniopExp *node) {};
            virtual void visit(UniopMinus *node) {};
            virtual void visit(UniopAbs *node) {};
            virtual void visit(UniopSqrt *node) {};
            virtual void visit(UniopLogistic *node) {};
            virtual void visit(UniopLogit *node) {};
            virtual void visit(UniopProbit *node) {};
            virtual void visit(UniopNormcdf *node) {};
            virtual void visit(UniopFactorial *node) {};
            virtual void visit(UniopFactln *node) {};
            virtual void visit(UniopGamma *node) {};
            virtual void visit(UniopGammaln *node) {};
            virtual void visit(UniopSin *node) {};
            virtual void visit(UniopSinh *node) {};
            virtual void visit(UniopCos *node) {};
            virtual void visit(UniopCosh *node) {};
            virtual void visit(UniopTan *node) {};
            virtual void visit(UniopTanh *node) {};
            virtual void visit(UniopCot *node) {};
            virtual void visit(UniopCoth *node) {};
            virtual void visit(UniopSec *node) {};
            virtual void visit(UniopSech *node) {};
            virtual void visit(UniopCsc *node) {};
            virtual void visit(UniopCsch *node) {};
            virtual void visit(UniopArcsin *node) {};
            virtual void visit(UniopArcsinh *node) {};
            virtual void visit(UniopArccos *node) {};
            virtual void visit(UniopArccosh *node) {};
            virtual void visit(UniopArctan *node) {};
            virtual void visit(UniopArctanh *node) {};
            virtual void visit(UniopArccot *node) {};
            virtual void visit(UniopArccoth *node) {};
            virtual void visit(UniopArcsec *node) {};
            virtual void visit(UniopArcsech *node) {};
            virtual void visit(UniopArccsc *node) {};
            virtual void visit(UniopArccsch *node) {};
            virtual void visit(UniopHeaviside *node) {};
            virtual void visit(UniopSign *node) {};
            virtual void visit(UniopFloor *node) {};
            virtual void visit(UniopCeiling *node) {};

            virtual void visit(LogicUniopIsdefined *node) {};
            virtual void visit(LogicUniopNot *node) {};

            virtual void visit(ScalarInt *node) {};
            virtual void visit(ScalarReal *node) {};
            virtual void visit(ScalarBool *node) {};
            virtual void visit(ScalarString *node) {};
            virtual void visit(ScalarId *node) {};
            virtual void visit(ScalarNA *node) {};
            virtual void visit(ScalarNaN *node) {};
            virtual void visit(ScalarPlusInf *node) {};
            virtual void visit(ScalarMinusInf *node) {};
            virtual void visit(ScalarALQ *node) {};
            virtual void visit(ScalarBLQ *node) {};

            virtual void visit(BinopPlus *node) {};
            virtual void visit(BinopMinus *node) {};
            virtual void visit(BinopDivide *node) {};
            virtual void visit(BinopTimes *node) {};
            virtual void visit(BinopPower *node) {};
            virtual void visit(BinopLogx *node) {};
            virtual void visit(BinopRoot *node) {};
            virtual void visit(BinopMin *node) {};
            virtual void visit(BinopMax *node) {};
            virtual void visit(BinopRem *node) {};
            virtual void visit(BinopAtan2 *node) {};

            virtual void visit(LogicBinopLt *node) {};
            virtual void visit(LogicBinopLeq *node) {};
            virtual void visit(LogicBinopGt *node) {};
            virtual void visit(LogicBinopGeq *node) {};
            virtual void visit(LogicBinopEq *node) {};
            virtual void visit(LogicBinopNeq *node) {};
            virtual void visit(LogicBinopAnd *node) {};
            virtual void visit(LogicBinopOr *node) {};
            virtual void visit(LogicBinopXor *node) {};

            virtual void visit(SymbRef *node) {};
            virtual void visit(SteadyStateParameter *node) {};
            virtual void visit(ColumnRef *node) {};
            virtual void visit(CatRef *node) {};

            virtual void visit(Pi *node) {};
            virtual void visit(Exponentiale *node) {};
            virtual void visit(NullValue *node) {};

            virtual void visit(Vector *node) {};

            virtual void visit(Piecewise *node) {};
            virtual void visit(Piece *node) {};

            virtual void visit(FunctionCall *node) {};
            virtual void visit(FunctionArgument *node) {};
            virtual void visit(Interval *node) {};
            virtual void visit(Sequence *node) {};

            virtual void visit(MatrixSelector *node) {};
    };
}

#endif
