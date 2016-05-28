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

#include "ColumnMapping.h"

namespace PharmML
{
    // TODO: Move elsewhere (Dataset.h?)
    TargetMapping::TargetMapping(PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
    }

    void TargetMapping::parse(xml::Node node) {
        // According to spec this is specifically to allow multiple structural models
        this->blkIdRef = node.getAttribute("blkIdRef").getValue();

        // Get maps
        std::vector<xml::Node> map_nodes = this->context->getElements(node, "./ds:Map");
        for (xml::Node map_node : map_nodes) {
            MapType map;

            map.dataSymbol = map_node.getAttribute("dataSymbol").getValue();
            map.modelSymbol = map_node.getAttribute("modelSymbol").getValue();
            map.admNumber = map_node.getAttribute("admNumber").getValue();

            this->maps.push_back(map);
        }
    }

    std::string TargetMapping::getBlkIdRef() {
        return this->blkIdRef;
    }

    std::vector<MapType> TargetMapping::getMaps() {
        return this->maps;
    }

    void TargetMapping::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    ColumnMapping::ColumnMapping(PharmML::PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void ColumnMapping::parse(xml::Node node) {
        xml::Node ref_node = this->context->getSingleElement(node, "./ds:ColumnRef");
        this->columnIdRef = ref_node.getAttribute("columnIdRef").getValue();
        xml::Node assign_node = this->context->getSingleElement(node, "./ct:Assign");
        xml::Node symbref_node = this->context->getSingleElement(node, "./ct:SymbRef");
        xml::Node piecewise_node = this->context->getSingleElement(node, "./ds:Piecewise");
        // TODO: Support CategoryMapping (for categorical covariates)

        // Store mapping expression (should only contain one symbol reference)
        if (symbref_node.exists()) {
            this->symbRef = new SymbRef(symbref_node);
        } else if (assign_node.exists()) {
            this->assignment = this->context->factory.create(assign_node);
        } else if (piecewise_node.exists()) {
            this->assignment = this->context->factory.create(piecewise_node);
        }

        // Get target map (e.g. to PK macros)
        // FIXME: Assume just one target mapping until we know definitely what unlimited maps means
        xml::Node target_node = this->context->getSingleElement(node, "./ds:TargetMapping");
        if (target_node.exists()) {
            TargetMapping *map = new TargetMapping(this->context, target_node);
            this->target_map = map;
        }
    }

    xml::Node ColumnMapping::xml() {
        xml::Node cm("ColumnMapping");
        xml::Node idref("ColumnRef", xml::Namespace::ds);
        idref.setAttribute("columnIdRef", this->columnIdRef);
        cm.addChild(idref);
        XMLAstVisitor xml;
        this->assignment->accept(&xml);
        cm.addChild(xml.getValue());
        return cm;
    }

    AstNode *ColumnMapping::getAssignment() {
        return this->assignment;
    }

    std::string ColumnMapping::getColumnIdRef() {
        return this->columnIdRef;
    }

    Symbol *ColumnMapping::getMappedSymbol() {
        return this->mappedSymbol;
    }

    TargetMapping *ColumnMapping::getTargetMapping() {
        return this->target_map;
    }

    // Convenience method to scan and get all administration->data symbol maps
    std::unordered_map<int, std::string> ColumnMapping::getAdministrationMap() {
        std::unordered_map<int, std::string> adm_map;
        if (!this->target_map) {
            return adm_map;
        }
        std::vector<PharmML::MapType> maps = target_map->getMaps();
        for (PharmML::MapType map : maps) {
            // Presence of both dataSymbol and admNumber indicates a map to (macro) administration
            if (map.dataSymbol != "" && map.admNumber != "") {
                int adm_number;
                if (PharmML::AstAnalyzer::tryParseInt(map.admNumber, adm_number)) {
                    adm_map[adm_number] = map.dataSymbol;
                }
            }
        }
        return adm_map;
    }

    void ColumnMapping::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        if (this->symbRef) {
            this->mappedSymbol = this->addSymbRef(this->symbRef, symbolMap);
        } else if (this->assignment) {
            std::unordered_set<Symbol *> symbols = this->symbRefsFromAst(this->assignment, symbolMap);
            this->mappedSymbol = *(symbols.begin()); // There shall only be one
        }
    }

    void ColumnMapping::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
