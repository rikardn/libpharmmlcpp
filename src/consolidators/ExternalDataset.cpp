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

#include <consolidators/ExternalDataset.h>

namespace CPharmML
{
    // Construct with ColumnMapping as base
    ColumnMapping::ColumnMapping(PharmML::ColumnMapping *col_map, std::shared_ptr<PharmML::Logger> logger) {
        this->logger = logger;
        this->col_map = col_map;

        // Deal with mapped symbols
        PharmML::Symbol *mapped_symbol = col_map->getMappedSymbol();
        if (mapped_symbol) {
            this->mapped_symbol = mapped_symbol;
            this->num_maps++;
        }

        // TODO: Deal with categorical covariate mapping

        // Deal with target maps
        PharmML::TargetMapping *target_map = col_map->getTargetMapping();
        if (target_map) {
            // Create associative arrays of (data) symbols to model (symbols and administrations)
            for (PharmML::MapType map : target_map->getMaps()) {
                if (map.modelSymbol != "") {
                    this->symbol_to_data[map.modelSymbol] = map.dataSymbol;
                    if (map.modelSymbol_ptr) {
                        this->data_to_symbol_ptr[map.dataSymbol] = map.modelSymbol_ptr;
                    } else {
                        this->logger->error("TargetMapping element contains non-resolvable 'modelSymbol': " + map.modelSymbol, target_map);
                    }
                    this->num_maps++;
                } else if (map.admNumber != "") {
                    int adm;
                    if (PharmML::AstAnalyzer::tryParseInt(map.admNumber, adm)) {
                        this->adm_to_data[adm] = map.dataSymbol;
                    } else {
                        this->logger->error("TargetMapping element contains non-integer 'admNumber': " + map.admNumber, target_map);
                    }
                    this->num_maps++;
                } else {
                    this->logger->error("TargetMapping element doesn't map symbol nor administration macro", target_map);
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

    // Get the (resolved) complete (TargetMapping) map from data symbol strings to the PharmML::Symbol pointers
    std::unordered_map<std::string, PharmML::Symbol *> ColumnMapping::getDataSymbolMap() {
        return data_to_symbol_ptr;
    }

    // Get the set of all (TargetMapping) administration numbers contained within this column mapping
    std::unordered_set<int> ColumnMapping::getAdmNumbers() {
        std::unordered_set<int> adm_nums;
        for (std::pair<int, std::string> pair : this->adm_to_data) {
            adm_nums.insert(pair.first);
        }
        return adm_nums;
    }

    // Get the (resolved) complete (TargetMapping) map from data symbol strings to the CPharmML::PKMacro pointers
    std::unordered_map<std::string, CPharmML::PKMacro *> ColumnMapping::getDataAdministrationMap() {
        return data_to_adm_cmacro;
    }

    // Add an administration macro which this column mapping refers to (TargetMapping via admNumber)
    void ColumnMapping::addAdministrationMacro(int adm_num, CPharmML::PKMacro *cmacro) {
        std::string data_symbol = this->adm_to_data[adm_num];
        this->data_to_adm_cmacro[data_symbol] = cmacro;
    }

    // Get the administration numbers which haven't been linked to an administration macro
    std::unordered_set<int> ColumnMapping::getDanglingAdmNumbers() {
        std::unordered_set<int> adm_nums;
        for (std::pair<int, std::string> pair : this->adm_to_data) {
            auto got = this->data_to_adm_cmacro.find(pair.second);
            if (got == this->data_to_adm_cmacro.end()) {
                adm_nums.insert(pair.first);
            }
        }
        return adm_nums;
    }

    // Construct with ExternalDataset as base
    ExternalDataset::ExternalDataset(PharmML::ExternalDataset *ext_ds, std::shared_ptr<PharmML::Logger> logger) {
        this->logger = logger;
        this->ext_ds = ext_ds;

        // Create consolidated column mappings
        std::vector<PharmML::ColumnMapping *> col_maps = ext_ds->getColumnMappings();
        for (PharmML::ColumnMapping *col_map : col_maps) {
            ColumnMapping *ccol_map = new ColumnMapping(col_map, this->logger);
            this->ccol_maps.push_back(ccol_map);
        }
    }

    // Add macro for consolidation with this ExternalDataset
    void ExternalDataset::addConsolidatedPKMacro(CPharmML::PKMacro *cmacro) {
        // Only consider administration macros as potential targets
        if (cmacro->hasAttribute("adm")) {
            int adm_attr;
            if (this->ast_analyzer.tryParsePureInt(cmacro->getAttribute("adm"), adm_attr)) {
                int mapping_ccol_maps = 0;
                for (ColumnMapping *ccol_map : this->ccol_maps) {
                    // Add cmacro to target mapping if it maps
                    std::unordered_set<int> adm_nums = ccol_map->getAdmNumbers();
                    if (adm_nums.count(adm_attr) > 0) {
                        ccol_map->addAdministrationMacro(adm_attr, cmacro);
                        mapping_ccol_maps++;
                    }
                }
                if (mapping_ccol_maps == 0) {
                    PharmML::PKMacro *macro = cmacro->getMacro();
                    std::string name = macro->getName();
                    this->logger->warning("PK macro '" + name + "' (%a) is not mapped by any column in external dataset (%b)", macro, this->ext_ds);
                }
            } else {
                PharmML::PKMacro *macro = cmacro->getMacro();
                std::string name = macro->getName();
                this->logger->error("PK macro '" + name + "' (%a) contains attribute 'adm' but value is not an integer", macro, nullptr);
            }
        }
    }

    // Get objects used to consolidate
    PharmML::ExternalDataset *ExternalDataset::getExternalDataset() {
        return this->ext_ds;
    }

    std::vector<CPharmML::ColumnMapping *> ExternalDataset::getColumnMappings() {
        return this->ccol_maps;
    }

    // Get attributes
}
