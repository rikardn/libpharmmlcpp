#include "ObservationModel.h"

namespace PharmML
{
    ObservationModel::ObservationModel(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void ObservationModel::parse(xml::Node node) {
        xml::Node standard = this->context->getSingleElement(node, ".//mdef:ContinuousData/mdef:Standard");
        if (standard.exists()) {
            this->symbId = standard.getAttribute("symbId").getValue();
        }
        xml::Node output = this->context->getSingleElement(node, ".//mdef:ContinuousData/mdef:Standard/mdef:Output");
        if (output.exists()) {
            this->Output = this->context->factory.create(output.getChild());
        }
        xml::Node error_model_tree = this->context->getSingleElement(node, ".//mdef:ContinuousData/mdef:Standard/mdef:ErrorModel/ct:Assign");
        if (error_model_tree.exists()) {
            this->ErrorModel = this->context->factory.create(error_model_tree.getChild());
        }
        xml::Node residual_error = this->context->getSingleElement(node, ".//mdef:ContinuousData/mdef:Standard/mdef:ResidualError");
        if (residual_error.exists()) {
            this->ResidualError = this->context->factory.create(residual_error.getChild());
        }
    } 
   
    std::string ObservationModel::getSymbId() {
        return this->symbId;
    }

    AstNode *ObservationModel::getOutput() {
        return this->Output;
    }

    AstNode *ObservationModel::getErrorModel() {
        return this->ErrorModel;
    }
    
    AstNode *ObservationModel::getResidualError() {
        return this->ResidualError;
    }

    std::string ObservationModel::accept(AbstractVisitor *visitor) {
        return visitor->visit(this);
    }
}
