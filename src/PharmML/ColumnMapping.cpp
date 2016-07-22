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

namespace pharmmlcpp
{
    // TODO: Move elsewhere (Dataset.h?)
    TargetMapping::TargetMapping(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->parse(reader, node);
    }

    void TargetMapping::parse(PharmMLReader &reader, xml::Node node) {
        // According to spec this is specifically to allow multiple structural models
        this->blkIdRef = node.getAttribute("blkIdRef").getValue();

        // Get maps
        std::vector<xml::Node> map_nodes = reader.getElements(node, "./ds:Map");
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

    // Get vector of (raw) maps
    std::vector<MapType> TargetMapping::getMaps() {
        return this->maps;
    }

    // Get the set of all symbol strings
    std::unordered_set<std::string> TargetMapping::getSymbolStrings() {
        std::unordered_set<std::string> set;
        for (MapType map : this->maps) {
            if (!map.modelSymbol.empty()) {
                set.insert(map.modelSymbol);
            }
        }
        return set;
    }

    // Get the set of all administration numbers
    std::unordered_set<int> TargetMapping::getAdmNumbers() {
        std::unordered_set<int> set;
        for (MapType map : this->maps) {
            if (!map.admNumber.empty()) {
                set.insert(std::stoi(map.admNumber));
            }
        }
        return set;
    }

    // Get a (resolved) complete map from data symbol strings to the Symbol objects
    std::unordered_map<std::string, Symbol *> TargetMapping::getDataSymbolMap() {
        // Create associative array
        std::unordered_map<std::string, Symbol *> data_to_symbol;
        for (pharmmlcpp::MapType map : this->maps) {
            if (!map.modelSymbol.empty()) {
                data_to_symbol[map.dataSymbol] = map.symbol;
            }
        }
        return data_to_symbol;
    }

    // Get a (resolved) complete map from data symbol strings to (administration) PKMacro objects
    std::unordered_map<std::string, PKMacro *> TargetMapping::getDataMacroMap() {
        // Create associative array
        std::unordered_map<std::string, PKMacro *> data_to_macro;
        for (pharmmlcpp::MapType map : this->maps) {
            if (!map.admNumber.empty()) {
                data_to_macro[map.dataSymbol] = map.macro;
            }
        }
        return data_to_macro;
    }

    // Fill modelSymbol string containing maps data with gathered Symbol object
    void TargetMapping::setupSymbolRefs(SymbolGathering &gathering) {
        for (MapType &map : this->maps) {
            if (!map.modelSymbol.empty()) {
                pharmmlcpp::Symbol *symbol = gathering.getSymbol(this->blkIdRef, map.modelSymbol);
                if (symbol) {
                    map.symbol = symbol;
                } else {
                    gathering.logger.error("TargetMapping element contains non-resolvable 'modelSymbol' (" + map.modelSymbol + ")", this);
                }
            }
        }
    }

   // Fill admNumber string containing maps with gathered (administration) PKMacro object
    void TargetMapping::setupMacroRefs(MacroGathering &gathering) {
        for (MapType &map : this->maps) {
            if (!map.admNumber.empty()) {
                pharmmlcpp::PKMacro *macro = gathering.getAdmMacro(this->blkIdRef, std::stoi(map.admNumber));
                if (macro) {
                    map.macro = macro;
                } else {
                    gathering.logger.error("TargetMapping element contains non-resolvable 'admNumber' (" + map.admNumber + ")", this);
                }
            }
        }
    }

    void TargetMapping::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    ColumnMapping::ColumnMapping(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void ColumnMapping::parse(PharmMLReader &reader, xml::Node node) {
        xml::Node ref_node = reader.getSingleElement(node, "./ds:ColumnRef");
        this->columnIdRef = ref_node.getAttribute("columnIdRef").getValue();
        xml::Node assign_node = reader.getSingleElement(node, "./ct:Assign");
        xml::Node symbref_node = reader.getSingleElement(node, "./ct:SymbRef");
        xml::Node piecewise_node = reader.getSingleElement(node, "./ds:Piecewise");
        // TODO: Support CategoryMapping (for categorical covariates)

        // Store mapping expression (should only contain one symbol reference)
        if (symbref_node.exists()) {
            this->symbRef = new SymbRef(symbref_node);
        } else if (assign_node.exists()) {
            this->assignment = reader.factory.create(assign_node);
        } else if (piecewise_node.exists()) {
            this->assignment = reader.factory.create(piecewise_node);
        }

        // Get target map (e.g. to PK macros)
        // FIXME: Assume just one target mapping until we know definitely what unlimited maps means
        xml::Node target_node = reader.getSingleElement(node, "./ds:TargetMapping");
        if (target_node.exists()) {
            TargetMapping *map = new TargetMapping(reader, target_node);
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

    std::shared_ptr<AstNode> ColumnMapping::getAssignment() {
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

    void ColumnMapping::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        if (this->symbRef) {
            this->mappedSymbol = this->addSymbRef(this->symbRef, gathering, blkId);
        } else if (this->assignment) {
            this->setupAstSymbRefs(this->assignment.get(), gathering, blkId);
            this->mappedSymbol = *(this->referencedSymbols.begin()); // There shall only be one
        }
        if (this->target_map) {
            this->target_map->setupSymbolRefs(gathering);
        }
    }

    void ColumnMapping::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
