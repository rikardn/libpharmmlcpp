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

#include "AstAnalyzer.h"

namespace PharmML
{
    // private
    void AstAnalyzer::clearPureState() {
        this->is_pure_symbref = false;
        this->is_pure_scalar = false;
        this->pure_symbref = nullptr;
        this->pure_scalar = nullptr;
    }
    
    // public
    void AstAnalyzer::reset() {
        this->clearPureState();
    }
    
    bool AstAnalyzer::isPureSymbRef() {
        return this->is_pure_symbref;
    }
    
    bool AstAnalyzer::isPureScalar() {
        return this->is_pure_scalar;
    }
    
    bool AstAnalyzer::isPureFunctionCall() {
        return this->is_pure_functioncall;
    }
    
    SymbRef *AstAnalyzer::getPureSymbRef() {
        return this->pure_symbref;
    }
    
    Scalar *AstAnalyzer::getPureScalar() {
        return this->pure_scalar;
    }
    
    FunctionCall *AstAnalyzer::getPureFunctionCall() {
        return this->pure_functioncall;
    }
    
    // visitor methods
    void AstAnalyzer::visit(SymbRef *node) {
        this->clearPureState();
        this->is_pure_symbref = true;
        this->pure_symbref = node;
    }

    void AstAnalyzer::visit(SteadyStateParameter *node) {
        this->clearPureState();
    }
    
    void AstAnalyzer::visit(ColumnRef *node) {
        this->clearPureState();
    }

    void AstAnalyzer::visit(TargetMapping *node) {
        this->clearPureState();
    }

    void AstAnalyzer::visit(UniopLog *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopLog2 *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopLog10 *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }

    void AstAnalyzer::visit(UniopExp *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }

    void AstAnalyzer::visit(UniopMinus *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }

    void AstAnalyzer::visit(UniopAbs *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopSqrt *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopLogistic *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopLogit *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopProbit *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopNormcdf *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopFactorial *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopFactln *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopGamma *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopGammaln *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopSin *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopSinh *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopCos *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }

    void AstAnalyzer::visit(UniopCosh *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopTan *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopTanh *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopCot *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopCoth *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopSec *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopSech *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopCsc *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopCsch *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopArcsin *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopArcsinh *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopArccos *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopArccosh *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopArctan *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopArctanh *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopArccot *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopArccoth *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopArcsec *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopArcsech *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopArccsc *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopArccsch *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopHeaviside *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopSign *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopFloor *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(UniopCeiling *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }

    void AstAnalyzer::visit(ScalarInt *node) {
        this->clearPureState();
        this->is_pure_scalar = true;
        this->pure_scalar = node;
    }

    void AstAnalyzer::visit(ScalarReal *node) {
        this->clearPureState();
        this->is_pure_scalar = true;
        this->pure_scalar = node;
    }

    void AstAnalyzer::visit(BinopPlus *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void AstAnalyzer::visit(BinopMinus *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void AstAnalyzer::visit(BinopDivide *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void AstAnalyzer::visit(BinopTimes *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }
    
    void AstAnalyzer::visit(BinopPower *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }
    
    void AstAnalyzer::visit(BinopLogx *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }
    
    void AstAnalyzer::visit(BinopRoot *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }
    
    void AstAnalyzer::visit(BinopMin *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }
    
    void AstAnalyzer::visit(BinopMax *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }
    
    void AstAnalyzer::visit(BinopRem *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }
    
    void AstAnalyzer::visit(BinopAtan2 *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }
    
    void AstAnalyzer::visit(LogicUniopIsdefined *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }
    
    void AstAnalyzer::visit(LogicUniopNot *node) {
        this->clearPureState();
        node->getChild()->accept(this);
    }

    void AstAnalyzer::visit(LogicBinopLt *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void AstAnalyzer::visit(LogicBinopLeq *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }
    
    void AstAnalyzer::visit(LogicBinopGt *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }
    
    void AstAnalyzer::visit(LogicBinopGeq *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }
    
    void AstAnalyzer::visit(LogicBinopEq *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }
    
    void AstAnalyzer::visit(LogicBinopNeq *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }
    
    void AstAnalyzer::visit(LogicBinopAnd *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }
    
    void AstAnalyzer::visit(LogicBinopOr *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }
    
    void AstAnalyzer::visit(LogicBinopXor *node) {
        this->clearPureState();
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }
    
    void AstAnalyzer::visit(Vector *node) {
        this->clearPureState();
        
    }
    
    void AstAnalyzer::visit(Piecewise *node) {
        this->clearPureState();
        for (Piece *piece : node->getPieces()) {
            piece->accept(this);
        }
    }

    void AstAnalyzer::visit(Piece *node) {
        this->clearPureState();
        if (node->getExpression()) {
            node->getExpression()->accept(this);
        }
        if (node->getCondition()) {
            node->getCondition()->accept(this);
        }
    }
    
    void AstAnalyzer::visit(LogicFalse *node) {
        this->clearPureState();
        
    }
    
    void AstAnalyzer::visit(LogicTrue *node) {
        this->clearPureState();
        
    }
    
    void AstAnalyzer::visit(Pi *node) {
        this->clearPureState();
        
    }
    
    void AstAnalyzer::visit(Exponentiale *node) {
        this->clearPureState();
        
    }
    
    void AstAnalyzer::visit(NullValue *node) {
        this->clearPureState();
        
    }

    void AstAnalyzer::visit(FunctionCall *node) {
        this->clearPureState();
        this->is_pure_functioncall = true;
        this->pure_functioncall = node;
    }

    void AstAnalyzer::visit(FunctionArgument *node) {
        this->clearPureState();
        
    }

    void AstAnalyzer::visit(Interval *node) {
        this->clearPureState();
        
    }
}
