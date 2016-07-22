/* libpharmmlcpp - Library to handle PharmML
 * Copyright (C) 2016 Rikard Nordgren and Gunnar Yngman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * his library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include "Vector.h"

namespace pharmmlcpp
{
    VectorCell::VectorCell(int index, std::shared_ptr<AstNode> content) {
        this->index = index;
        this->content = content;
    }

    int VectorCell::getIndex() {
        return this->index;
    }

    std::shared_ptr<AstNode> VectorCell::getContent() {
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

    void Vector::addElement(std::shared_ptr<AstNode> element) {
        this->elements.push_back(element);
    }

    void Vector::populateCell(VectorCell *cell) {
        // Pad vector if required before insertion
        int index = cell->getIndex();
        int req_length = std::max(this->length, index);
        int current_length = this->elements.size();
        if (current_length < req_length) {
            for (int i = 0; i < (req_length - current_length); i++) {
                this->elements.push_back(std::shared_ptr<AstNode>(this->defaultContent));
            }
        }
        // Insert content
        //AstNode *content = cell->getContent();
        this->elements[index-1] = cell->getContent();
    }

    std::vector<std::shared_ptr<AstNode>> Vector::getElements() {
        // TODO: Add VectorSelector class (problematic integration since it uses a SymbRef to refer to vector)
        return this->elements;
    }

    std::unique_ptr<AstNode> Vector::clone() {
        std::unique_ptr<Vector> cl;
        return std::move(cl);
    }

    void Vector::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }
}
