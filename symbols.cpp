#include "symbols.h"
#include <iostream>

namespace PharmML
{
    SymbRef::SymbRef(std::string ref) {
        this->symbIdRef = ref;
    }

    std::string SymbRef::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    std::string SymbRef::toString() {
        return symbIdRef;
    }
}
