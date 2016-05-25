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
#include <visitors/AstAnalyzer.h>
#include <consolidators/PKMacros.h>

#include <PharmML/ExternalDataset.h>

namespace CPharmML
{
    class ExternalDataset
    {
        public:
            // Construct with ExternalDataset as base
            ExternalDataset(PharmML::ExternalDataset *ext_ds);
            
            // Add objects for consolidation with ExternalDataset
            void addConsolidatedPKMacro(CPharmML::PKMacro *cmacro);
            
            // Get objects used to consolidate
            PharmML::ExternalDataset *getExternalDataset();
            std::vector<CPharmML::PKMacro *> getPKMacros();
            
            // Get attributes
            bool hasTargetMappings();
            std::vector<int> getMappedAdmNumbers();

        private:
            PharmML::AstAnalyzer ast_analyzer;
            // Objects used to consolidate
            PharmML::ExternalDataset *ext_ds = nullptr;
            std::vector<CPharmML::PKMacro *> macros;

            std::vector<PharmML::ColumnMapping *> target_column_maps;
            std::vector<PharmML::ColumnMapping *> symbol_column_maps;
            std::unordered_map<PharmML::ColumnMapping *, std::unordered_set<int>> mapped_adm;
            std::unordered_map<PharmML::ColumnMapping *, std::unordered_set<CPharmML::PKMacro *>> mapped_cmacros;
    };
}

#endif
