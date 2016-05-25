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

        // Split column mappings into symbol and target mapping ones
        std::vector<PharmML::ColumnMapping *> column_mappings = ext_ds->getColumnMappings();
        for (PharmML::ColumnMapping *column_map : column_mappings) {
            if (column_map->getTargetMapping()) {
                this->target_column_maps.push_back(column_map);
                
                // TODO: Support multiple maps (and decide what that would mean)
                std::unordered_map<int, std::string> adm_map = column_map->getAdministrationMap();

                // Add mapped administrations for easy access
                this->mapped_adm.emplace(column_map, std::unordered_set<int>());
                for (auto adm_pair : adm_map) {
                    this->mapped_adm[column_map].insert(adm_pair.first);
                }
            } else {
                this->symbol_column_maps.push_back(column_map);
            }
        }
    }

    // Add macro for consolidation with this ExternalDataset
    void ExternalDataset::addConsolidatedPKMacro(CPharmML::PKMacro *cmacro) {
        if (cmacro->hasAttribute("adm")) { // Only consider administration macro's
            // Consider target mapping column maps for mapping this cmacro
            for (PharmML::ColumnMapping *column_map : this->target_column_maps) {
                // Add cmacro if and only if external dataset maps it
                int adm_attr;
                if (this->ast_analyzer.tryParsePureInt(cmacro->getAttribute("adm"), adm_attr)) {
                    if (mapped_adm[column_map].count(adm_attr) > 1) {
                        this->mapped_cmacros[column_map].insert(cmacro);
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
        return !this->target_column_maps.empty();
    }

    std::vector<int> ExternalDataset::getMappedAdmNumbers() {
        std::vector<int> numbers;
        for (PharmML::ColumnMapping *column_map : this->target_column_maps) {
            for (int adm : this->mapped_adm[column_map]) {
                numbers.push_back(adm);
            }
        }
        return numbers;
    }
}
