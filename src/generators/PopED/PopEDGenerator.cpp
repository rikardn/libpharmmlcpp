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

    std::string PopEDGenerator::accept(AstNode *node) {
        node->accept(&this->ast_gen);
        return ast_gen.getValue();
    }

    // public
    std::string PopEDGenerator::getValue() {
        return this->value;
    }
    
    // Generators
    std::string PopEDGenerator::generateModel(Model *model) {
        // FIXME: Bad design to put in model here? A smell of visitor pattern breakdown. Solution might be visitor on Model level.
        // Note that this is now also present in RPharmMLGenerator::genFunctionDefinitions(Model *model); Maybe bad. Maybe not bad?
        this->model = model;
        RFormatter form(2, ' ');
        
        // Output function definitions (e.g. MDL proportionalError function)
        for (std::string function_def : this->r_gen.genFunctionDefinitions(model)) {
            form.addMany(function_def);
        }
        form.add("");
        
        // Generate the three PopED functions
        form.addMany(this->genParameterModel());
        form.addMany(this->genStructuralModel());
        form.addMany(this->genErrorFunction());
        form.add("");
        
        // Generate PopED database call (initial design and space)
        form.addMany(this->genDatabaseCall());
        
        return form.createString();
    }

    std::string PopEDGenerator::genParameterModel() {
        RFormatter form;
        form.indentAdd("sfg <- function(x, a, bpop, b, bocc) {");
        form.openVector("parameters = c()", 1, ", ");
        for (IndividualParameter *parameter : model->getModelDefinition()->getParameterModel()->getIndividualParameters()) {
            // FIXME: Don't need accept here as we already know the type. Could as well put code here?
            parameter->accept(this);
            form.add(this->getValue());
        }
        form.closeVector();
        form.add("return(parameters)");
        form.outdentAdd("}");
        return form.createString();
    }
    
    std::string PopEDGenerator::genODEFunc() {
        RFormatter form;
        // Function header
        form.indentAdd("ode_func <- function(Time, Stat, Pars) {");
        form.indentAdd("with(as.list(c(State, Pars)), {");
        
        // Derivative definitions
        std::vector<std::string> name_list;
        std::vector<std::string> symbols = this->r_gen.consol.derivs.getSymbols();
        std::vector<std::string> assigns = this->r_gen.consol.derivs.getAssigns();
        for (int i = 0; i < symbols.size(); i++) {
            form.add("d" + symbols[i] + " <- " + assigns[i]);
            name_list.push_back("d" + symbols[i]);
        }
        
        // Return list
        form.add("return(list(" + PharmML::formatVector(name_list, "c", "") + "))");
        form.outdentAdd("})");
        form.outdentAdd("}");

        return form.createString(); 
    }

    std::string PopEDGenerator::genStructuralModel() {
        // Visit all CommonVariable's to build consolidating objects
        for (CommonVariable *var : model->getModelDefinition()->getStructuralModel()->getVariables()) {
            var->accept(&this->r_gen);
        }
        
        RFormatter form;
        
        // Function header
        form.indentAdd("ff <- function(model_switch, xt, parameters, poped.db) {");
        form.indentAdd("with(as.list(parameters), {");
        
        // Init values
        form.add("d_ini <- " + this->r_gen.consol.derivs.genInitVector());
        
        // Dose times
        form.add("times_xt <- drop(xt)");
        // TODO: Consolidate dosing times (from TrialDesign) and use actual information (not a sequence!)
        form.add("dose_times = seq(from=0,to=max(times_xt),by=TAU)");
        
        // Event data
        // TODO: Consolidate and use actual dosing information (e.g. dose variable, linkage method and dosing compartment)
        form.indentAdd("eventdat <- data.frame(var = c('A1'),");
        form.add("time = dose_times,");
        form.add("value = c(DOSE), method = c('add'))");
        form.closeIndent();
        
        // ODE call
        form.add("out <- ode(d_ini, times, ode_func, parameters, events = list(data = eventdat))");
        
        // Y definition
        form.addMany(this->r_gen.consol.vars.genStatements());
        // TODO: Get structural part (only?) of observation model and resolv derivative symbol references to this form
        form.add("y = out[, 'A2']/(V/Favail)");
        form.add("y=y[match(times_xt,out[,'time'])]");
        form.add("y=cbind(y)");
        // Just set the output part of the obs model for now (seems to be how PopED does it)
        // TODO: Develop complete structural Y resolve
        std::string out = this->accept(model->getModelDefinition()->getObservationModel()->getOutput());
        form.add("y = " + out);
        
        // Return list
        form.add("return(list(y=y,poped.db=poped.db))");
        form.outdentAdd("})");
        form.outdentAdd("}");

        // Generate separate ODE function
        form.addMany(this->genODEFunc());

        return form.createString();
    }
    
    std::string PopEDGenerator::genErrorFunction() {
        RFormatter form;
        
        form.indentAdd("feps <- function(model_switch,xt,parameters,epsi,poped.db) {");
        form.add("y <- ff(model_switch,xt,parameters,poped.db)[[1]]");
        
        // Get weight definition
        // TODO: Figure out how to get the dependencies of w in here
        ObservationModel *node = model->getModelDefinition()->getObservationModel();
        form.add("w <- " + this->accept(node->getErrorModel()));
        
        // Increase y by error fraction (weight * epsilon)
        // TODO: Figure out how to resolve this with multiple EPS
        form.add("y = y + w*epsi[,1]");
        
        // Return list
        form.add("return(list(y=y,poped.db=poped.db))");
        form.outdentAdd("}");
        
        return form.createString();
    }
    
    std::string PopEDGenerator::genDatabaseCall() {
        RFormatter form;
        
        form.openVector("poped.db <- create.poped.database()", 1, ", ");
        form.add("ff_file = 'ff'");
        form.add("fg_file = 'sfg'");
        form.add("fError_file_file = 'feps'");
        
        std::vector<IndividualParameter *> ips = model->getModelDefinition()->getParameterModel()->getIndividualParameters();
        RFormatter bpop;
        bpop.openVector("c()", 0, ", ");
        for (IndividualParameter *ip : ips) {
            if (!ip->isStructured()) {
                /* Only half the story: Now the assignment needs to be matched to the corresponding PopulationParameter and
                 * that PopulationParameter looked up for initial estimate in ModellingSteps section... */
                // TODO: The above
                std::string s = ip->getSymbId() + "=" + this->accept(ip->getAssignment());
                bpop.add(s);
            }
            
        }
        bpop.closeVector();
        form.add("bpop = " + bpop.createString(false));
        
        form.add("notfixed_bpop = NULL");
        form.add("d = NULL");
        form.add("sigma = NULL");
        form.add("groupsize = NULL");
        form.add("xt = NULL");
        form.add("minxt = NULL");
        form.add("maxxt = NULL");
        form.add("a = NULL");
        form.add("mina = NULL");
        form.add("maxa = NULL");
        form.closeVector();
        
        return form.createString();
    }
    
    // Visitors
    void PopEDGenerator::visit(FunctionDefinition *node) {}
    void PopEDGenerator::visit(PopulationParameter *node) {}

    void PopEDGenerator::visit(IndividualParameter *node) {
        std::string result;
        if (!node->isStructured()) {
            node->getAssignment()->accept(&this->poped_astgen);
            std::string assign = this->poped_astgen.getValue();
            result = node->getSymbId() + "=bpop[" + assign + "]";
        }
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
