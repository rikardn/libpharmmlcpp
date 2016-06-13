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

namespace pharmmlcpp
{
    // public
    SymbRefFinder::SymbRefFinder() {
    }

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

    void SymbRefFinder::visit(UniopLog *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopLog2 *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopLog10 *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopExp *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopMinus *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopAbs *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopSqrt *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopLogistic *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopLogit *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopProbit *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopNormcdf *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopFactorial *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopFactln *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopGamma *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopGammaln *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopSin *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopSinh *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopCos *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopCosh *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopTan *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopTanh *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopCot *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopCoth *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopSec *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopSech *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopCsc *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopCsch *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopArcsin *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopArcsinh *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopArccos *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopArccosh *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopArctan *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopArctanh *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopArccot *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopArccoth *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopArcsec *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopArcsech *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopArccsc *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopArccsch *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopHeaviside *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopSign *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopFloor *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(UniopCeiling *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(ScalarInt *node) {
    }

    void SymbRefFinder::visit(ScalarReal *node) {
    }

    void SymbRefFinder::visit(ScalarBool *node) {
    }

    void SymbRefFinder::visit(ScalarString *node) {
    }

    void SymbRefFinder::visit(BinopPlus *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(BinopMinus *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(BinopDivide *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(BinopTimes *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(BinopPower *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(BinopLogx *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(BinopRoot *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(BinopMin *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(BinopMax *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(BinopRem *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(BinopAtan2 *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(LogicUniopIsdefined *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(LogicUniopNot *node) {
        node->getChild()->accept(this);
    }

    void SymbRefFinder::visit(LogicBinopLt *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(LogicBinopLeq *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(LogicBinopGt *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(LogicBinopGeq *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(LogicBinopEq *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(LogicBinopNeq *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(LogicBinopAnd *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(LogicBinopOr *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(LogicBinopXor *node) {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
    }

    void SymbRefFinder::visit(Vector *node) {
    }

    void SymbRefFinder::visit(Piecewise *node) {
        for (Piece *piece : node->getPieces()) {
            piece->accept(this);
        }
    }

    void SymbRefFinder::visit(Piece *node) {
        if (node->getExpression()) {
            node->getExpression()->accept(this);
        }
        if (node->getCondition()) {
            node->getCondition()->accept(this);
        }
    }

    void SymbRefFinder::visit(Pi *node) {
    }

    void SymbRefFinder::visit(Exponentiale *node) {
    }

    void SymbRefFinder::visit(NullValue *node) {
    }

    void SymbRefFinder::visit(FunctionCall *node) {
        this->symbRefs.insert(node->getFunction());
        for (FunctionArgument *farg : node->getFunctionArguments()) {
            farg->accept(this);
        }
    }

    void SymbRefFinder::visit(FunctionArgument *node) {
        node->getArgument()->accept(this);
    }

    void SymbRefFinder::visit(Interval *node) {
    }
}
