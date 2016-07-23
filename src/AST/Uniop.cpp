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

#define UNIOP_METHODS(name) \
    std::unique_ptr<AstNode> name::clone() { \
        std::unique_ptr<name> cl = std::make_unique<name>(); \
        if (this->child) { \
            (*cl).child = this->child->clone(); \
        } \
        return std::move(cl); \
    } \
    \
    void name::accept(AstNodeVisitor *visitor) { \
        visitor->visit(this); \
    }

    UNIOP_METHODS(UniopLog)
    UNIOP_METHODS(UniopLog2)
    UNIOP_METHODS(UniopLog10)
    UNIOP_METHODS(UniopExp)
    UNIOP_METHODS(UniopMinus)
    UNIOP_METHODS(UniopAbs)
    UNIOP_METHODS(UniopSqrt)
    UNIOP_METHODS(UniopLogistic)
    UNIOP_METHODS(UniopLogit)
    UNIOP_METHODS(UniopProbit)
    UNIOP_METHODS(UniopNormcdf)
    UNIOP_METHODS(UniopFactorial)
    UNIOP_METHODS(UniopFactln)
    UNIOP_METHODS(UniopGamma)
    UNIOP_METHODS(UniopGammaln)
    UNIOP_METHODS(UniopSin)
    UNIOP_METHODS(UniopSinh)
    UNIOP_METHODS(UniopCos)
    UNIOP_METHODS(UniopCosh)
    UNIOP_METHODS(UniopTan)
    UNIOP_METHODS(UniopTanh)
    UNIOP_METHODS(UniopCot)
    UNIOP_METHODS(UniopCoth)
    UNIOP_METHODS(UniopSec)
    UNIOP_METHODS(UniopSech)
    UNIOP_METHODS(UniopCsc)
    UNIOP_METHODS(UniopCsch)
    UNIOP_METHODS(UniopArcsin)
    UNIOP_METHODS(UniopArcsinh)
    UNIOP_METHODS(UniopArccos)
    UNIOP_METHODS(UniopArccosh)
    UNIOP_METHODS(UniopArctan)
    UNIOP_METHODS(UniopArctanh)
    UNIOP_METHODS(UniopArccot)
    UNIOP_METHODS(UniopArccoth)
    UNIOP_METHODS(UniopArcsec)
    UNIOP_METHODS(UniopArcsech)
    UNIOP_METHODS(UniopArccsc)
    UNIOP_METHODS(UniopArccsch)
    UNIOP_METHODS(UniopHeaviside)
    UNIOP_METHODS(UniopSign)
    UNIOP_METHODS(UniopFloor)
    UNIOP_METHODS(UniopCeiling)
    UNIOP_METHODS(LogicUniopIsdefined)
    UNIOP_METHODS(LogicUniopNot)
}
