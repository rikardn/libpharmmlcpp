#ifndef PHARMML_EXTERNALDATASET_H_
#define PHARMML_EXTERNALDATASET_H_

#include "PharmMLContext.h"
#include "ColumnMapping.h"

namespace PharmML
{
    class ExternalDataset {
        PharmML::PharmMLContext *context;
        std::vector<PharmML::ColumnMapping *> ColumnMappings;

        public:
        ExternalDataset(PharmML::PharmMLContext *context, xml::Node node);
        std::vector<PharmML::ColumnMapping *> getColumnMappings();
        void parse(xml::Node node);
    };
}

#endif
