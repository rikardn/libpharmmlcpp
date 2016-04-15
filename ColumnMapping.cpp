#include "ColumnMapping.h"

namespace PharmML
{
    ColumnMapping::ColumnMapping(PharmML::PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void ColumnMapping::parse(xml::Node node) {
        xml::Node column_ref = node.getChild();
        this->columnIdRef = column_ref.getAttribute("columnIdRef").getValue();
        xml::Node assign_ref = node.getLastChild();     // Currently can support one symbref or one piecewise
        this->assignment = this->context->factory.create(assign_ref);
    }

    AstNode *ColumnMapping::getAssignment() {
        return this->assignment;
    }

    std::string ColumnMapping::getColumnIdRef() {
        return this->columnIdRef;
    }
    
    void ColumnMapping::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
}
