#include "ObservationModel.h"

namespace PharmML
{
    ObservationModel::ObservationModel(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void ObservationModel::parse(xml::Node node) {
        xml::Node error_model_tree = this->context->getSingleElement(node, ".//mdef:ContinuousData/mdef:Standard/mdef:ErrorModel/ct:Assign");
        if (error_model_tree.exists()) {
            this->ErrorModel = this->context->factory.create(error_model_tree.getChild());
        }
    } 

    AstNode *ObservationModel::getErrorModel() {
        return this->ErrorModel;
    }
}
