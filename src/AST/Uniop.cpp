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

#include "Uniop.h"

namespace pharmmlcpp
{
    /**
     *  Get a pointer to the child node
     */
    AstNode *Uniop::getChild() {
        return this->child.get();
    }

    /**
     *  Set a new child node
     */
    void Uniop::setChild(std::unique_ptr<AstNode> node) {
        this->child = std::move(node);
    }

#define UNIOP_METHODS(name, type, op) \
    std::unique_ptr<AstNode> name::clone() { \
        std::unique_ptr<AstNode> child_clone = this->child->clone(); \
        std::unique_ptr<name> cl = std::make_unique<name>(std::move(child_clone)); \
        return std::move(cl); \
    } \
    \
    void name::accept(AstNodeVisitor *visitor) { \
        visitor->visit(this); \
    } \
    xml::Node name::xml(PharmMLWriter &writer) { \
        xml::Node uniop(type, xml::Namespace::math); \
        uniop.setAttribute("op", op); \
        uniop.addChild(this->child->xml(writer)); \
        return uniop; \
    }

    UNIOP_METHODS(UniopLog, "Uniop", "log")
    UNIOP_METHODS(UniopLog2, "Uniop", "log2")
    UNIOP_METHODS(UniopLog10, "Uniop", "log10")
    UNIOP_METHODS(UniopExp, "Uniop", "exp")
    UNIOP_METHODS(UniopMinus, "Uniop", "minus")
    UNIOP_METHODS(UniopAbs, "Uniop", "abs")
    UNIOP_METHODS(UniopSqrt, "Uniop", "sqrt")
    UNIOP_METHODS(UniopLogistic, "Uniop", "logistic")
    UNIOP_METHODS(UniopLogit, "Uniop", "logit")
    UNIOP_METHODS(UniopProbit, "Uniop", "probit")
    UNIOP_METHODS(UniopNormcdf, "Uniop", "normcdf" )
    UNIOP_METHODS(UniopFactorial, "Uniop", "factorial")
    UNIOP_METHODS(UniopFactln, "Uniop", "factln")
    UNIOP_METHODS(UniopGamma, "Uniop", "gamma")
    UNIOP_METHODS(UniopGammaln, "Uniop", "gammaln")
    UNIOP_METHODS(UniopSin, "Uniop", "sin")
    UNIOP_METHODS(UniopSinh, "Uniop", "sinh")
    UNIOP_METHODS(UniopCos, "Uniop", "cos")
    UNIOP_METHODS(UniopCosh, "Uniop", "cosh")
    UNIOP_METHODS(UniopTan, "Uniop", "tan")
    UNIOP_METHODS(UniopTanh, "Uniop", "tanh")
    UNIOP_METHODS(UniopCot, "Uniop", "cot")
    UNIOP_METHODS(UniopCoth, "Uniop", "coth")
    UNIOP_METHODS(UniopSec, "Uniop", "sec")
    UNIOP_METHODS(UniopSech, "Uniop", "sech")
    UNIOP_METHODS(UniopCsc, "Uniop", "csc")
    UNIOP_METHODS(UniopCsch, "Uniop", "csch")
    UNIOP_METHODS(UniopArcsin, "Uniop", "arcsin")
    UNIOP_METHODS(UniopArcsinh, "Uniop", "arcsinh")
    UNIOP_METHODS(UniopArccos, "Uniop", "arccos")
    UNIOP_METHODS(UniopArccosh, "Uniop", "arccosh")
    UNIOP_METHODS(UniopArctan, "Uniop", "arctan")
    UNIOP_METHODS(UniopArctanh, "Uniop", "arctanh")
    UNIOP_METHODS(UniopArccot, "Uniop", "arccot")
    UNIOP_METHODS(UniopArccoth, "Uniop", "arccoth")
    UNIOP_METHODS(UniopArcsec, "Uniop", "arcsec")
    UNIOP_METHODS(UniopArcsech, "Uniop", "arcsech")
    UNIOP_METHODS(UniopArccsc, "Uniop", "arccsc")
    UNIOP_METHODS(UniopArccsch, "Uniop", "arccsch")
    UNIOP_METHODS(UniopHeaviside, "Uniop", "heaviside")
    UNIOP_METHODS(UniopSign, "Uniop", "sign")
    UNIOP_METHODS(UniopFloor, "Uniop", "floor")
    UNIOP_METHODS(UniopCeiling, "Uniop", "ceiling")
    UNIOP_METHODS(LogicUniopIsdefined, "LogicUniop", "isDefined")
    UNIOP_METHODS(LogicUniopNot, "LogicUniop", "not")
    UNIOP_METHODS(MatrixUniopDeterminant, "MatrixUniop", "determinant")
    UNIOP_METHODS(MatrixUniopInverse, "MatrixUniop", "inverse")
    UNIOP_METHODS(MatrixUniopTrace, "MatrixUniop", "trace")
    UNIOP_METHODS(MatrixUniopTranspose, "MatrixUniop", "transpose")
}
