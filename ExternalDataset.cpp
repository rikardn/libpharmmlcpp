#include "ExternalDataset.h"

namespace PharmML
{
    ExternalDataset::ExternalDataset(PharmML::PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void ExternalDataset::parse(xml::Node node) {
        
    }

}
