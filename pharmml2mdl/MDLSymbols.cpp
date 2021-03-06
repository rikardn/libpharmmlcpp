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
#include <symbols/VariabilityLevel.h>
#include <symbols/IndependentVariable.h>
#include <symbols/Variable.h>
#include <symbols/DerivativeVariable.h>
#include <symbols/Covariate.h>
#include <symbols/Category.h>
#include <symbols/FunctionDefinition.h>

#include "MDLSymbols.h"
#include "MDLAstGenerator.h"

namespace pharmmlcpp
{
    MDLSymbols::MDLSymbols(std::shared_ptr<Logger> logger) {
        this->logger = logger;
        std::unique_ptr<MDLAstGenerator> ast_gen(new MDLAstGenerator(logger));
        this->ast_gen = std::move(ast_gen);
    }

    /// Add a single declared symbol name so a Variable without an assignment sharing the same name won't be output
    void MDLSymbols::addDeclaredSymbol(std::string name) {
        this->declared_symbols.insert(name);
    }

    /// Add a single dosing target name so output can add "::dosingTarget" to such variables
    void MDLSymbols::addDosingTarget(std::string name) {
        this->dosing_targets.insert(name);
    }

    /// Add multiple dosing target names so output can add "::dosingTarget" to such variables
    void MDLSymbols::addDosingTargets(std::vector<std::string> names) {
        for (std::string name : names) {
            this->addDosingTarget(name);
        }
    }

    void MDLSymbols::visit(ObservationModel *node) {
        this->setValue(node->getName());
    }

    void MDLSymbols::visit(Parameter *node) {
    }

    void MDLSymbols::visit(PopulationParameter *node) {
        if (!node->hasAssignment()) {
            this->setValue(node->getName());
            return;
        }
        this->setValue(node->getName() + " = " + this->ast_gen->acceptRoot(node->getAssignment().get()));
    }

    void MDLSymbols::visit(IndividualParameter *node) {
        this->setValue(node->getName());
    }

    void MDLSymbols::visit(RandomVariable *node) {
        this->setValue(node->getName());
    }

    void MDLSymbols::visit(VariabilityLevel *node) {
        this->setValue(node->getName());
    }

    void MDLSymbols::visit(IndependentVariable *node) {
        this->setValue(node->getName());
    }

    void MDLSymbols::visit(Variable *node) {
        if (node->getAssignment()) {
            this->setValue(node->getName() + " = " + this->ast_gen->acceptRoot(node->getAssignment().get()));
        } else {
            std::string name = node->getName();
            auto got = this->declared_symbols.find(name);
            // This protects against e.g. duplicate PK macro/variable without assignment definition in UCs: 4_1, 5, 6, 6_1, 7, 9, etc..
            if (got == this->declared_symbols.end()) {
                auto got = this->dosing_targets.find(name);
                name = (got == this->dosing_targets.end()) ? name : name + "::dosingVar";
                this->setValue(name);
            } else {
                this->logger->warning("Variable without assignment '" + name + "' ignored due to being presumed duplicate of already declared symbol (with same name)", node);
                this->setValue("");
            }
        }
    }

    void MDLSymbols::visit(DesignParameter *node) {
        this->visit(static_cast<Variable *>(node));
    }


    void MDLSymbols::visit(DerivativeVariable *node) {
        TextFormatter form;

        std::string name = node->getName();
        form.openVector(name + " : {}", 0, ", ");
        form.add("deriv = " + this->ast_gen->acceptRoot(node->getAssignment().get()));
        form.add("init = " + this->ast_gen->acceptRoot(node->getInitialValue().get()));
        form.add("x0 = " + this->ast_gen->acceptRoot(node->getInitialTime().get()));
        form.closeVector();

        this->setValue(form.createString());
    }

    void MDLSymbols::visit(Covariate *node) {
        this->setValue(node->getName());
    }

    void MDLSymbols::visit(Category *node) {
        this->setValue("." + node->getName());
    }

    void MDLSymbols::visit(DiscreteVariable *node) {
        this->setValue(node->getName());
    }

    void MDLSymbols::visit(FunctionDefinition *node) {
        this->setValue(node->getName());
    }

    void MDLSymbols::visit(FunctionArgumentDefinition *node) {
        this->setValue(node->getName());
    }
}
