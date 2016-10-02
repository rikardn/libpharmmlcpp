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

#include <symbols/ObservationModel.h>
#include <symbols/PopulationParameter.h>
#include <symbols/IndividualParameter.h>
#include <symbols/RandomVariable.h>
#include <symbols/IndependentVariable.h>
#include <symbols/Variable.h>
#include <symbols/DerivativeVariable.h>
#include <symbols/Covariate.h>
#include <symbols/Category.h>
#include <symbols/FunctionDefinition.h>
#include <generators/R/RSymbols.h>
#include <iostream>

namespace pharmmlcpp
{
    RSymbols::RSymbols() {
        // Create a default R AST generator
        this->astgen = new RAstGenerator();
    }

    RSymbols::RSymbols(RAstGenerator *astgen) {
        this->astgen = astgen;
    }

    RSymbols::~RSymbols() {
        // FIXME: Find out how to delete delete this->astgen;
    }

    void RSymbols::visit(ObservationModel *node) {
        this->setValue("");
    }

    void RSymbols::visit(Parameter *node) {
        this->setValue("");
    }

    void RSymbols::visit(PopulationParameter *node) {
        this->setValue("");
    }

    void RSymbols::visit(IndividualParameter *node) {
        this->astgen->acceptRoot(node->asExplicit());
        this->setValue(node->getSymbId() + " <- " + this->astgen->getValue());
    }

    void RSymbols::visit(RandomVariable *node) {
        this->setValue("");
    }

    void RSymbols::visit(VariabilityLevel *node) {
        this->setValue("");
    }

    void RSymbols::visit(IndependentVariable *node) {
        this->setValue("");
    }

    void RSymbols::visit(Variable *node) {
        if (node->getAssignment()) {
            this->astgen->acceptRoot(node->getAssignment().get());
            this->setValue(node->getSymbId() + " <- " + this->astgen->getValue());
        } else {
            this->setValue("");
        }
    }

    void RSymbols::visit(DesignParameter *node) {
        this->visit(static_cast<Variable *>(node));
    }

    void RSymbols::visit(DerivativeVariable *node) {
        this->astgen->acceptRoot(node->getAssignment().get());
        this->setValue("d" + node->getSymbId() + " <- " + this->astgen->getValue());
    }

    void RSymbols::visit(Covariate *node) {
        auto assignment = node->getAssignment();
        if (assignment) {
            this->astgen->acceptRoot(node->getAssignment().get());
            this->setValue(node->getSymbId() + " <- " + this->astgen->getValue());
        } else {
            this->setValue("");
        }
    }

    void RSymbols::visit(Category *node) {
        this->setValue(node->getName());
    }

    void RSymbols::visit(DiscreteVariable *node) {
        this->setValue(node->getName());
    }

    void RSymbols::visit(FunctionDefinition *node) {
        this->setValue("");
    }

    void RSymbols::visit(FunctionArgumentDefinition *node) {
        this->setValue("");
    }
}
