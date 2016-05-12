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

#include "SymbRefFinder.h"

namespace PharmML
{
    // public
    std::unordered_set<SymbRef *> SymbRefFinder::getSymbRefs() {
        return this->symbRefs;
    }

    void SymbRefFinder::visit(SymbRef *node) {
        this->symbRefs.insert(node);
    }
    
    void SymbRefFinder::visit(SteadyStateParameter *node) {
    }
    
    void SymbRefFinder::visit(ColumnRef *node) {
    }

    void SymbRefFinder::visit(TargetMapping *node) {
    }

    void SymbRefFinder::visit(UniopLog *node) {
    }
    
    void SymbRefFinder::visit(UniopLog2 *node) {
    }
    
    void SymbRefFinder::visit(UniopLog10 *node) {
    }

    void SymbRefFinder::visit(UniopExp *node) {
    }

    void SymbRefFinder::visit(UniopMinus *node) {
    }

    void SymbRefFinder::visit(UniopAbs *node) {
    }
    
    void SymbRefFinder::visit(UniopSqrt *node) {
    }
    
    void SymbRefFinder::visit(UniopLogistic *node) {
    }
    
    void SymbRefFinder::visit(UniopLogit *node) {
    }
    
    void SymbRefFinder::visit(UniopProbit *node) {
    }
    
    void SymbRefFinder::visit(UniopNormcdf *node) {
    }
    
    void SymbRefFinder::visit(UniopFactorial *node) {
    }
    
    void SymbRefFinder::visit(UniopFactln *node) {
    }
    
    void SymbRefFinder::visit(UniopGamma *node) {
    }
    
    void SymbRefFinder::visit(UniopGammaln *node) {
    }
    
    void SymbRefFinder::visit(UniopSin *node) {
    }
    
    void SymbRefFinder::visit(UniopSinh *node) {
    }
    
    void SymbRefFinder::visit(UniopCos *node) {
    }

    void SymbRefFinder::visit(UniopCosh *node) {
    }
    
    void SymbRefFinder::visit(UniopTan *node) {
    }
    
    void SymbRefFinder::visit(UniopTanh *node) {
    }
    
    void SymbRefFinder::visit(UniopCot *node) {
    }
    
    void SymbRefFinder::visit(UniopCoth *node) {
    }
    
    void SymbRefFinder::visit(UniopSec *node) {
    }
    
    void SymbRefFinder::visit(UniopSech *node) {
    }
    
    void SymbRefFinder::visit(UniopCsc *node) {
    }
    
    void SymbRefFinder::visit(UniopCsch *node) {
    }
    
    void SymbRefFinder::visit(UniopArcsin *node) {
    }
    
    void SymbRefFinder::visit(UniopArcsinh *node) {
    }
    
    void SymbRefFinder::visit(UniopArccos *node) {
    }
    
    void SymbRefFinder::visit(UniopArccosh *node) {
    }
    
    void SymbRefFinder::visit(UniopArctan *node) {
    }
    
    void SymbRefFinder::visit(UniopArctanh *node) {
    }
    
    void SymbRefFinder::visit(UniopArccot *node) {
    }
    
    void SymbRefFinder::visit(UniopArccoth *node) {
    }
    
    void SymbRefFinder::visit(UniopArcsec *node) {
    }
    
    void SymbRefFinder::visit(UniopArcsech *node) {
    }
    
    void SymbRefFinder::visit(UniopArccsc *node) {
    }
    
    void SymbRefFinder::visit(UniopArccsch *node) {
    }
    
    void SymbRefFinder::visit(UniopHeaviside *node) {
    }
    
    void SymbRefFinder::visit(UniopSign *node) {
    }
    
    void SymbRefFinder::visit(UniopFloor *node) {
    }
    
    void SymbRefFinder::visit(UniopCeiling *node) {
    }

    void SymbRefFinder::visit(ScalarInt *node) {
    }

    void SymbRefFinder::visit(ScalarReal *node) {
    }

    void SymbRefFinder::visit(BinopPlus *node) {
    }

    void SymbRefFinder::visit(BinopMinus *node) {
    }

    void SymbRefFinder::visit(BinopDivide *node) {
    }

    void SymbRefFinder::visit(BinopTimes *node) {
    }
    
    void SymbRefFinder::visit(BinopPower *node) {
    }
    
    void SymbRefFinder::visit(BinopLogx *node) {
    }
    
    void SymbRefFinder::visit(BinopRoot *node) {
    }
    
    void SymbRefFinder::visit(BinopMin *node) {
    }
    
    void SymbRefFinder::visit(BinopMax *node) {
    }
    
    void SymbRefFinder::visit(BinopRem *node) {
    }
    
    void SymbRefFinder::visit(BinopAtan2 *node) {
    }
    
    void SymbRefFinder::visit(LogicUniopIsdefined *node) {
    }
    
    void SymbRefFinder::visit(LogicUniopNot *node) {
    }

    void SymbRefFinder::visit(LogicBinopLt *node) {
    }

    void SymbRefFinder::visit(LogicBinopLeq *node) {
    }
    
    void SymbRefFinder::visit(LogicBinopGt *node) {
    }
    
    void SymbRefFinder::visit(LogicBinopGeq *node) {
    }
    
    void SymbRefFinder::visit(LogicBinopEq *node) {
    }
    
    void SymbRefFinder::visit(LogicBinopNeq *node) {
    }
    
    void SymbRefFinder::visit(LogicBinopAnd *node) {
    }
    
    void SymbRefFinder::visit(LogicBinopOr *node) {
    }
    
    void SymbRefFinder::visit(LogicBinopXor *node) {
    }
    
    void SymbRefFinder::visit(Vector *node) {
    }
    
    void SymbRefFinder::visit(Piecewise *node) {
    }

    void SymbRefFinder::visit(Piece *node) {
    }
    
    void SymbRefFinder::visit(LogicFalse *node) {
    }
    
    void SymbRefFinder::visit(LogicTrue *node) {
    }
    
    void SymbRefFinder::visit(Pi *node) {
    }
    
    void SymbRefFinder::visit(Exponentiale *node) {
    }
    
    void SymbRefFinder::visit(NullValue *node) {
    }

    void SymbRefFinder::visit(FunctionCall *node) {
    }

    void SymbRefFinder::visit(FunctionArgument *node) {
    }

    void SymbRefFinder::visit(Interval *node) {
    }
}
