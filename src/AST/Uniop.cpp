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

namespace PharmML
{
    AstNode *Uniop::getChild() {
        return this->child;
    }

    void Uniop::setChild(AstNode *node) {
        // FIXME: how to dispose of AstNodes ? delete child;
        this->child = node;
    }

    // Ordinary uniops
    void UniopLog::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopLog2::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopLog10::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }

    void UniopExp::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }

    void UniopMinus::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopAbs::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopSqrt::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopLogistic::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopLogit::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopProbit::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopNormcdf::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopFactorial::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopFactln::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopGamma::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopGammaln::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopSin::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopSinh::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopCos::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopCosh::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopTan::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopTanh::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopCot::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopCoth::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopSec::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopSech::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopCsc::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopCsch::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArcsin::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArcsinh::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArccos::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArccosh::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArctan::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArctanh::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArccot::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArccoth::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArcsec::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArcsech::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArccsc::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArccsch::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopHeaviside::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopSign::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopFloor::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopCeiling::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    // Logic uniops
    void LogicUniopIsdefined::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
    
    void LogicUniopNot::accept(AstNodeVisitor *visitor) {
         visitor->visit(this);
    }
}
