#include "Scalar.h"

namespace PharmML
{
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

    std::string ScalarInt::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }

    ScalarReal::ScalarReal(double value) {
        this->value = value;
    }

    ScalarReal::ScalarReal(std::string s) {
        this->value = std::stod(s);
    }

    ScalarReal::ScalarReal(xml::Node xml_node) {
        this->value = std::stod(xml_node.getText());
    }

    std::string ScalarReal::toString() {
        return std::to_string(value); 
    } 

    std::string ScalarReal::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
}
