#include "PharmMLContext.h"
#include "ModelDefinition.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "xml.h"

namespace PharmML
{
    ModelDefinition::ModelDefinition(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void ModelDefinition::parse(xml::Node node) {
        xml::Node cov_node = this->context->getSingleElement(node, ".//mdef:CovariateModel");
        if (cov_node.exists()) {
            this->CovariateModel = new PharmML::CovariateModel(this->context, cov_node);
        }
        xml::Node struct_node = this->context->getSingleElement(node, ".//mdef:StructuralModel");
        if (struct_node.exists()) {
            this->StructuralModel = new PharmML::StructuralModel(this->context, struct_node);
        }
    }

    PharmML::CovariateModel *ModelDefinition::getCovariateModel() {
        return this->CovariateModel;
    }

    PharmML::StructuralModel *ModelDefinition::getStructuralModel() {
        return this->StructuralModel;
    }
}
