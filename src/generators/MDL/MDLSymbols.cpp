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

#include <generators/MDL/MDLSymbols.h>
#include <symbols/ObservationModel.h>
#include <symbols/PopulationParameter.h>
#include <symbols/IndividualParameter.h>
#include <symbols/RandomVariable.h>
#include <symbols/VariabilityLevel.h>
#include <symbols/IndependentVariable.h>
#include <symbols/Variable.h>
#include <symbols/DerivativeVariable.h>
#include <symbols/Covariate.h>
#include <symbols/FunctionDefinition.h>

#include <generators/MDL/MDLAstGenerator.h>

namespace PharmML
{
    MDLSymbols::MDLSymbols(std::shared_ptr<Logger> logger) {
        this->logger = logger;
        std::unique_ptr<MDLAstGenerator> ast_gen(new MDLAstGenerator(logger));
        this->ast_gen = std::move(ast_gen);
    }

    void MDLSymbols::visit(ObservationModel *node) {
        this->setValue(node->getSymbId());
    }

    void MDLSymbols::visit(Parameter *node) {
    }

    void MDLSymbols::visit(PopulationParameter *node) {
        this->setValue(node->getSymbId());
    }

    void MDLSymbols::visit(IndividualParameter *node) {
        this->setValue(node->getSymbId());
    }

    void MDLSymbols::visit(RandomVariable *node) {
        this->setValue(node->getSymbId());
    }

    void MDLSymbols::visit(VariabilityLevel *node) {
        this->setValue(node->getSymbId());
    }

    void MDLSymbols::visit(IndependentVariable *node) {
        this->setValue(node->getSymbId());
    }

    void MDLSymbols::visit(Variable *node) {
        if (node->getAssignment()) {
            this->setValue(node->getSymbId() + " = " + this->ast_gen->accept(node->getAssignment()));
        } else {
            this->setValue(node->getSymbId() + "::dosingVar"); // FIXME: This is a hack! I'm not sure if dosingVar is the only reasonable alternative.
        }
    }

    void MDLSymbols::visit(DerivativeVariable *node) {
        TextFormatter form;

        std::string name = node->getSymbId();
        form.openVector(name + " : {}", 0, ", ");
        form.add("deriv = " + this->ast_gen->accept(node->getAssignment()));
        form.add("init = " + this->ast_gen->accept(node->getInitialValue()));
        form.add("x0 = " + this->ast_gen->accept(node->getInitialTime()));
        form.closeVector();

        this->setValue(form.createString());
    }

    void MDLSymbols::visit(Covariate *node) {
        this->setValue(node->getSymbId());
    }

    void MDLSymbols::visit(FunctionDefinition *node) {
        this->setValue(node->getSymbId());
    }

    void MDLSymbols::visit(FunctionArgumentDefinition *node) {
        this->setValue(node->getSymbId());
    }
}
