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

#ifndef PHARMML_EXTERNALDATASET_H_
#define PHARMML_EXTERNALDATASET_H_

#include "PharmMLContext.h"
#include <PharmML/PharmMLSection.h>
#include <PharmML/ColumnMapping.h>
#include <symbols/Symbol.h>

namespace PharmML
{
    // Note that type design:DatasetMappingType (used by TrialDesign->Observations->IndividualObservations)
    // and type design:ExternalDataSetType (used by TrialDesign->ExternalDataset) are extremely similar.
    // Both have unbounded ds:ColumnMappingType's and one ds:Dataset; Although ExternalDataSetType might
    // have design:CodeInjectionType instead of ds:DataSet and ds:ColumnTransformationType's/ds:MultipleDVMappingType's
    // instead of ds:ColumnMappingType's (plus an attribute, toolName). I'm unsure of the wisest path here.
    class ExternalDataset : public PharmMLSection
    {
        public:
            ExternalDataset(PharmML::PharmMLContext *context, xml::Node node);
            std::string getOid();
            std::vector<PharmML::ColumnMapping *> getColumnMappings();
            Dataset *getDataset();
            std::string getToolName();
            void parse(xml::Node node);
            void gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap);
            void accept(PharmMLVisitor *visitor);
        
        private:
            PharmML::PharmMLContext *context;
            std::string oid;
            std::vector<PharmML::ColumnMapping *> ColumnMappings;
            Dataset *dataset;
            // Why don't we even have an accept for this?
            std::string toolName;
    };
}

#endif
