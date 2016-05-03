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

#ifndef PHARMML_XMLASTVISITOR_H_
#define PHARMML_XMLASTVISITOR_H_

#include <string>
#include <visitors/AstNodeVisitor.h>
#include <AST/symbols.h>
#include <AST/Uniop.h>
#include <AST/Binop.h>
#include <AST/Scalar.h>
#include <AST/Constant.h>
#include <AST/Vector.h>
#include <AST/Piecewise.h>
#include <symbols/DerivativeVariable.h>
#include <AST/FunctionCall.h>
#include <AST/Interval.h>
#include <PharmML/Distribution.h>
#include <PharmML/ColumnMapping.h>
#include <PharmML/Interventions.h>

namespace PharmML
{
    class XMLAstVisitor : public AstNodeVisitor
    {
        private:
            xml::Node value;
            void setValue(xml::Node node);
            xml::Node acceptLeft(Binop *binop);
            xml::Node acceptRight(Binop *binop);
            xml::Node acceptChild(Uniop *uniop);
            xml::Node accept(AstNode *node);
            void createUniop(Uniop *node, std::string op);
            void createLogicUniop(LogicUniop *node, std::string op);
            void createBinop(Binop *node, std::string op);
            void createLogicBinop(Binop *node, std::string op);

        public:
            xml::Node getValue();

            virtual void visit(SymbRef *node);
            virtual void visit(SteadyStateParameter *node);
            virtual void visit(ColumnRef *node);
            virtual void visit(TargetMapping *node);
            
            virtual void visit(UniopLog *node);
            virtual void visit(UniopLog2 *node);
            virtual void visit(UniopLog10 *node);
            virtual void visit(UniopExp *node);
            virtual void visit(UniopMinus *node);
            virtual void visit(UniopAbs *node);
            virtual void visit(UniopSqrt *node);
            virtual void visit(UniopLogistic *node);
            virtual void visit(UniopLogit *node);
            virtual void visit(UniopProbit *node);
            virtual void visit(UniopNormcdf *node);
            virtual void visit(UniopFactorial *node);
            virtual void visit(UniopFactln *node);
            virtual void visit(UniopGamma *node);
            virtual void visit(UniopGammaln *node);
            virtual void visit(UniopSin *node);
            virtual void visit(UniopSinh *node);
            virtual void visit(UniopCos *node);
            virtual void visit(UniopCosh *node);
            virtual void visit(UniopTan *node);
            virtual void visit(UniopTanh *node);
            virtual void visit(UniopCot *node);
            virtual void visit(UniopCoth *node);
            virtual void visit(UniopSec *node);
            virtual void visit(UniopSech *node);
            virtual void visit(UniopCsc *node);
            virtual void visit(UniopCsch *node);
            virtual void visit(UniopArcsin *node);
            virtual void visit(UniopArcsinh *node);
            virtual void visit(UniopArccos *node);
            virtual void visit(UniopArccosh *node);
            virtual void visit(UniopArctan *node);
            virtual void visit(UniopArctanh *node);
            virtual void visit(UniopArccot *node);
            virtual void visit(UniopArccoth *node);
            virtual void visit(UniopArcsec *node);
            virtual void visit(UniopArcsech *node);
            virtual void visit(UniopArccsc *node);
            virtual void visit(UniopArccsch *node);
            virtual void visit(UniopHeaviside *node);
            virtual void visit(UniopSign *node);
            virtual void visit(UniopFloor *node);
            virtual void visit(UniopCeiling *node);
            
            virtual void visit(ScalarInt *node);
            virtual void visit(ScalarReal *node);
            
            virtual void visit(BinopPlus *node);
            virtual void visit(BinopMinus *node);
            virtual void visit(BinopDivide *node);
            virtual void visit(BinopTimes *node);
            virtual void visit(BinopPower *node);
            virtual void visit(BinopLogx *node);
            virtual void visit(BinopRoot *node);
            virtual void visit(BinopMin *node);
            virtual void visit(BinopMax *node);
            virtual void visit(BinopRem *node);
            virtual void visit(BinopAtan2 *node);
            
            virtual void visit(LogicFalse *node);
            virtual void visit(LogicTrue *node);
            virtual void visit(Pi *node);
            virtual void visit(Exponentiale *node);
            virtual void visit(NullValue *node);
            
            virtual void visit(LogicUniopIsdefined *node);
            virtual void visit(LogicUniopNot *node);
            
            virtual void visit(LogicBinopLt *node);
            virtual void visit(LogicBinopLeq *node);
            virtual void visit(LogicBinopGt *node);
            virtual void visit(LogicBinopGeq *node);
            virtual void visit(LogicBinopEq *node);
            virtual void visit(LogicBinopNeq *node);
            virtual void visit(LogicBinopAnd *node);
            virtual void visit(LogicBinopOr *node);
            virtual void visit(LogicBinopXor *node);
            
            virtual void visit(Vector *node);
            
            virtual void visit(Piecewise *node);
            virtual void visit(Piece *node);
            
            virtual void visit(FunctionCall *node);
            virtual void visit(FunctionArgument *node);
            virtual void visit(Interval *node);
    };
}

#endif
