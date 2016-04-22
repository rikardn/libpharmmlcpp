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

#include "XMLAstVisitor.h"

namespace PharmML
{
    // private
    void XMLAstVisitor::setValue(xml::Node node) {
        this->value = node;
    }

    xml::Node XMLAstVisitor::acceptLeft(Binop *binop) {
        binop->getLeft()->accept(this);
        return this->getValue();
    }

    xml::Node XMLAstVisitor::acceptRight(Binop *binop) {
        binop->getRight()->accept(this);
        return this->getValue();
    }

    xml::Node XMLAstVisitor::acceptChild(Uniop *uniop) {
        uniop->getChild()->accept(this);
        return this->getValue();
    }
   
    xml::Node XMLAstVisitor::accept(AstNode *node) {
        node->accept(this);
        return this->getValue();
    }

    // public
    xml::Node XMLAstVisitor::getValue() {
        return this->value;
    }
    
    void XMLAstVisitor::visit(SymbRef *node) {
        xml::Node symbref("SymbRef", xml::Namespace::ds);
        symbref.setAttribute("blkIdRef", node->getBlkId());
        symbref.setAttribute("symbIdRef", node->toString());
        this->setValue(symbref);
    }
    
    void XMLAstVisitor::visit(SteadyStateParameter *node) {
    }
    
    void XMLAstVisitor::visit(ColumnRef *node) {
    }

    void XMLAstVisitor::visit(TargetMapping *node) {
    }

    void XMLAstVisitor::visit(UniopLog *node) {
    }
    
    void XMLAstVisitor::visit(UniopLog2 *node) {
    }
    
    void XMLAstVisitor::visit(UniopLog10 *node) {
    }

    void XMLAstVisitor::visit(UniopExp *node) {
    }

    void XMLAstVisitor::visit(UniopMinus *node) {
    }

    void XMLAstVisitor::visit(UniopAbs *node) {
    }
    
    void XMLAstVisitor::visit(UniopSqrt *node) {
    }
    
    void XMLAstVisitor::visit(UniopLogistic *node) {
    }
    
    void XMLAstVisitor::visit(UniopLogit *node) {
    }
    
    void XMLAstVisitor::visit(UniopProbit *node) {
    }
    
    void XMLAstVisitor::visit(UniopNormcdf *node) {
    }
    
    void XMLAstVisitor::visit(UniopFactorial *node) {
    }
    
    void XMLAstVisitor::visit(UniopFactln *node) {
    }
    
    void XMLAstVisitor::visit(UniopGamma *node) {
    }
    
    void XMLAstVisitor::visit(UniopGammaln *node) {
    }
    
    void XMLAstVisitor::visit(UniopSin *node) {
    }
    
    void XMLAstVisitor::visit(UniopSinh *node) {
    }
    
    void XMLAstVisitor::visit(UniopCos *node) {
    }

    void XMLAstVisitor::visit(UniopCosh *node) {
    }
    
    void XMLAstVisitor::visit(UniopTan *node) {
    }
    
    void XMLAstVisitor::visit(UniopTanh *node) {
    }
    
    void XMLAstVisitor::visit(UniopCot *node) {
    }
    
    void XMLAstVisitor::visit(UniopCoth *node) {
    }
    
    void XMLAstVisitor::visit(UniopSec *node) {
    }
    
    void XMLAstVisitor::visit(UniopSech *node) {
    }
    
    void XMLAstVisitor::visit(UniopCsc *node) {
    }
    
    void XMLAstVisitor::visit(UniopCsch *node) {
    }
    
    void XMLAstVisitor::visit(UniopArcsin *node) {
    }
    
    void XMLAstVisitor::visit(UniopArcsinh *node) {
    }
    
    void XMLAstVisitor::visit(UniopArccos *node) {
    }
    
    void XMLAstVisitor::visit(UniopArccosh *node) {
    }
    
    void XMLAstVisitor::visit(UniopArctan *node) {
    }
    
    void XMLAstVisitor::visit(UniopArctanh *node) {
    }
    
    void XMLAstVisitor::visit(UniopArccot *node) {
    }
    
    void XMLAstVisitor::visit(UniopArccoth *node) {
    }
    
    void XMLAstVisitor::visit(UniopArcsec *node) {
    }
    
    void XMLAstVisitor::visit(UniopArcsech *node) {
    }
    
    void XMLAstVisitor::visit(UniopArccsc *node) {
    }
    
    void XMLAstVisitor::visit(UniopArccsch *node) {
    }
    
    void XMLAstVisitor::visit(UniopHeaviside *node) {
    }
    
    void XMLAstVisitor::visit(UniopSign *node) {
    }
    
    void XMLAstVisitor::visit(UniopFloor *node) {
    }
    
    void XMLAstVisitor::visit(UniopCeiling *node) {
    }

    void XMLAstVisitor::visit(ScalarInt *node) {
    }

    void XMLAstVisitor::visit(ScalarReal *node) {
    }

    void XMLAstVisitor::visit(BinopPlus *node) {
    }

    void XMLAstVisitor::visit(BinopMinus *node) {
    }

    void XMLAstVisitor::visit(BinopDivide *node) {
    }

    void XMLAstVisitor::visit(BinopTimes *node) {
    }
    
    void XMLAstVisitor::visit(BinopPower *node) {
    }
    
    void XMLAstVisitor::visit(BinopLogx *node) {
    }
    
    void XMLAstVisitor::visit(BinopRoot *node) {
    }
    
    void XMLAstVisitor::visit(BinopMin *node) {
    }
    
    void XMLAstVisitor::visit(BinopMax *node) {
    }
    
    void XMLAstVisitor::visit(BinopRem *node) {
    }
    
    void XMLAstVisitor::visit(BinopAtan2 *node) {
    }
    
    void XMLAstVisitor::visit(LogicUniopIsdefined *node) {
    }
    
    void XMLAstVisitor::visit(LogicUniopNot *node) {
    }

    void XMLAstVisitor::visit(LogicBinopLt *node) {
    }

    void XMLAstVisitor::visit(LogicBinopLeq *node) {
    }
    
    void XMLAstVisitor::visit(LogicBinopGt *node) {
    }
    
    void XMLAstVisitor::visit(LogicBinopGeq *node) {
    }
    
    void XMLAstVisitor::visit(LogicBinopEq *node) {
    }
    
    void XMLAstVisitor::visit(LogicBinopNeq *node) {
    }
    
    void XMLAstVisitor::visit(LogicBinopAnd *node) {
    }
    
    void XMLAstVisitor::visit(LogicBinopOr *node) {
    }
    
    void XMLAstVisitor::visit(LogicBinopXor *node) {
    }
    
    void XMLAstVisitor::visit(Vector *node) {
    }
    
    void XMLAstVisitor::visit(Piecewise *node) {
    }

    void XMLAstVisitor::visit(Piece *node) {
    }
    
    void XMLAstVisitor::visit(LogicFalse *node) {
    }
    
    void XMLAstVisitor::visit(LogicTrue *node) {
    }
    
    void XMLAstVisitor::visit(Pi *node) {
    }
    
    void XMLAstVisitor::visit(Exponentiale *node) {
    }
    
    void XMLAstVisitor::visit(NullValue *node) {
    }

    void XMLAstVisitor::visit(FunctionCall *node) {
    }

    void XMLAstVisitor::visit(FunctionArgument *node) {
    }

    void XMLAstVisitor::visit(Interval *node) {
    }
}
