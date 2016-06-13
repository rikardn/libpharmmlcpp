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
    VariabilityLevel::VariabilityLevel(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->VariabilityLevel::parse(node);
    }

    void VariabilityLevel::parse(xml::Node node) {
        // Gets symbId
        this->Symbol::parse(node);

        // Get name and if this level is the reference level
        xml::Node name_node = this->context->getSingleElement(node, "./ct:Name");
        if (name_node.exists()) {
            this->name = this->context->getSingleElement(node, "./ct:Name").getText();
        }
        this->referenceLevel = node.getAttribute("referenceLevel").getValue() == "true" ? true : false;

        // Get parent reference
        xml::Node parent_ref_node = this->context->getSingleElement(node, "./mdef:ParentLevel");
        if (parent_ref_node.exists()) {
            this->parentLevelRef = new SymbRef(parent_ref_node.getChild());
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

    VariabilityReference::VariabilityReference(pharmmlcpp::PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void VariabilityReference::parse(xml::Node node) {
        // Get the variability level reference
        xml::Node ref_node = this->context->getSingleElement(node, "./ct:SymbRef");
        this->levelReference = new SymbRef(ref_node);

        // Get random effect mapping (what does it mean?)
        // "Type defining the stdev or variance to be referenced in the VariabilityReference element"
        xml::Node map_node = this->context->getSingleElement(node, "./ct:RandomEffectMapping/ct:SymbRef");
        if (map_node.exists()) {
            this->randomEffectsMapping = this->context->factory.create(map_node);
        }
    }

    SymbRef *VariabilityReference::getLevelReference() {
        return this->levelReference;
    }

    AstNode *VariabilityReference::getRandomEffectsMapping() {
        return this->randomEffectsMapping;
    }

    void VariabilityReference::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        this->addSymbRef(this->levelReference, gathering, blkId);
    }

    //~ void VariabilityReference::accept(AstNodeVisitor *visitor) {
        //~ visitor->visit(this);
    //~ }
}
