#include "Scalar.h"

namespace PharmML
{
    std::string ScalarInt::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    ScalarInt::ScalarInt(int value) {
        this->value = value;
    }

    ScalarInt::ScalarInt(std::string s) {
        this->value = std::stoi(s);
    }

    ScalarInt::ScalarInt(xml::Node xml_node) {
        this->value = std::stoi(xml_node.getText());
    }

    std::string ScalarInt::toString() {
        return std::to_string(value); 
    } 
}
