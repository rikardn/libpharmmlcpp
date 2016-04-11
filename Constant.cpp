#include "Constant.h"

namespace PharmML
{
    // Null value (example usage: missing otherwise piece)
    std::string NullValue::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    // Boolean primitives
    std::string LogicFalse::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
    
    std::string LogicTrue::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
}
