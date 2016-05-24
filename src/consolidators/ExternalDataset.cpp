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
    // Construct with ExternalDataset as base
    ExternalDataset::ExternalDataset(PharmML::ExternalDataset *ext_ds) {
        this->ext_ds = ext_ds;

        // Scan column mappings after macro mappings
        std::vector<PharmML::ColumnMapping *> column_mappings = ext_ds->getColumnMappings();
        for (PharmML::ColumnMapping *column_map : column_mappings) {
            if (column_map->hasTargetMappings()) {
                this->target_mapping_column_maps.push_back(column_map);
            } else {
                this->symbol_mapping_column_maps.push_back(column_map);
            }
        }
    }

    // Add objects for consolidation with ExternalDataset
    void ExternalDataset::addConsolidatedPKMacro(CPharmML::PKMacro *cmacro) {
        for (PharmML::ColumnMapping *column_map : this->target_mapping_column_maps) {
            for (PharmML::TargetMapping *target_map : column_map->getTargetMappings()) {
                std::vector<PharmML::MapType> maps = target_map->getMaps();
                for (PharmML::MapType map : maps) {
                    if (cmacro->hasAttribute("adm") && map.admNumber != "") {
                        int map_adm, adm_attr;
                        this->ast_analyzer.tryParseInt(map.admNumber, map_adm);
                        this->ast_analyzer.tryParsePureInt(cmacro->getAttribute("adm"), adm_attr);
                        if (map_adm == adm_attr) {
                            this->column_map_cmacros[column_map].insert(cmacro);
                        }
                    }
                }
            }
        }
    }

    // Get objects used to consolidate
    PharmML::ExternalDataset *ExternalDataset::getExternalDataset() {
        return this->ext_ds;
    }
    
    std::vector<CPharmML::PKMacro *> ExternalDataset::getPKMacros() {
        return this->macros;
    }

    // Get attributes
    bool ExternalDataset::hasTargetMappings() {
        return !this->target_mapping_column_maps.empty();
    }

    std::vector<int> ExternalDataset::getMappedAdmNumbers() {
        std::vector<int> result;
        for (PharmML::ColumnMapping *column_mapping : this->target_mapping_column_maps) {
            std::vector<PharmML::TargetMapping *> target_mappings = column_mapping->getTargetMappings();
            for (PharmML::TargetMapping *target_mapping : target_mappings) {
                // TODO: Be careful with blkIdRef here (multiple structural models -- a possibility!)
                std::vector<PharmML::MapType> maps = target_mapping->getMaps();
                for (PharmML::MapType map : maps) {
                    if (map.admNumber != "") {
                        // TODO: What if someone put something which isn't an integer herein?
                        result.push_back(std::stoi(map.admNumber));
                    }
                }
            }
        }
        return result;
    }
}
