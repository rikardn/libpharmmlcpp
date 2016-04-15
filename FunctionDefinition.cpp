#include "FunctionDefinition.h"

namespace PharmML
{
    FunctionDefinition::FunctionDefinition(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void FunctionDefinition::parse(xml::Node node) {
        this->symbId = node.getAttribute("symbId").getValue();

        xml::Node assign = this->context->getSingleElement(node, ".//ct:Assign");
        xml::Node tree = assign.getChild();
        this->assignment = this->context->factory.create(tree);

        std::vector<xml::Node> args = this->context->getElements(node, ".//ct:FunctionArgument");
        for (xml::Node a : args) {
            this->arguments.push_back(a.getAttribute("symbId").getValue());
        }
    }

    std::string FunctionDefinition::getSymbId() {
        return this->symbId;
    }
    
    std::vector<std::string> FunctionDefinition::getArguments() {
        return this->arguments;
    }

    AstNode *FunctionDefinition::getAssignment() {
        return this->assignment;
    }

    void FunctionDefinition::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
}
