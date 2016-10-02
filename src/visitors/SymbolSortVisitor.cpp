/* libpharmmlcpp - Library to handle PharmML
 * Copyright (C) 2016 Rikard Nordgren and Gunnar Yngman
 *
 * This library is free software { you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation { either
 * version 3 of the License, or (at your option) any later version.
 *
 * his library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY { without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library { if not, see <http://www.gnu.org/licenses/>.
 */

#include <visitors/SymbolSortVisitor.h>
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

namespace pharmmlcpp {

    SymbolSet SymbolSortVisitor::getDerivatives() {
        return this->derivatives;
    }

    SymbolSet SymbolSortVisitor::getRandomVariables() {
        return this->randomVariables;
    }

    SymbolSet SymbolSortVisitor::getPopulationParameters() {
        return this->populationParameters;
    }

    SymbolSet SymbolSortVisitor::getParameters() {
        return this->parameters;
    }

    SymbolSet SymbolSortVisitor::getCovariates() {
        return this->covariates;
    }

    SymbolSet SymbolSortVisitor::getIndividualParameters() {
        return this->individualParameters;
    }

    SymbolSet SymbolSortVisitor::getDesignParameters() {
        return this->designParameters;
    }

    void SymbolSortVisitor::visit(ObservationModel *node) {
    }

    void SymbolSortVisitor::visit(Parameter *node) {
        this->parameters.addSymbol(node);
    }

    void SymbolSortVisitor::visit(PopulationParameter *node) {
        this->populationParameters.addSymbol(node);
    }

    void SymbolSortVisitor::visit(IndividualParameter *node) {
        this->individualParameters.addSymbol(node);
    }

    void SymbolSortVisitor::visit(RandomVariable *node) {
        this->randomVariables.addSymbol(node);
    }

    void SymbolSortVisitor::visit(VariabilityLevel *node) {
    }

    void SymbolSortVisitor::visit(IndependentVariable *node) {
    }

    void SymbolSortVisitor::visit(Variable *node) {
    }

    void SymbolSortVisitor::visit(DesignParameter *node) {
        this->designParameters.addSymbol(node);
    }

    void SymbolSortVisitor::visit(DerivativeVariable *node) {
        this->derivatives.addSymbol(node);
    }

    void SymbolSortVisitor::visit(Covariate *node) {
        this->covariates.addSymbol(node);
    }

    void SymbolSortVisitor::visit(Category *node) {
        this->categories.addSymbol(node);
    }

    void SymbolSortVisitor::visit(DiscreteVariable *node) {
    }

    void SymbolSortVisitor::visit(FunctionDefinition *node) {
    }

    void SymbolSortVisitor::visit(FunctionArgumentDefinition *node) {
    }
}
