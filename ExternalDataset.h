#ifndef PHARMML_EXTERNALDATASET_H_
#define PHARMML_EXTERNALDATASET_H_

#include "PharmMLContext.h"

namespace PharmML
{
    class ExternalDataset {
        PharmML::PharmMLContext *context;

        public:
        ExternalDataset(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
    };
}

#endif
