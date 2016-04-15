#include "Uniop.h"

namespace PharmML
{
    AstNode *Uniop::getChild() {
        return this->child;
    }

    void Uniop::setChild(AstNode *node) {
        delete child;
        this->child = node;
    }

    // Ordinary uniops
    void UniopLog::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopLog2::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopLog10::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }

    void UniopExp::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }

    void UniopMinus::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopAbs::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopSqrt::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopLogistic::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopLogit::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopProbit::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopNormcdf::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopFactorial::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopFactln::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopGamma::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopGammaln::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopSin::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopSinh::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopCos::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopCosh::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopTan::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopTanh::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopCot::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopCoth::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopSec::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopSech::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopCsc::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopCsch::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArcsin::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArcsinh::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArccos::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArccosh::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArctan::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArctanh::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArccot::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArccoth::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArcsec::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArcsech::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArccsc::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopArccsch::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopHeaviside::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopSign::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopFloor::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void UniopCeiling::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    // Logic uniops
    void LogicUniopIsdefined::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
    
    void LogicUniopNot::accept(AbstractVisitor *visitor) {
         visitor->visit(this);
    }
}
