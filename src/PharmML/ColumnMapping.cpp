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

    void TargetMapping::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        // TODO: Is this right? How else should these strings be resolved to real symbols?
        for (MapType map : this->maps) {
            if (map.modelSymbol != "") {
                PharmML::Symbol *symbol = gathering.getSymbol(blkId, map.modelSymbol);
                map.modelSymbol_ptr = symbol;
                this->addReference(symbol);
            }
        }
    }

    void TargetMapping::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    ColumnMapping::ColumnMapping(PharmML::PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
        this->postParse();
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
                if (StringTyper::isInt(map.admNumber, adm_number)) {
                    adm_map[adm_number] = map.dataSymbol;
                }
            }
        }
        return adm_map;
    }

    void ColumnMapping::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        if (this->symbRef) {
            this->mappedSymbol = this->addSymbRef(this->symbRef, gathering, blkId);
        } else if (this->assignment) {
            this->setupAstSymbRefs(this->assignment, gathering, blkId);
            this->mappedSymbol = *(this->referencedSymbols.begin()); // There shall only be one
        }
    }

    void ColumnMapping::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    // POST PARSE/CONSOLIDATION
    // Perform post-parse functions to enable higher-level abstraction/consolidation
    void ColumnMapping::postParse() {
        Logger logger("Post");

        // Deal with mapped symbols
        if (this->mappedSymbol) {
            this->num_maps++;
        }

        // TODO: Deal with categorical covariate mapping

        // Deal with target maps
        if (this->target_map) {
            // Create associative arrays of (data) symbols to model (symbols and administrations)
            for (PharmML::MapType map : this->target_map->getMaps()) {
                if (map.modelSymbol != "") {
                    this->symbol_to_data[map.modelSymbol] = map.dataSymbol;
                    if (map.modelSymbol_ptr) {
                        this->data_to_symbol_ptr[map.dataSymbol] = map.modelSymbol_ptr;
                    } else {
                        logger.error("TargetMapping element contains non-resolvable 'modelSymbol': " + map.modelSymbol, target_map);
                    }
                    this->num_maps++;
                } else if (map.admNumber != "") {
                    int adm;
                    if (StringTyper::isInt(map.admNumber, adm)) {
                        this->adm_to_data[adm] = map.dataSymbol;
                    } else {
                        logger.error("TargetMapping element contains non-integer 'admNumber': " + map.admNumber, target_map);
                    }
                    this->num_maps++;
                } else {
                    logger.error("TargetMapping element doesn't 'modelSymbol' nor 'admNumber'", target_map);
                }
            }
        }
    }

    // True if this column mapping maps multiple symbols/administrations
    bool ColumnMapping::mapsMultiple() {
        return (this->num_maps > 1);
    }

    // Get the set of all (TargetMapping) symbol strings contained within this column mapping
    std::unordered_set<std::string> ColumnMapping::getSymbolStrings() {
        std::unordered_set<std::string> symb_strs;
        for (std::pair<std::string, std::string> pair : this->symbol_to_data) {
            symb_strs.insert(pair.first);
        }
        return symb_strs;
    }

    // Get the set of all (TargetMapping) administration numbers contained within this column mapping
    std::unordered_set<int> ColumnMapping::getAdmNumbers() {
        std::unordered_set<int> adm_nums;
        for (std::pair<int, std::string> pair : this->adm_to_data) {
            adm_nums.insert(pair.first);
        }
        return adm_nums;
    }

    // Get a (resolved) complete (TargetMapping) map from data symbol strings to the PharmML::Symbol pointers
    std::unordered_map<std::string, PharmML::Symbol *> ColumnMapping::getDataSymbolMap() {
        return data_to_symbol_ptr;
    }

    // Get a (resolved) complete (TargetMapping) map from data symbol strings to PharmML::PKMacro pointers
    std::unordered_map<std::string, PharmML::PKMacro *> ColumnMapping::getDataAdministrationMap(std::vector<PharmML::PKMacro *> macros,
        PharmML::PharmMLSection *ext_ds_section = nullptr, PharmML::PharmMLSection *pk_macros_section = nullptr) {
            
        Logger logger("Post");
        std::unordered_map<std::string, PharmML::PKMacro *> data_to_adm_macro;

        for (PharmML::PKMacro *macro : macros) {
            // Only consider administration macros as potential targets
            AstNode *adm_assignment = macro->getAssignment("adm");
            AstAnalyzer ast_analyzer;
            adm_assignment->accept(&ast_analyzer);
            ScalarInt *adm_scint = ast_analyzer.getPureScalarInt();
            if (adm_scint) {
                int adm_num;
                if (StringTyper::isInt(adm_scint->toString(), adm_num)) { // FIXME: StringTyper::isInt and check should be done by ScalarInt constructor so it's valid everywhere!
                    // Add matching macro to data->macro map
                    auto got = this->adm_to_data.find(adm_num);
                    if (got != adm_to_data.end()) {
                        std::string data_symbol = (*got).second;
                        data_to_adm_macro[data_symbol] = macro;
                    } else if (ext_ds_section && pk_macros_section) {
                        // FIXME: Does it really make sense to supply external dataset and pk macros section just to detect situation below? Consolidator was easier...
                        // TODO: Decide if it's a good idea to save the pk_macros_node as a PharmMLSection for pretty logging
                        logger.error("Administration " + std::to_string(adm_num) + " refered by external dataset (%a) matches no defined administration macro (%b)",
                            ext_ds_section, pk_macros_section);
                    }
                } else {
                    std::string type = macro->getType();
                    logger.error("PK macro '" + type + "' (%a) contains attribute 'adm' but value is not an integer", macro, nullptr);
                }
            }
        }

        return data_to_adm_macro;
    }
}
