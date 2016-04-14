#include "Vector.h"

namespace PharmML
{
    VectorCell::VectorCell(int index, AstNode *content) {
        this->index = index;
        this->content = content;
    }
    
    int VectorCell::getIndex() {
        return this->index;
    }
    
    AstNode *VectorCell::getContent() {
        return this->content;
    }
    
    Vector::Vector(std::string length, std::string defaultValue) {
        this->defaultContent = new ScalarReal(0);
        this->length = 0;
        if (defaultValue != "") {
            this->defaultContent = new ScalarReal(std::stod(defaultValue));
        }
        if (length != "") {
            this->length = std::stoi(length);
        }
    }
    
    void Vector::addElement(AstNode *element) {
        this->elements.push_back(element);
    }
    
    void Vector::populateCell(VectorCell *cell) {
        // Pad vector if required before insertion
        int index = cell->getIndex();
        int req_length = std::max(this->length, index);
        int current_length = this->elements.size();
        if (current_length < req_length) {
            for (int i = 0; i < (req_length - current_length); i++) {
                this->elements.push_back(this->defaultContent);
            }
        }
        // Insert content
        AstNode *content = cell->getContent();
        this->elements[index-1] = content;
    }
    
    std::vector<AstNode *> Vector::getElements() {
        // TODO: Add VectorSelector class (problematic integration since it uses a SymbRef to refer to vector)
        return this->elements;
    }
    
    std::string Vector::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
}
