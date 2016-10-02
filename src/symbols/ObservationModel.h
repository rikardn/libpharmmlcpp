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

#ifndef PHARMMLCPP_OBSERVATIONMODEL_H_
#define PHARMMLCPP_OBSERVATIONMODEL_H_

#include <PharmML/PharmMLContext.h>
#include <AST/AstNode.h>
#include <xml/xml.h>
#include <visitors/PharmMLVisitor.h>
#include <visitors/SymbolVisitor.h>
#include <PharmML/Distribution.h>
#include <symbols/VariabilityLevel.h>
#include <symbols/Category.h>
#include <symbols/DiscreteVariable.h>
#include <PharmML/PharmMLSection.h>
#include <PharmML/Block.h>

namespace pharmmlcpp
{
    class ObservationModel : public Block, public Symbol, public PharmMLSection
    {
        public:
            ObservationModel(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);

            // Type of observation data
            bool isContinuous(); // Standard, general och distributional
            bool isCategorical();
            bool isCount();
            bool isTTE();

            // Type of observation model
            bool hasStandardErrorModel(); // u(y) = u(f) + g * eps
            bool hasGeneralErrorModel(); // h(y) = H(f, xi, eps)
            bool hasDistributionalErrorModel(); // u(y) ~ distribution(parameter1, parameter2, ...)

            // Transformations (LHS or LHS & RHS)
            std::string getTransformation();
            // getTransformationParameters()

            // Standard error model
            SymbRef *getOutput();
            std::shared_ptr<AstNode> getErrorModel();
            SymbRef *getResidualError();

            // General error model
            std::shared_ptr<AstNode> getAssignment();

            // Distributional error model
            std::vector<VariabilityReference *> getVariabilityReferences();

            // Count data accessors
            std::shared_ptr<DiscreteVariable> getCountVariable() { return this->count_variable; };
            std::string getCountPMFLinkFunction() { return this->count_pmf_transform; };
            Distribution *getCountPMFDistribution() { return this->count_pmf_distribution.get(); };

            // Categorical data accessors
            bool isOrderedCategorical() { return this->categorical_ordered; };
            std::vector<std::shared_ptr<Category>> getCategoricalCategories() { return this->categorical_categories; };
            std::shared_ptr<DiscreteVariable> getCategoricalVariable() { return this->categorical_variable; };
            std::string getCategoricalPMFLinkFunction() { return this->categorical_pmf_transform; };
            Distribution *getCategoricalPMFDistribution() { return this->categorical_pmf_distribution.get(); };

            // TTE data accessors
            std::shared_ptr<DiscreteVariable> getTTEVariable() { return this->tte_variable; };
            bool hasTTEHazardFunction() { return this->tte_haz_symbid != ""; }; // otherwise likely survival function parameterization
            std::string getTTEHazardFunctionSymbId() { return this->tte_haz_symbid; };
            std::shared_ptr<Distribution> getTTEHazardFunctionDistribution() { return this->tte_haz_distribution; };
            std::shared_ptr<AstNode> getTTEHazardFunctionAssignment() { return this->tte_haz_assignment; };

            // Convenience methods
            SymbolSet getNeededSymbols();

            void setupSymbRefs(SymbolGathering &gathering, std::string blkId);
            void gatherSymbols(SymbolGathering &gathering) override;
            void accept(PharmMLVisitor *visitor);
            void accept(SymbolVisitor *visitor);

        private:
            // Common to all observation models
            std::string name;

            // Type of observation data
            bool continuousData = false;
            bool categoricalData = false;
            bool countData = false;
            bool tteData = false;

            // Type of observation model
            bool standardErrorModel = false;
            bool generalErrorModel = false;
            bool distributionalErrorModel = false;

            // Transformations (on LHS or both)
            std::string transformation;
            std::vector<std::shared_ptr<AstNode>> transformationParameters;

            // Standard error model
            SymbRef *output = nullptr;
            std::shared_ptr<AstNode> errorModel;
            SymbRef *residualError = nullptr;

            // General error model
            std::shared_ptr<AstNode> generalAssignment;

            // Distributional error model
            std::vector<VariabilityReference *> variabilityReferences;

            // Count data
            std::shared_ptr<DiscreteVariable> count_variable;
            std::string count_pmf_transform; // link functions: identity, log, logit, probit, loglog or comploglog
            std::unique_ptr<Distribution> count_pmf_distribution;

            // Categorical data
            bool categorical_ordered;
            std::vector<std::shared_ptr<Category>> categorical_categories;
            std::shared_ptr<DiscreteVariable> categorical_variable;
            std::string categorical_pmf_transform; // link functions: identity, log, logit, probit, loglog or comploglog
            std::unique_ptr<Distribution> categorical_pmf_distribution;

            // TTE data
            std::shared_ptr<DiscreteVariable> tte_variable;
            std::string tte_haz_symbid;
            std::shared_ptr<Distribution> tte_haz_distribution;
            std::shared_ptr<AstNode> tte_haz_assignment;
    };
}

#endif
