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

#include "ObservationModel.h"

namespace PharmML
{
    ObservationModel::ObservationModel(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void ObservationModel::parse(xml::Node node) {
        this->blkId = node.getAttribute("blkId").getValue();
        xml::Node standard = this->context->getSingleElement(node, ".//mdef:ContinuousData/mdef:Standard");
        if (standard.exists()) {
            this->symbId = standard.getAttribute("symbId").getValue();
        }
        xml::Node output = this->context->getSingleElement(node, ".//mdef:ContinuousData/mdef:Standard/mdef:Output");
        if (output.exists()) {
            this->Output = new SymbRef(output.getChild());
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

    SymbRef *ObservationModel::getOutput() {
        return this->Output;
    }

    AstNode *ObservationModel::getErrorModel() {
        return this->ErrorModel;
    }
    
    AstNode *ObservationModel::getResidualError() {
        return this->ResidualError;
    }

    void ObservationModel::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
