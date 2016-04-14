#include "symbols.h"
#include <iostream>

namespace PharmML
{
    SymbRef::SymbRef(std::string ref) {
        this->symbIdRef = ref;
    }

    SymbRef::SymbRef(xml::Node node) {
        this->blkIdRef = node.getAttribute("blkIdRef").getValue();
        this->symbIdRef = node.getAttribute("symbIdRef").getValue();
    }

    std::string SymbRef::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    std::string SymbRef::toString() {
        return symbIdRef;
    }

    ColumnRef::ColumnRef(std::string ref) {
        this->columnIdRef = ref;
    }

    ColumnRef::ColumnRef(xml::Node node) {
        this->columnIdRef = node.getAttribute("columnIdRef").getValue();
        this->columnIdRef = node.getAttribute("columnIdRef").getValue();
    }

    std::string ColumnRef::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    std::string ColumnRef::toString() {
        return this->columnIdRef;
    }
}
