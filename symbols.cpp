#include "symbols.h"
#include <iostream>

namespace PharmML
{
    SymbRef::SymbRef(std::string ref) {
        this->symbIdRef = ref;
    }

    SymbRef::SymbRef(xml::Node node) {
        this->symbIdRef = node.getAttribute("symbIdRef").getValue();
    }

    std::string SymbRef::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    std::string SymbRef::toString() {
        return symbIdRef;
    }
}
