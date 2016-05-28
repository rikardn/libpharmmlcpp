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

#ifndef CPHARMML_EXTERNALDATASET_H_
#define CPHARMML_EXTERNALDATASET_H_

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>
#include <symbols/Symbol.h>
#include <visitors/AstAnalyzer.h>
#include <consolidators/PKMacros.h>

#include <PharmML/ExternalDataset.h>

namespace CPharmML
{
    class ColumnMapping
    {
        public:
            // Construct with ColumnMapping as base
            ColumnMapping(PharmML::ColumnMapping *col_map, std::shared_ptr<PharmML::Logger> logger);

            // Add objects for consolidation with ColumnMapping
            void addAdministrationMacro(int adm_num, CPharmML::PKMacro *cmacro);

            // Get attributes
            bool mapsMultiple();
            std::unordered_set<std::string> getSymbolStrings();
            std::unordered_set<int> getAdmNumbers();
            std::unordered_set<int> getDanglingAdmNumbers();

        private:
            std::shared_ptr<PharmML::Logger> logger;
            
            // Objects used to consolidate
            PharmML::ColumnMapping *col_map;

            // Mapping structures
            PharmML::Symbol *mapped_symbol = nullptr;
            std::unordered_map<std::string, std::string> data_to_symbol;
            std::unordered_map<int, std::string> adm_to_data;
            std::unordered_map<std::string, CPharmML::PKMacro *> data_to_adm_cmacro;

            // Attributes
            int num_maps = 0;
    };
    
    class ExternalDataset
    {
        public:
            // Construct with ExternalDataset as base
            ExternalDataset(PharmML::ExternalDataset *ext_ds, std::shared_ptr<PharmML::Logger> logger);
            
            // Add objects for consolidation with ExternalDataset
            void addConsolidatedPKMacro(CPharmML::PKMacro *cmacro);
            
            // Get objects used to consolidate
            PharmML::ExternalDataset *getExternalDataset();
            std::vector<CPharmML::ColumnMapping *> getColumnMappings();
            
            // Get attributes


        private:
            std::shared_ptr<PharmML::Logger> logger;
            PharmML::AstAnalyzer ast_analyzer;
            
            // Objects used to consolidate
            PharmML::ExternalDataset *ext_ds = nullptr;
            std::vector<CPharmML::ColumnMapping *> ccol_maps;
    };
}

#endif
