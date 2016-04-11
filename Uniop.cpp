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

    std::string Uniop::acceptChild(AbstractVisitor *visitor) {
        return this->child->accept(visitor);
    }

    std::string UniopLog::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopLog2::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopLog10::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    std::string UniopExp::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    std::string UniopMinus::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopAbs::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopSqrt::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopLogistic::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopLogit::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopProbit::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopNormcdf::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopFactorial::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopFactln::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopGamma::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopGammaln::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopSin::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopSinh::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopCos::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopCosh::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopTan::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopTanh::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopCot::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopCoth::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopSec::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopSech::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopCsc::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopCsch::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopArcsin::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopArcsinh::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopArccos::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopArccosh::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopArctan::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopArctanh::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopArccot::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopArccoth::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopArcsec::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopArcsech::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopArccsc::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopArccsch::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopHeaviside::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopSign::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopFloor::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string UniopCeiling::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
}
