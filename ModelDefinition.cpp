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
        xml::Node param_node = this->context->getSingleElement(node, ".//mdef:ParameterModel");
        if (param_node.exists()) {
            this->ParameterModel = new PharmML::ParameterModel(this->context, param_node);
        }
        xml::Node obs_node = this->context->getSingleElement(node, ".//mdef:ObservationModel");
        if (obs_node.exists()) {
            this->ObservationModel = new PharmML::ObservationModel(this->context, obs_node);
        }
    }

    PharmML::CovariateModel *ModelDefinition::getCovariateModel() {
        return this->CovariateModel;
    }

    PharmML::ParameterModel *ModelDefinition::getParameterModel() {
        return this->ParameterModel;
    }

    PharmML::StructuralModel *ModelDefinition::getStructuralModel() {
        return this->StructuralModel;
    }
        
    PharmML::ObservationModel *ModelDefinition::getObservationModel() {
        return this->ObservationModel;
    }
}
