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

#include "VariabilityLevel.h"

namespace pharmmlcpp
{
    VariabilityLevel::VariabilityLevel(PharmMLReader &reader, xml::Node node) {
        this->VariabilityLevel::parse(reader, node);
    }

    void VariabilityLevel::parse(PharmMLReader &reader, xml::Node node) {
        // Gets symbId
        this->Symbol::parse(node);

        // Get name and if this level is the reference level
        xml::Node name_node = reader.getSingleElement(node, "./ct:Name");
        if (name_node.exists()) {
            this->name = reader.getSingleElement(node, "./ct:Name").getText();
        }
        this->referenceLevel = node.getAttribute("referenceLevel").getValue() == "true" ? true : false;

        // Get parent reference
        xml::Node parent_ref_node = reader.getSingleElement(node, "./mdef:ParentLevel");
        if (parent_ref_node.exists()) {
            this->parentLevelRef = new SymbRef(reader, parent_ref_node.getChild());
        }
    }

    std::string VariabilityLevel::getName() {
        return this->name;
    }

    bool VariabilityLevel::isReferenceLevel() {
        return this->referenceLevel;
    }

    SymbRef *VariabilityLevel::getParentReference() {
        return this->parentLevelRef;
    }

    void VariabilityLevel::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        if (this->parentLevelRef) {
            this->addSymbRef(this->parentLevelRef, gathering, blkId);
        }
    }

    void VariabilityLevel::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void VariabilityLevel::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }

    VariabilityReference::VariabilityReference(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void VariabilityReference::parse(PharmMLReader &reader, xml::Node node) {
        // Get the variability level reference
        xml::Node ref_node = reader.getSingleElement(node, "./ct:SymbRef");
        this->levelReference = new SymbRef(reader, ref_node);

        // Get random effect mapping (what does it mean?)
        // "Type defining the stdev or variance to be referenced in the VariabilityReference element"
        xml::Node map_node = reader.getSingleElement(node, "./ct:RandomEffectMapping/ct:SymbRef");
        if (map_node.exists()) {
            this->randomEffectsMapping = reader.factory.create(reader, map_node);
        }
    }

    SymbRef *VariabilityReference::getLevelReference() {
        return this->levelReference;
    }

    std::shared_ptr<AstNode> VariabilityReference::getRandomEffectsMapping() {
        return this->randomEffectsMapping;
    }

    void VariabilityReference::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        this->addSymbRef(this->levelReference, gathering, blkId);
    }

    //~ void VariabilityReference::accept(AstNodeVisitor *visitor) {
        //~ visitor->visit(this);
    //~ }
}
