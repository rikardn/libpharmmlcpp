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

#include "symbols.h"
#include <iostream>

namespace pharmmlcpp
{
    /**
     *  Creates a new SymbRef given a symbId
     */
    SymbRef::SymbRef(std::string ref) {
        this->symbIdRef = ref;
    }

    /**
     *  Creates a new SymbRef from the xml PharmML code
     */
    SymbRef::SymbRef(PharmMLReader &reader, xml::Node node) {
        this->blkIdRef = node.getAttribute("blkIdRef").getValue();
        this->symbIdRef = node.getAttribute("symbIdRef").getValue();
    }

    xml::Node SymbRef::xml(PharmMLWriter &writer) {
        xml::Node symbref("SymbRef", xml::Namespace::ct);
        symbref.setAttribute("symbIdRef", this->symbIdRef);
        symbref.setAttribute("blkIdRef", this->blkIdRef);
        return symbref;
    }

    void SymbRef::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    /**
     *  Make a clone of this symbref.
     *  Note that the symbol to which it refers will not be copied just the pointer to it.
     */
    std::unique_ptr<AstNode> SymbRef::clone() {
        std::unique_ptr<SymbRef> cl = std::make_unique<SymbRef>(this->symbIdRef);
        cl->blkIdRef = this->blkIdRef;
        cl->symbol = this->symbol;  // Refers to same symbol. No deep copy here
        return std::move(cl);
    }

    /**
     *  Get the symbol Id to which this SymbRef refers
     */
    std::string SymbRef::getSymbIdRef() {
        return this->symbIdRef;
    }

    /**
     *  Get the block Id for the symbol to which this SymbRef refers
     */
    std::string SymbRef::getBlkIdRef() {
        return this->blkIdRef;
    }

    /**
     *  Get the block Id for the symbol to which this SymbRef refers
     *  and return defaultBlkId if none exists.
     */
    std::string SymbRef::getBlkIdRef(std::string defaultBlkId) {
        if (this->blkIdRef.empty()) {
            return defaultBlkId;
        } else {
            return this->blkIdRef;
        }
    }

    /**
     *  Set the symbol to which this SymbRef refers
     *  This is done by the setup methods and should
     *  not be called separately.
     */
    void SymbRef::setSymbol(Symbol *symbol) {
        this->symbol = symbol;
    }

    /**
     *  Set the symbIdRef
     *  Note that this will not automatically change the symbol
     */
    void SymbRef::setSymbIdRef(std::string symbId) {
        this->symbIdRef = symbId;
    }

    /**
     *  Set the blkIdRef
     *  Note that this will not automatically change the symbol
     */
    void SymbRef::setBlkIdRef(std::string blkId) {
        this->blkIdRef = blkId;
    }

    /**
     *  Get a pointer to the symbol to which this SymbRef refers
     */
    Symbol *SymbRef::getSymbol() {
        return this->symbol;
    }


    ColumnRef::ColumnRef(std::string ref) {
        this->columnIdRef = ref;
    }

    ColumnRef::ColumnRef(PharmMLReader &reader, xml::Node node) {
        this->columnIdRef = node.getAttribute("columnIdRef").getValue();
    }

    xml::Node ColumnRef::xml(PharmMLWriter &writer) {
        xml::Node colref("SymbRef", xml::Namespace::ct);
        colref.setAttribute("columnIdRef", this->columnIdRef);
        return colref;
    }

    void ColumnRef::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    std::unique_ptr<AstNode> ColumnRef::clone() {
        std::unique_ptr<ColumnRef> cl;
        return std::move(cl);
    }

    std::string ColumnRef::getColumnIdRef() {
        return this->columnIdRef;
    }

    std::string ColumnRef::toString() {
        return this->columnIdRef;
    }

    /// Create a category reference object from string
    CatRef::CatRef(std::string ref) {
        this->catRef = ref;
    }

    /**
     *  Creates a new CatRef from the xml PharmML code
     */
    CatRef::CatRef(PharmMLReader &reader, xml::Node node) {
        this->blkIdRef = node.getAttribute("blkIdRef").getValue();
        this->catRef = node.getAttribute("catIdRef").getValue();
    }

    /**
     *  Get the category Id to which this CatRef refers
     */
    std::string CatRef::getCatRef() {
        return this->catRef;
    }

    /**
     *  Get the block Id for the category to which this CatRef refers
     */
    std::string CatRef::getBlkIdRef() {
        return this->blkIdRef;
    }

    /**
     *  Get the block Id for the category to which this CatRef refers
     *  and return defaultBlkId if none exists.
     */
    std::string CatRef::getBlkIdRef(std::string defaultBlkId) {
        if (this->blkIdRef.empty()) {
            return defaultBlkId;
        } else {
            return this->blkIdRef;
        }
    }

    // FIXME: Use implemented Category class in new reference system
    // #<{(|*
    //  *  Set the category to which this CatRef refers
    //  *  This is done by the setup methods and should
    //  *  not be called separately.
    //  |)}>#
    // void CatRef::setCategory(Category *category) {
    //     this->category = category;
    // }

    /**
     *  Set the catIdRef
     *  Note that this will not automatically change the category
     */
    void CatRef::setCatRef(std::string ref) {
        this->catRef = ref;
    }

    /**
     *  Set the blkIdRef
     *  Note that this will not automatically change the category
     */
    void CatRef::setBlkIdRef(std::string blkId) {
        this->blkIdRef = blkId;
    }

    // FIXME: Use implemented Category class in new reference system
    // #<{(|*
    //  *  Get a pointer to the category to which this CatRef refers
    //  |)}>#
    // Category *CatRef::getCategory() {
    //     return this->category;
    // }

    xml::Node CatRef::xml(PharmMLWriter &writer) {
        xml::Node catref("CatRef", xml::Namespace::ct);
        catref.setAttribute("catIdRef", this->catRef);
        return catref;
    }

    void CatRef::accept(AstNodeVisitor *visitor) {
        visitor->visit(this);
    }

    std::unique_ptr<AstNode> CatRef::clone() {
        std::unique_ptr<CatRef> cl;
        return std::move(cl);
    }

    std::string CatRef::toString() {
        return this->catRef;
    }
}
