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
#include <iostream>

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
        TextFormatter form(2, ' ');
       
        // Preamble
        form.add("library(PopED)");
        form.add("library(deSolve)");
        form.emptyLine();

        // Output function definitions (e.g. MDL proportionalError function)
        for (std::string function_def : this->r_gen.genFunctionDefinitions(model)) {
            form.addMany(function_def);
        }
        form.emptyLine();
        
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
        TextFormatter form;
        form.indentAdd("sfg <- function(x, a, bpop, b, bocc) {");
        form.openVector("parameters = c()", 1, ", ");
        for (IndividualParameter *parameter : model->getModelDefinition()->getParameterModel()->getIndividualParameters()) {
            // FIXME: Don't need accept here as we already know the type. Could as well put code here?
            parameter->accept(this);
            form.add(this->getValue());
        }

        std::vector<std::string> time_names = genDoseTimeNames();
        std::vector<std::string> amount_names = genDoseAmountNames();

        for (int i = 0; i < time_names.size(); i++) {
            form.add(amount_names[i] + " = a[" + std::to_string(2*i + 1) + "]");
            form.add(time_names[i] + " = a[" + std::to_string(2*i + 2) + "]");
        }

        form.closeVector();
        form.add("return(parameters)");
        form.outdentAdd("}");
        form.emptyLine();
        return form.createString();
    }

    std::string PopEDGenerator::genODEFunc() {
        TextFormatter form;
        // Function header
        form.indentAdd("ode_func <- function(Time, Stat, Pars) {");
        form.indentAdd("with(as.list(c(State, Pars)), {");
     
        // FIXME: Should be a method to do all this and not so ugly
        auto derivs = this->model->getModelDefinition()->getStructuralModel()->getDerivatives();
        SymbolSet derivs_set;
        for (auto deriv : derivs) {
            derivs_set.addSymbol(deriv);
        }

        // FIXME: for same reason as above
        auto indivs = this->model->getModelDefinition()->getParameterModel()->getIndividualParameters();
        SymbolSet indiv_set;
        for (auto indiv : indivs) {
            indiv_set.addSymbol(indiv);
        }

        auto deriv_deps = derivs_set.getOrderedDependenciesNoPass(indiv_set);
        for (Symbol *symbol : deriv_deps) {
            symbol->accept(&this->r_symb);
            form.add(this->r_symb.getValue());
        }

        form.emptyLine();

        // Derivative definitions
        std::vector<std::string> name_list;

        for (Symbol *symbol : derivs_set) {
            symbol->accept(&this->r_symb);
            form.add(this->r_symb.getValue());
            name_list.push_back("d" + symbol->getSymbId());
        }

        // Return list
        form.add("return(list(" + TextFormatter::createInlineVector(name_list, "c()", ", ") + "))");
        form.outdentAdd("})");
        form.outdentAdd("}");
        form.emptyLine();

        return form.createString(); 
    }

    // Generate a vector of dose time names
    std::vector<std::string> PopEDGenerator::genDoseTimeNames() {
        // FIXME: Must check invariant at construction time. Need TrialDesign etc for PopED to convert to something useful
        // FIXME: Need information from Arms here. Assume a lot for now. 
        IndividualAdministration *ia = this->model->getTrialDesign()->getInterventions()->getIndividualAdministrations()[0];
        Dataset *ds = ia->getDataset();
        int numrows = ds->getColumns()[0]->getNumRows();

        std::vector<std::string> result;

        for (int i = 1; i <= numrows; i++) {
            result.push_back("DOSE_" + std::to_string(i) + "_TIME");
        }

        return result;
    }

    // Generate a vector of dose time amount names
    std::vector<std::string> PopEDGenerator::genDoseAmountNames() {
        // FIXME: Combine with genDoseTimeNames
        // FIXME: Must check invariant at construction time. Need TrialDesign etc for PopED to convert to something useful
        // FIXME: Need information from Arms here. Assume a lot for now. 
        IndividualAdministration *ia = this->model->getTrialDesign()->getInterventions()->getIndividualAdministrations()[0];
        Dataset *ds = ia->getDataset();
        int numrows = ds->getColumns()[0]->getNumRows();

        std::vector<std::string> result;

        for (int i = 1; i <= numrows; i++) {
            result.push_back("DOSE_" + std::to_string(i) + "_AMT");
        }

        return result;
    }


    // Get the name of the dose variable. 
    std::string PopEDGenerator::getDoseVariable() {
        // FIXME: Assumes a specific structure
        Administration *adm = this->model->getTrialDesign()->getInterventions()->getAdministrations()[0];
        adm->getTarget()->accept(&this->ast_gen);
        return this->ast_gen.getValue();
    }

    std::string PopEDGenerator::genStructuralModel() {
        // Visit all CommonVariable's to build consolidating objects
        for (CommonVariable *var : model->getModelDefinition()->getStructuralModel()->getVariables()) {
            var->accept(&this->r_gen);
        }

        TextFormatter form;
 
        // Generate separate ODE function
        form.addMany(this->genODEFunc());

        // Function header
        form.indentAdd("ff <- function(model_switch, xt, parameters, poped.db) {");
        form.indentAdd("with(as.list(parameters), {");
        
        // Init values
        form.add("d_ini <- " + this->r_gen.consol.derivs.genInitVector());

        // Dose times
        form.add("times_xt <- drop(xt)");
        // TODO: Consolidate dosing times (from TrialDesign) and use actual information (not a sequence!)
        std::vector<std::string> dose_time_names = this->genDoseTimeNames();
        form.add("dose_times <- c(" + TextFormatter::createCommaSeparatedList(dose_time_names) + ")");
        form.add("integration_start_time <- 0");

        // Event data
        // TODO: Consolidate and use actual dosing information (e.g. dose variable, linkage method and dosing compartment)
        form.indentAdd("eventdat <- data.frame(var = c('" + this->getDoseVariable() +  "'),");
        form.add("time = dose_times,");
        std::vector<std::string> dose_amount_names = this->genDoseAmountNames();
        form.add("value = c(" + TextFormatter::createCommaSeparatedList(dose_amount_names) + "), method = c('add'))");
        form.closeIndent();
        form.add("times <- sort(unique(c(0, times_xt, dose_times)))");

        // ODE call
        form.add("out <- ode(d_ini, times, ode_func, parameters, events = list(data = eventdat))");
        form.emptyLine();
        
        // Y definition

        // FIXME: This code again!
        std::vector<CommonVariable *> derivatives = this->model->getModelDefinition()->getStructuralModel()->getDerivatives();
        SymbolSet derivs_set;
        for (auto deriv : derivatives) {
            derivs_set.addSymbol(deriv);
        }

        SymbRef *output = this->model->getModelDefinition()->getObservationModel()->getOutput();
        SymbolSet output_set;
        output_set.addSymbol(output->getSymbol());
        std::vector<Symbol *> post_ode_symbols = output_set.getOrderedDependenciesNoPass(derivs_set);
        post_ode_symbols.push_back(output->getSymbol());

        // Need R symbol generator with non-default AST generator that use non-default symbol generator 
        PopEDPastDerivativesSymbols *symbgen = new PopEDPastDerivativesSymbols();   // Symbol name generator
        PopEDAstGenerator *astgen = new PopEDAstGenerator(symbgen);     // Ast generator taking the symbol name generator as argument
        RSymbols rsymb_past(astgen);                                    // Symbol expression generator with ast generator as argument
        for (Symbol *symbol : post_ode_symbols) {
            symbol->accept(&rsymb_past);
            form.add(rsymb_past.getValue());
        }

        form.add("y <- " + output->toString());
        form.add("y=y[match(times_xt, out[,'time'])]");
        form.add("y=cbind(y)");
                
        // Return list
        form.add("return(list(y=y,poped.db=poped.db))");
        form.outdentAdd("})");
        form.outdentAdd("}");
        form.emptyLine();

        return form.createString();
    }
    
    std::string PopEDGenerator::genErrorFunction() {
        TextFormatter form;
     
        ObservationModel *om = this->model->getModelDefinition()->getObservationModel();
        std::string result_name = om->getSymbId();
        std::string output_name = om->getOutput()->toString();

        form.indentAdd("feps <- function(model_switch, xt, parameters, epsi, poped.db) {");
        form.add("returnArgs <- do.call(poped.db$model$ff_pointer,list(model_switch,xt,parameters,poped.db))");
        form.add(result_name +" <- returnArgs[[1]]");
        form.add("poped.db <- returnArgs[[2]]");
        form.emptyLine();

        form.add(output_name + " <- " + result_name);

        // Get weight definition
        // TODO: Figure out how to get the dependencies of w in here
        PopEDErrorAstGenerator error_ast_gen;
        om->getErrorModel()->accept(&error_ast_gen);
        form.add("w <- " + error_ast_gen.getValue());

        // Increase y by error fraction (weight * epsilon)
        // TODO: Figure out how to resolve this with multiple EPS
        form.add(result_name + " = " + result_name + " + w*epsi[,1]");
        
        // Return list
        form.emptyLine();
        form.add("return(list(y=" + result_name + ",poped.db=poped.db))");
        form.outdentAdd("}");
        
        return form.createString();
    }
    
    std::string PopEDGenerator::genDatabaseCall() {
        TextFormatter form;
        
        form.openVector("poped.db <- create.poped.database()", 1, ", ");
        form.add("ff_fun = 'ff'");
        form.add("fg_fun = 'sfg'");
        form.add("fError_fun = 'feps'");

/*     
    sigma = c(0.01), # variance units
    groupsize = 1,
    m = 2,
    xt = list(c(8,10,13,15,18,22,26),
              c(8,10,13,15,18,22,26,30,35,40)),
    a = list(c(DOSE_1_AMT=0,DOSE_1_TIME=0,DOSE_2_AMT=0,DOSE_2_TIME=0,DOSE_3_AMT=0,DOSE_3_TIME=0),
             c(DOSE_1_AMT=30,DOSE_1_TIME=8,DOSE_2_AMT=30,DOSE_2_TIME=12,DOSE_3_AMT=30,DOSE_3_TIME=16))
*/

        TextFormatter bpop;
        bpop.openVector("c()", 0, ", ");
        auto pop_params = this->model->getConsolidator()->getPopulationParameters();
        for (auto pop_param : pop_params) {
            if (pop_param->getIndividualParameters().size() != 0) {     // Check if individual parameter is connected
                std::string indiv_name = pop_param->getIndividualParameters()[0]->getSymbId();  // FIXME: When will there be more than one?
                bpop.add(indiv_name + "=" + this->accept(pop_param->getParameterEstimation()->getInitValue()));
            }
        }
        bpop.closeVector();
        form.add("bpop = " + bpop.createString(false));
    
        form.add("d = c()");
        form.add("groupsize = 1");
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
    void PopEDGenerator::visit(VariabilityLevel *node) {}
    void PopEDGenerator::visit(Correlation *node) {}
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
    
    void PopEDGenerator::visit(ParameterEstimation *node) {}
}
