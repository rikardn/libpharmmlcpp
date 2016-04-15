#include "Constant.h"

namespace PharmML
{
    // Null value (example usage: missing otherwise piece)
    void NullValue::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    // Boolean primitives
    void LogicFalse::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    void LogicTrue::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    // Pi and Euler's constant
    void Pi::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
    void Exponentiale::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
}
