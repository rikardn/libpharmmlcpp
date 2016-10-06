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

#ifndef PHARMMLCPP_EXTERNALDATASET_H_
#define PHARMMLCPP_EXTERNALDATASET_H_

#include <PharmML/PharmMLReader.h>
#include <PharmML/PharmMLSection.h>
#include <PharmML/MultipleDVMapping.h>
#include <PharmML/ColumnMapping.h>
#include <PharmML/Dataset.h>
#include <symbols/SymbolGathering.h>
#include <symbols/MacroGathering.h>
#include <symbols/Symbol.h>

namespace pharmmlcpp
{
    // Note that type design:DatasetMappingType (used by TrialDesign->Observations->IndividualObservations)
    // and type design:ExternalDataSetType (used by TrialDesign->ExternalDataset) are extremely similar.
    // Both have unbounded ds:ColumnMappingType's and one ds:Dataset; Although ExternalDataSetType might
    // have design:CodeInjectionType instead of ds:DataSet and ds:ColumnTransformationType's/ds:MultipleDVMappingType's
    // instead of ds:ColumnMappingType's (plus an attribute, toolName). I'm unsure of the wisest path here.
    class ExternalDataset : public PharmMLSection
    {
        public:
            ExternalDataset(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            std::string getOid();
            std::vector<pharmmlcpp::ColumnMapping *> getColumnMappings();
            MultipleDVMapping *getMultipleDVMapping(std::string col_id);
            Dataset *getDataset();
            std::string getToolName();
            void setupRefererSymbRefs(SymbolGathering &gathering);
            void setupTargetMappings(SymbolGathering &gatherer);
            void setupTargetMappings(MacroGathering &gatherer);

            void accept(PharmMLVisitor *visitor);

            // POST PARSE/CONSOLIDATION


        private:
            std::string oid;
            std::vector<ColumnMapping *> col_maps;
            std::vector<std::unique_ptr<MultipleDVMapping>> multi_dv_maps;
            Dataset *dataset = nullptr;
            // Why don't we even have an accept for this?
            std::string toolName;

            // POST PARSE/CONSOLIDATION
    };
}

#endif
