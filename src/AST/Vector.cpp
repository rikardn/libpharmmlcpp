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
    VectorCell::VectorCell(int index, std::unique_ptr<AstNode> content) {
        this->index = index;
        this->content = std::move(content);
    }

    int VectorCell::getIndex() {
        return this->index;
    }

    AstNode* VectorCell::getContent() {
        return this->content.get();
    }

    Vector::Vector(std::string length, std::string defaultValue) {
    }

    Vector::Vector(PharmMLReader &reader, xml::Node node) {
        std::string length_string = node.getAttribute("length").getValue();
        if (!length_string.empty()) {
            this->length = std::stoi(length_string);
        } else {
            this->length = 0;
        }
        std::string defaultValue = node.getAttribute("default").getValue();
        if (defaultValue.empty()) {
            this->defaultContent = std::make_unique<ScalarReal>(0);
        } else {
            this->defaultContent = std::make_unique<ScalarReal>(defaultValue);
        }

        // Get elements, cells and segments
        std::vector<xml::Node> children = node.getChildren();
        xml::Node vectorElements;
        std::vector<xml::Node> vectorCells;
        std::vector<xml::Node> vectorSegments;
        for (xml::Node node : children) {
            std::string name = node.getName();
            if (name == "VectorElements") {
                vectorElements = node;
            } else if (name == "VectorCell") {
                vectorCells.push_back(node);
            } else if (name == "VectorSegment") {
                vectorSegments.push_back(node);
            }
        }
        if (vectorElements.exists()) {
            // Build vector object from elements
            std::vector<xml::Node> elements = vectorElements.getChildren();
            for (xml::Node element : elements) {
                this->addElement(AstNode::create(reader, element));
            }
        } else if (!(vectorCells.empty() && vectorSegments.empty())) {
            // Build vector from cells
            for (xml::Node cell : vectorCells) {
                std::vector<xml::Node> children = cell.getChildren();
                int cellIndex = std::stoi(children[0].getText());
                std::unique_ptr<AstNode> cellContent = AstNode::create(reader, children[1]);

                VectorCell *vectorCell = new VectorCell(cellIndex, std::move(cellContent));
                this->populateCell(vectorCell);
            }
        }
    }


    xml::Node Vector::xml(PharmMLWriter &writer) {
        xml::Node vec("Vector", xml::Namespace::ct);
            //FIXME: To be filled
        return vec;
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
                this->elements.push_back(std::shared_ptr<AstNode>(this->defaultContent.get()));
            }
        }
        // Insert content
        //AstNode *content = cell->getContent();
        this->elements[index-1] = std::shared_ptr<AstNode>(cell->getContent());
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
