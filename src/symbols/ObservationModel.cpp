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
        this->setXMLNode(node);
        this->context = context;
        this->ObservationModel::parse(node);
    }

    void ObservationModel::parse(xml::Node node) {
        // Get common data (to all observation model types)
        this->blkId = node.getAttribute("blkId").getValue();
        xml::Node name_node = this->context->getSingleElement(node, ".//ct:Name");
        if (name_node.exists()) {
            this->name = name_node.getText();
        }
        // TODO: Get other mdef:CommonObservationModelType stuff (don't know what most is for..)

        // Continuous or discrete model?
        xml::Node cont_node = this->context->getSingleElement(node, "./mdef:ContinuousData");
        xml::Node disc_node = this->context->getSingleElement(node, "./mdef:Discrete");
        if (cont_node.exists()) {
            // Continuous data model
            this->continuousData = true;

            // Get built in error models
            xml::Node stand_node = this->context->getSingleElement(cont_node, "./mdef:Standard");
            xml::Node general_node = this->context->getSingleElement(cont_node, "./mdef:General");
            if (stand_node.exists()) {
                // Standard error model (type 1): u(y) = u(f) + g * eps
                this->standardErrorModel = true;
                this->Symbol::parse(stand_node);

                // TODO: Support mdef:ObservationErrorType

                // Get transformation if available (u)
                xml::Node trans_node = this->context->getSingleElement(stand_node, "./mdef:Transformation");
                if (trans_node.exists()) {
                    // Get transformation type (Box-Cox, identity, log, logit or probit)
                    this->transformation = trans_node.getAttribute("type").getValue();

                    // Get transformation parameters (if available)
                    std::vector<xml::Node> trans_params = this->context->getElements(trans_node, "./ct:Parameter");
                    for (xml::Node trans_param : trans_params) {
                        PharmML::AstNode *param = this->context->factory.create(trans_param.getChild());
                        this->transformationParameters.push_back(param);
                    }
                }

                // Get output from the structural model (f)
                xml::Node output_node = this->context->getSingleElement(stand_node, "./mdef:Output");
                this->output = new SymbRef(output_node.getChild());

                // Get error model (g)
                xml::Node error_assign = this->context->getSingleElement(stand_node, "./mdef:ErrorModel/ct:Assign");
                this->errorModel = this->context->factory.create(error_assign.getChild());

                // Get residual error (eps)
                xml::Node res_node = this->context->getSingleElement(stand_node, "./mdef:ResidualError");
                this->residualError = new SymbRef(res_node.getChild());
            } else if (general_node.exists()) {
                // General/distributional error model (type 2/3): h(y) = H(f, xi, eps) / u(y) ~ distribution(parameter1, parameter2, ...)
                this->Symbol::parse(general_node);

                // TODO: Support mdef:ObservationErrorType

                // Get assign/distribution
                xml::Node assign_node = this->context->getSingleElement(general_node, "./ct:Assign");
                if (assign_node.exists()) {
                    // General error model (type 2): h(y) = H(f, xi, eps)
                    this->generalErrorModel = true;

                    // Get the general assignment (right hand side)
                    this->generalAssignment = this->context->factory.create(assign_node.getChild());

                    // Get LHS transformation
                    xml::Node trans_node = this->context->getSingleElement(general_node, "./mdef:LHSTransformation");
                    if (trans_node.exists()) {
                        // Get transformation type (Box-Cox, identity, log, logit or probit)
                        this->transformation = trans_node.getAttribute("type").getValue();

                        // Get transformation parameters (if available)
                        std::vector<xml::Node> trans_params = this->context->getElements(trans_node, "./ct:Parameter");
                        for (xml::Node trans_param : trans_params) {
                            PharmML::AstNode *param = this->context->factory.create(trans_param.getChild());
                            this->transformationParameters.push_back(param);
                        }
                    }
                } else {
                    // Distributional error model (type 3): u(y) ~ distribution(parameter1, parameter2, ...)
                    this->distributionalErrorModel = true;

                    // Get variablitiy references
                    std::vector<xml::Node> var_ref_nodes = this->context->getElements(general_node, "./ct:VariabilityReference");
                    for (xml::Node var_ref_node : var_ref_nodes) {
                        PharmML::VariabilityReference *varRef = new VariabilityReference(this->context, var_ref_node);
                        this->variabilityReferences.push_back(varRef);
                    }

                    // TODO: Support distributional
                }
            }
        } else {
            // Discrete data model
            this->continuousData = false;

            xml::Node cat_node = this->context->getSingleElement(disc_node, "./mdef:CategoricalData");
            xml::Node cnt_node = this->context->getSingleElement(disc_node, "./mdef:CountData");
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

    AstNode *ObservationModel::getErrorModel() {
        return this->errorModel;
    }

    SymbRef *ObservationModel::getResidualError() {
        return this->residualError;
    }

    AstNode *ObservationModel::getAssignment() {
        return this->generalAssignment;
    }

    std::vector<PharmML::VariabilityReference *> ObservationModel::getVariabilityReferences() {
        return this->variabilityReferences;
    }

    SymbolSet ObservationModel::getNeededSymbols() {
        SymbolSet y_refs = this->referencedSymbols.getDependencies();
        y_refs.merge(this->referencedSymbols);
        return y_refs;
    }

    void ObservationModel::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        // FIXME: Fill this!
        if (this->standardErrorModel) {
            std::unordered_set<PharmML::Symbol *> found_symbols;
            // Output
            found_symbols.insert(this->addSymbRef(this->output, symbolMap));
            // Error model and residual error
            std::unordered_set<PharmML::Symbol *> error_symbols = this->setupAstSymbRefs(this->errorModel, symbolMap);
            std::unordered_set<PharmML::Symbol *> res_symbols = this->setupAstSymbRefs(this->residualError, symbolMap);
            found_symbols.insert(error_symbols.begin(), error_symbols.end());
            found_symbols.insert(res_symbols.begin(), res_symbols.end());

            this->addReferences(found_symbols);
        }
    }

    void ObservationModel::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }

    void ObservationModel::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
