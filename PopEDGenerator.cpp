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
        s += this->genParameterModel();
        s += "\n\n" + this->genStructuralModel();
        return s;
    }

    std::string PopEDGenerator::genParameterModel() {
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
    
    std::string PopEDGenerator::genODEFunc() {
        Text::Indenter ind;
        // Function header
        ind.addRowIndent("ode_func <- function(Time, Stat, Pars) {");
        ind.addRowIndent("with(as.list(c(State, Pars)), {");
        
        // Derivative definitions
        std::vector<std::string> name_list;
        std::vector<std::string> symbols = this->r_gen.consol.derivs.getSymbols();
        std::vector<std::string> assigns = this->r_gen.consol.derivs.getAssigns();
        for (int i = 0; i < symbols.size(); i++) {
            ind.addRow("d" + symbols[i] + " <- " + assigns[i]);
            name_list.push_back("d" + symbols[i]);
        }
        
        // Return list
        ind.addRow("return(list(" + Text::formatVector(name_list, "c", "") + "))");
        ind.addRowOutdent("})");
        ind.addRowOutdent("}");

        return ind.createString(); 
    }

    std::string PopEDGenerator::genStructuralModel() {
        // Visit all CommonVariable's to build consolidating objects
        for (CommonVariable *var : model->getModelDefinition()->getStructuralModel()->getVariables()) {
            var->accept(&this->r_gen);
        }
        
        Text::Indenter ind;
        
        // Function header
        ind.addRowIndent("ff <- function(model_switch, xt, parameters, poped.db) {");
        ind.addRowIndent("with(as.list(parameters), {");
        
        // Init values
        ind.addRow("d_ini <- " + this->r_gen.consol.derivs.genInitVector());
        
        // Dose times
        ind.addRow("times_xt <- drop(xt)");
        // TODO: Consolidate dosing times (from TrialDesign) and use actual information (not a sequence!)
        ind.addRow("dose_times = seq(from=0,to=max(times_xt),by=TAU)");
        
        // Event data
        // TODO: Consolidate and use actual dosing information (e.g. dose variable, linkage method and dosing compartment)
        ind.addRowIndent("eventdat <- data.frame(var = c('A1'),");
        ind.addRow("time = dose_times,");
        ind.addRow("value = c(DOSE), method = c('add'))");
        ind.closeIndent();
        
        // ODE call
        ind.addRow("out <- ode(d_ini, times, ode_func, parameters, events = list(data = eventdat))");
        
        // Y definition
        // TODO: Get structural part (only?) of observation model and resolv derivative symbol references to this form
        ind.addRow("y = out[, 'A2']/(V/Favail)");
        ind.addRow("y=y[match(times_xt,out[,'time'])]");
        ind.addRow("y=cbind(y)");
        
        // Return list
        ind.addBlock(this->r_gen.consol.vars.genStatements());
        ind.addRow("return(list(y=y,poped.db=poped.db))");
        ind.addRowOutdent("})");
        ind.addRowOutdent("}");

        // Generate separate ODE function
        ind.addBlock(this->genODEFunc());

        return ind.createString();
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
