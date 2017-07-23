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

#include "MatrixVectorIndex.h"
#include <visitors/AstAnalyzer.h>

namespace pharmmlcpp
{
    /**
     *  Creates a new MatrixVectorIndex from the xml PharmML code
     */
    MatrixVectorIndex::MatrixVectorIndex(PharmMLReader &reader, xml::Node node) {
        xml::Node assign = reader.getSingleElement(node, "./ct:Assign");
        xml::Node pure_int = reader.getSingleElement(node, "./ct:Int");
        xml::Node pure_symbref = reader.getSingleElement(node, "./ct:SymbRef");
        if (assign.exists()) {
            this->index = AstNode::create(reader, assign);
        } else if (pure_int.exists()) {
            this->index = AstNode::create(reader, pure_int);
        } else if (pure_symbref.exists()) {
            this->index = AstNode::create(reader, pure_symbref);
        }
    }

    xml::Node MatrixVectorIndex::xml(PharmMLWriter &writer, std::string name) {
        xml::Node index(name, xml::Namespace::ct);
        AstAnalyzer analyzer;
        this->index->accept(&analyzer);
        if (analyzer.getPureScalarInt() || analyzer.getPureSymbRef()) {
            index.addChild(this->index->xml(writer));
        } else {
            xml::Node assign("Assign", xml::Namespace::ct);
            index.addChild(assign);
            assign.addChild(this->index->xml(writer));
        }
        return index;
    }

    /**
     *  Creates a new MatrixVectorIndex
     */
    MatrixVectorIndex::MatrixVectorIndex(std::unique_ptr<AstNode> index) {
        if (!index) {
            throw std::invalid_argument("nullptr");
        }
        this->index = std::move(index);
    }

    /**
     *  Copy constructor
     */
    MatrixVectorIndex::MatrixVectorIndex(const MatrixVectorIndex& from) {
        this->index = from.index->clone();
    }

    /**
     *  Get the index
     */
    AstNode *MatrixVectorIndex::getIndex() {
        return this->index.get();
    }

    /**
     *  Set the index
     */
    void MatrixVectorIndex::setIndex(std::unique_ptr<AstNode> index) {
        if (!index) {
            throw std::invalid_argument("nullptr");
        }
        this->index = std::move(index);
    }

    /**
     *  Make a clone (deep copy) of this interval.
     */
    std::unique_ptr<MatrixVectorIndex> MatrixVectorIndex::clone() {
        std::unique_ptr<MatrixVectorIndex> cl = std::make_unique<MatrixVectorIndex>(this->index->clone());
        return std::move(cl);
    }
}
