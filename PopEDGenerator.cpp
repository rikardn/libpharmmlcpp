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

#include "PopEDGenerator.h"

namespace PharmML
{
    // private
    void PopEDGenerator::setValue(std::string str) {
        this->value = str;
    }

    // Helper function to reduce redundant code
    // TODO: Overload with similar function accepting vector of nodes and performing element->accept(this) instead (how?)
    std::string PopEDGenerator::formatVector(std::vector<std::string> vector, std::string prefix, std::string quote, int pre_indent) {
        std::string s = prefix + "(";
        std::string sep = ", ";
        if (pre_indent > 0) {
            sep = ",\n" + std::string(pre_indent + s.size(), ' ');
        }

        bool first = true;
        for (std::string element : vector) {
            if (first) {
                first = false;
            } else {
                s += sep;
            }
            s += quote + element + quote;
        }
        return(s + ")");
    }

    std::string PopEDGenerator::accept(AstNode *node) {
        node->accept(&this->ast_gen);
        return ast_gen.getValue();
    }

    // public
    std::string PopEDGenerator::getValue() {
        return this->value;
    }

    // FIXME: Bad design to put in model here? A smell of visitor pattern breakdown. Solution might be visitor on Model level
    std::string PopEDGenerator::generateModel(Model *model) {
        this->model = model;

        std::string s;
        s += this->genParameterModelFunc();
        s += "\n\n" + this->genStructuralModelFunc();
        return s;
    }

    std::string PopEDGenerator::genParameterModelFunc() {
        std::string s = "sfg <- function(x, a, bpop, b, bocc) {\n";
        std::vector<std::string> list;
        for (IndividualParameter *parameter : model->getModelDefinition()->getParameterModel()->getIndividualParameters()) {
            // FIXME: Don't need accept here as we already know the type. Could as well put code here?
            parameter->accept(this);
            list.push_back(this->getValue());
        }
        s += this->formatVector(list, "    parameters=c", "", 1);
        return(s + "\n    return(parameters)\n}");
    }

    std::string PopEDGenerator::genStructuralModelFunc() {
        std::string s = "ff <- function(model_switch, xt, parameters, poped.db) {\n";

        std::vector<std::string> list;
        for (CommonVariable *var : model->getModelDefinition()->getStructuralModel()->getVariables()) {
            var->accept(&this->r_gen);
            list.push_back(this->r_gen.getValue());
        }
        s += this->formatVector(list, "Q", "Y", 1);
        s += "\n}\n\n";

        s += this->r_gen.derivatives.genODEFunc();

        return s;
    }

    void PopEDGenerator::visit(FunctionDefinition *node) {}
    void PopEDGenerator::visit(PopulationParameter *node) {}
    void PopEDGenerator::visit(IndividualParameter *node) {
        std::string result = node->getSymbId() + "=bpop[" + std::to_string(this->parameter_count) + "]";
        this->parameter_count++;
        this->setValue(result);
    }
    void PopEDGenerator::visit(RandomVariable *node) {}
    void PopEDGenerator::visit(Covariate *node) {}
    void PopEDGenerator::visit(IndependentVariable *node) {}
    void PopEDGenerator::visit(Variable *node) {}
    void PopEDGenerator::visit(DerivativeVariable *node) {}
    void PopEDGenerator::visit(ObservationModel *node) {}
    void PopEDGenerator::visit(Distribution *node) {}
    void PopEDGenerator::visit(ColumnMapping *node) {}

    void PopEDGenerator::visit(ExternalFile *node) {}
    void PopEDGenerator::visit(DataColumn *node) {}
    void PopEDGenerator::visit(Dataset *node) {}

    void PopEDGenerator::visit(ExternalDataset *node) {}

    void PopEDGenerator::visit(Interventions *node) {}
    void PopEDGenerator::visit(Administration *node) {}
    void PopEDGenerator::visit(IndividualAdministration *node) {}

    void PopEDGenerator::visit(Observations *node) {}
    void PopEDGenerator::visit(Observation *node) {}
    void PopEDGenerator::visit(IndividualObservations *node) {}
    void PopEDGenerator::visit(ObservationCombination *node) {}

    void PopEDGenerator::visit(Arms *node) {}
    void PopEDGenerator::visit(Arm *node) {}
    void PopEDGenerator::visit(InterventionSequence *node) {}
    void PopEDGenerator::visit(ObservationSequence *node) {}
    void PopEDGenerator::visit(OccasionSequence *node) {}

    void PopEDGenerator::visit(DesignSpaces *node) {}
    void PopEDGenerator::visit(DesignSpace *node) {}
}
