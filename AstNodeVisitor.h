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

#ifndef PHARMML_ASTNODEVISITOR_H_
#define PHARMML_ASTNODEVISITOR_H_

namespace PharmML
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
    class TargetMapping;
    
    class LogicFalse;
    class LogicTrue;
    class Pi;
    class Exponentiale;
    class NullValue;
    
    class Vector;
    
    class Piecewise;
    class Piece;
    
    class FunctionCall;
    class FunctionArgument;
    class Interval;
    
    class AstNodeVisitor
    {
        public:
            virtual void visit(UniopLog *node) = 0;
            virtual void visit(UniopLog2 *node) = 0;
            virtual void visit(UniopLog10 *node) = 0;
            virtual void visit(UniopExp *node) = 0;
            virtual void visit(UniopMinus *node) = 0;
            virtual void visit(UniopAbs *node) = 0;
            virtual void visit(UniopSqrt *node) = 0;
            virtual void visit(UniopLogistic *node) = 0;
            virtual void visit(UniopLogit *node) = 0;
            virtual void visit(UniopProbit *node) = 0;
            virtual void visit(UniopNormcdf *node) = 0;
            virtual void visit(UniopFactorial *node) = 0;
            virtual void visit(UniopFactln *node) = 0;
            virtual void visit(UniopGamma *node) = 0;
            virtual void visit(UniopGammaln *node) = 0;
            virtual void visit(UniopSin *node) = 0;
            virtual void visit(UniopSinh *node) = 0;
            virtual void visit(UniopCos *node) = 0;
            virtual void visit(UniopCosh *node) = 0;
            virtual void visit(UniopTan *node) = 0;
            virtual void visit(UniopTanh *node) = 0;
            virtual void visit(UniopCot *node) = 0;
            virtual void visit(UniopCoth *node) = 0;
            virtual void visit(UniopSec *node) = 0;
            virtual void visit(UniopSech *node) = 0;
            virtual void visit(UniopCsc *node) = 0;
            virtual void visit(UniopCsch *node) = 0;
            virtual void visit(UniopArcsin *node) = 0;
            virtual void visit(UniopArcsinh *node) = 0;
            virtual void visit(UniopArccos *node) = 0;
            virtual void visit(UniopArccosh *node) = 0;
            virtual void visit(UniopArctan *node) = 0;
            virtual void visit(UniopArctanh *node) = 0;
            virtual void visit(UniopArccot *node) = 0;
            virtual void visit(UniopArccoth *node) = 0;
            virtual void visit(UniopArcsec *node) = 0;
            virtual void visit(UniopArcsech *node) = 0;
            virtual void visit(UniopArccsc *node) = 0;
            virtual void visit(UniopArccsch *node) = 0;
            virtual void visit(UniopHeaviside *node) = 0;
            virtual void visit(UniopSign *node) = 0;
            virtual void visit(UniopFloor *node) = 0;
            virtual void visit(UniopCeiling *node) = 0;
            
            virtual void visit(LogicUniopIsdefined *node) = 0;
            virtual void visit(LogicUniopNot *node) = 0;
            
            virtual void visit(ScalarInt *node) = 0;
            virtual void visit(ScalarReal *node) = 0;
            
            virtual void visit(BinopPlus *node) = 0;
            virtual void visit(BinopMinus *node) = 0;
            virtual void visit(BinopDivide *node) = 0;
            virtual void visit(BinopTimes *node) = 0;  
            virtual void visit(BinopPower *node) = 0;
            virtual void visit(BinopLogx *node) = 0;
            virtual void visit(BinopRoot *node) = 0;
            virtual void visit(BinopMin *node) = 0;
            virtual void visit(BinopMax *node) = 0;
            virtual void visit(BinopRem *node) = 0;
            virtual void visit(BinopAtan2 *node) = 0;
            
            virtual void visit(LogicBinopLt *node) = 0;
            virtual void visit(LogicBinopLeq *node) = 0;
            virtual void visit(LogicBinopGt *node) = 0;
            virtual void visit(LogicBinopGeq *node) = 0;
            virtual void visit(LogicBinopEq *node) = 0;
            virtual void visit(LogicBinopNeq *node) = 0;
            virtual void visit(LogicBinopAnd *node) = 0;
            virtual void visit(LogicBinopOr *node) = 0;
            virtual void visit(LogicBinopXor *node) = 0;
            
            virtual void visit(SymbRef *node) = 0;
            virtual void visit(SteadyStateParameter *node) = 0;
            virtual void visit(ColumnRef *node) = 0;
            virtual void visit(TargetMapping *node) = 0;
            
            virtual void visit(LogicFalse *node) = 0;
            virtual void visit(LogicTrue *node) = 0;
            virtual void visit(Pi *node) = 0;
            virtual void visit(Exponentiale *node) = 0;
            virtual void visit(NullValue *node) = 0;
            
            virtual void visit(Vector *node) = 0;
            
            virtual void visit(Piecewise *node) = 0;
            virtual void visit(Piece *node) = 0;
            
            virtual void visit(FunctionCall *node) = 0;
            virtual void visit(FunctionArgument *node) = 0;
            virtual void visit(Interval *node) = 0;
    };
}

#endif
