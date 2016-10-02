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
            xml::Node tte_node = reader.getSingleElement(disc_node, "./mdef:TimeToEventData");
            if (cat_node.exists()) {
                // Categorical data model
                this->categoricalData = true;

                // TODO: Support categorical data models
                // Ordered categories or not?
                xml::Attribute ordered_attr = cat_node.getAttribute("ordered");
                if (ordered_attr.exists()) {
                    this->categorical_ordered = ordered_attr.getValue() == "yes" ? true : false;
                } else {
                    // FIXME: Enable logging here. It seems reasonable that lack of the attribute would mean that categories are NOT ordered, but it should be reported.
                    // this->logger.warning("Categorical observation model does not contain attribute 'ordered'; Categories assumed to NOT be ordered", cat_node);
                    this->categorical_ordered = false;
                }

                // Get all categories
                xml::Node list_of_cats_node = reader.getSingleElement(cat_node, "./mdef:ListOfCategories");
                for (xml::Node category_node : list_of_cats_node.getChildren()) {
                    std::shared_ptr<Category> category = std::make_shared<Category>(reader, category_node);
                    this->categorical_categories.push_back(category);
                }

                // Get the category variable
                xml::Node categorical_var_node = reader.getSingleElement(cat_node, "./mdef:CategoryVariable");
                this->categorical_variable = categorical_var_node.getAttribute("symbId").getValue();

                // Get the distribution
                xml::Node pmf_node = reader.getSingleElement(cat_node, "./mdef:PMF");
                // TODO: Schema allows unbounded occurences of probability mass functions for count observations (but what that would mean?)
                if (pmf_node.exists()) {
                    xml::Attribute trans = pmf_node.getAttribute("transform");
                    if (trans.exists()) {
                        this->categorical_pmf_transform = trans.getValue();
                    } else {
                        this->categorical_pmf_transform = "identity"; // assumption
                    }

                    xml::Node dist_node = reader.getSingleElement(pmf_node, "./mdef:Distribution");
                    std::unique_ptr<Distribution> dist = std::make_unique<Distribution>(reader, dist_node.getChild());
                    this->categorical_pmf_distribution = std::move(dist);
                }
            } else if (cnt_node.exists()) {
                // Count data model
                this->countData = true;

                // Get the count variable
                xml::Node count_var_node = reader.getSingleElement(cnt_node, "./mdef:CountVariable");
                this->count_variable = count_var_node.getAttribute("symbId").getValue();

                // Get the distribution
                xml::Node pmf_node = reader.getSingleElement(cnt_node, "./mdef:PMF");
                // TODO: Schema allows unbounded occurences of probability mass functions for count observations (but what that would mean?)
                if (pmf_node.exists()) {
                    xml::Attribute trans = pmf_node.getAttribute("transform");
                    if (trans.exists()) {
                        this->count_pmf_transform = trans.getValue();
                    } else {
                        this->count_pmf_transform = "identity"; // assumption
                    }

                    // FIXME: PMF for count data can be e.g. assignment instead of distribution (just like hazard/survival function for TTE)!
                    xml::Node dist_node = reader.getSingleElement(pmf_node, "./mdef:Distribution");
                    std::unique_ptr<Distribution> dist = std::make_unique<Distribution>(reader, dist_node.getChild());
                    this->count_pmf_distribution = std::move(dist);
                }
            } else if (tte_node.exists()) {
                // Time-to-event data model
                this->tteData = true;

                // Get the event variable
                xml::Node tte_var_node = reader.getSingleElement(tte_node, "./mdef:EventVariable");
                this->tte_variable = std::make_shared<DiscreteVariable>(reader, tte_var_node);

                // TODO: With continuous data the entire observation model acts as one symbol. With count, categorical and tte
                //       the situation is more convoluted. The symbId is carried by the DiscreteVariable object above which is
                //       its own Symbol derived class now. Until further notice, the following re-parse hack resolves the issue:
                this->Symbol::parse(tte_var_node);

                // Get the hazard function
                xml::Node haz_node = reader.getSingleElement(tte_node, "./mdef:HazardFunction");
                // TODO: Schema allows unbounded occurences of hazard functions for tte observations (but what that would mean?)
                if (haz_node.exists()) {
                    // Hazard (or survival) function acts like a symbol but doesn't seem to be generally refered
                    // TODO: Create Symbol-derived class if a referal is seen in some model
                    this->tte_haz_symbid = haz_node.getAttribute("symbId").getValue();

                    xml::Node dist_node = reader.getSingleElement(haz_node, "./mdef:Distribution");
                    xml::Node assign_node = reader.getSingleElement(haz_node, "./ct:Assign");
                    if (dist_node.exists()) {
                        std::shared_ptr<Distribution> dist = std::make_unique<Distribution>(reader, dist_node.getChild());
                        this->tte_haz_distribution = std::move(dist);
                    } else {
                        this->tte_haz_assignment = reader.factory.create(reader, assign_node.getChild());
                    }
                }
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
        if (this->isCount()) {
            for (DistributionParameter *par : this->count_pmf_distribution->getDistributionParameters()) {
                this->setupAstSymbRefs(par->getAssignment().get(), gathering, blkId);
            }
        } else if (this->isCategorical()) {
            for (DistributionParameter *par : this->categorical_pmf_distribution->getDistributionParameters()) {
                this->setupAstSymbRefs(par->getAssignment().get(), gathering, blkId);
            }
        } else if (this->isTTE()) {
            if (this->tte_haz_assignment) {
                this->setupAstSymbRefs(this->tte_haz_assignment.get(), gathering, blkId);
            } else if (this->tte_haz_distribution) {
                for (DistributionParameter *par : this->tte_haz_distribution->getDistributionParameters()) {
                    this->setupAstSymbRefs(par->getAssignment().get(), gathering, blkId);
                }
            }
        } else if (this->standardErrorModel) {
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
