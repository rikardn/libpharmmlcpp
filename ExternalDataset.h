#ifndef PHARMML_EXTERNALDATASET_H_
#define PHARMML_EXTERNALDATASET_H_

#include "PharmMLContext.h"
#include "ColumnMapping.h"

namespace PharmML
{
    // Note that type design:DatasetMappingType (used by TrialDesign->Observations->IndividualObservations)
    // and type design:ExternalDataSetType (used by TrialDesign->ExternalDataset) are extremely similar.
    // Both have unbounded ds:ColumnMappingType's and one ds:Dataset; Although ExternalDataSetType might
    // have design:CodeInjectionType instead of ds:DataSet and ds:ColumnTransformationType's/ds:MultipleDVMappingType's
    // instead of ds:ColumnMappingType's (plus an attribute, toolName). I'm unsure of the wisest path here.
    class ExternalDataset {
        PharmML::PharmMLContext *context;
        std::string oid;
        std::vector<PharmML::ColumnMapping *> ColumnMappings;
        // TODO: Support ds:Dataset (data for each subject within the study)

        public:
        ExternalDataset(PharmML::PharmMLContext *context, xml::Node node);
        std::string getOid();
        std::vector<PharmML::ColumnMapping *> getColumnMappings();
        void parse(xml::Node node);
    };
}

#endif
