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
#include <symbols/SymbolGathering.h>

namespace pharmmlcpp
{
    ObservationModel::ObservationModel(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->Block::parse(node);
        this->ObservationModel::parse(reader, node);
    }

    void ObservationModel::parse(PharmMLReader &reader, xml::Node node) {
        // Get common data (to all observation model types)
        xml::Node name_node = reader.getSingleElement(node, ".//ct:Name");
        if (name_node.exists()) {
            this->name = name_node.getText();
        }
        // TODO: Get other mdef:CommonObservationModelType stuff (don't know what most is for..)

        // Continuous or discrete model?
        xml::Node cont_node = reader.getSingleElement(node, "./mdef:ContinuousData");
        xml::Node disc_node = reader.getSingleElement(node, "./mdef:Discrete");
        if (cont_node.exists()) {
            // Continuous data model
            this->continuousData = true;

            // Get built in error models
            xml::Node stand_node = reader.getSingleElement(cont_node, "./mdef:Standard");
            xml::Node general_node = reader.getSingleElement(cont_node, "./mdef:General");
            if (stand_node.exists()) {
                // Standard error model (type 1): u(y) = u(f) + g * eps
                this->standardErrorModel = true;
                this->Symbol::parse(stand_node);

                // TODO: Support mdef:ObservationErrorType

                // Get transformation if available (u)
                xml::Node trans_node = reader.getSingleElement(stand_node, "./mdef:Transformation");
                if (trans_node.exists()) {
                    // Get transformation type (Box-Cox, identity, log, logit or probit)
                    this->transformation = trans_node.getAttribute("type").getValue();

                    // Get transformation parameters (if available)
                    std::vector<xml::Node> trans_params = reader.getElements(trans_node, "./ct:Parameter");
                    for (xml::Node trans_param : trans_params) {
                        std::shared_ptr<AstNode> param = reader.factory.create(reader, trans_param.getChild());
                        this->transformationParameters.push_back(param);
                    }
                }

                // Get output from the structural model (f)
                xml::Node output_node = reader.getSingleElement(stand_node, "./mdef:Output");
                this->output = new SymbRef(reader, output_node.getChild());

                // Get error model (g)
                xml::Node error_assign = reader.getSingleElement(stand_node, "./mdef:ErrorModel/ct:Assign");
                this->errorModel = reader.factory.create(reader, error_assign.getChild());

                // Get residual error (eps)
                xml::Node res_node = reader.getSingleElement(stand_node, "./mdef:ResidualError");
                this->residualError = new SymbRef(reader, res_node.getChild());
            } else if (general_node.exists()) {
                // General/distributional error model (type 2/3): h(y) = H(f, xi, eps) / u(y) ~ distribution(parameter1, parameter2, ...)
                this->Symbol::parse(general_node);

                // TODO: Support mdef:ObservationErrorType

                // Get assign/distribution
                xml::Node assign_node = reader.getSingleElement(general_node, "./ct:Assign");
                if (assign_node.exists()) {
                    // General error model (type 2): h(y) = H(f, xi, eps)
                    this->generalErrorModel = true;

                    // Get the general assignment (right hand side)
                    this->generalAssignment = reader.factory.create(reader, assign_node.getChild());

                    // Get LHS transformation
                    xml::Node trans_node = reader.getSingleElement(general_node, "./mdef:LHSTransformation");
                    if (trans_node.exists()) {
                        // Get transformation type (Box-Cox, identity, log, logit or probit)
                        this->transformation = trans_node.getAttribute("type").getValue();

                        // Get transformation parameters (if available)
                        std::vector<xml::Node> trans_params = reader.getElements(trans_node, "./ct:Parameter");
                        for (xml::Node trans_param : trans_params) {
                            std::shared_ptr<AstNode> param = reader.factory.create(reader, trans_param.getChild());
                            this->transformationParameters.push_back(param);
                        }
                    }
                } else {
                    // Distributional error model (type 3): u(y) ~ distribution(parameter1, parameter2, ...)
                    this->distributionalErrorModel = true;

                    // Get variablitiy references
                    std::vector<xml::Node> var_ref_nodes = reader.getElements(general_node, "./ct:VariabilityReference");
                    for (xml::Node var_ref_node : var_ref_nodes) {
                        pharmmlcpp::VariabilityReference *varRef = new VariabilityReference(reader, var_ref_node);
                        this->variabilityReferences.push_back(varRef);
                    }

                    // TODO: Support distributional
                }
            }
        } else {
            // Discrete data model
            this->continuousData = false;

            xml::Node cat_node = reader.getSingleElement(disc_node, "./mdef:CategoricalData");
            xml::Node cnt_node = reader.getSingleElement(disc_node, "./mdef:CountData");
            //TODO: xml::Node tte_node = this->context->getSingleElement(disc_node, "./mdef:TimeToEventData");
            if (cat_node.exists()) {
                // Categorical data model
                this->categoricalData = true;

                // TODO: Support categorical data models
            } else if (cnt_node.exists()) {
                // Count data model
                this->countData = true;

                // TODO: Support count data models
            } else {
                // Time-to-event data model
                this->tteData = true;

                // TODO: Support TTE data models
            }
        }
    }

    bool ObservationModel::isContinuous() {
        return this->continuousData;
    }

    bool ObservationModel::isCategorical() {
        return this->categoricalData;
    }

    bool ObservationModel::isCount() {
        return this->countData;
    }

    bool ObservationModel::isTTE() {
        return this->tteData;
    }

    bool ObservationModel::hasStandardErrorModel() {
        return this->standardErrorModel;
    }

    bool ObservationModel::hasGeneralErrorModel() {
        return this->generalErrorModel;
    }

    bool ObservationModel::hasDistributionalErrorModel() {
        return this->distributionalErrorModel;
    }

    std::string ObservationModel::getTransformation() {
        return this->transformation;
    }

    SymbRef *ObservationModel::getOutput() {
        return this->output;
    }

    std::shared_ptr<AstNode> ObservationModel::getErrorModel() {
        return this->errorModel;
    }

    SymbRef *ObservationModel::getResidualError() {
        return this->residualError;
    }

    std::shared_ptr<AstNode> ObservationModel::getAssignment() {
        return this->generalAssignment;
    }

    std::vector<pharmmlcpp::VariabilityReference *> ObservationModel::getVariabilityReferences() {
        return this->variabilityReferences;
    }

    SymbolSet ObservationModel::getNeededSymbols() {
        SymbolSet y_refs = this->referencedSymbols.getDependencies();
        y_refs.merge(this->referencedSymbols);
        return y_refs;
    }

    void ObservationModel::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        // FIXME: Fill this
        if (this->standardErrorModel) {
            this->addSymbRef(this->output, gathering, blkId);
            this->setupAstSymbRefs(this->errorModel.get(), gathering, blkId);
            this->setupAstSymbRefs(this->residualError, gathering, blkId);
        }
    }

    void ObservationModel::gatherSymbols(SymbolGathering &gathering) {
        gathering.newBlock(this);
        gathering.addSymbol(this);
    }

    void ObservationModel::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }

    void ObservationModel::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
