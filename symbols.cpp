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

    void SymbRef::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }

    std::string SymbRef::toString() {
        return symbIdRef;
    }

    ColumnRef::ColumnRef(std::string ref) {
        this->columnIdRef = ref;
    }

    ColumnRef::ColumnRef(xml::Node node) {
        // Why duplicate lines?
        this->columnIdRef = node.getAttribute("columnIdRef").getValue();
        this->columnIdRef = node.getAttribute("columnIdRef").getValue();
    }

    void ColumnRef::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }

    std::string ColumnRef::toString() {
        return this->columnIdRef;
    }
}
