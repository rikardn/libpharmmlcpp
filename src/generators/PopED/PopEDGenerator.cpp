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
#include <algorithm>

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
        this->logger.setToolName("PopED");
        // FIXME: Bad design to put in model here? A smell of visitor pattern breakdown. Solution might be visitor on Model level.
        // Note that this is now also present in RPharmMLGenerator::genFunctionDefinitions(Model *model); Maybe bad. Maybe not bad?
        this->model = model;
        TextFormatter form(2, ' ');

        this->collectTrialDesignInformation();

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

    void PopEDGenerator::collectTrialDesignInformation() {
        // Generate trial design information
        TrialDesign *td = this->model->getTrialDesign();
        if (td) {
            Arms *arms = td->getArms();
            this->nArms = arms->getArms().size();

            // Need to get all IndividualAdministrations separately as these cannot be Objects and referenced.
            // This might change in future versions of PharmML
            Interventions *interventions = td->getInterventions();
            if (interventions) {
                std::vector<IndividualAdministration *> ia = interventions->getIndividualAdministrations();
                this->td_visitor.setIndividualAdministrations(ia);
            }

            if (arms) {
                for (Arm *arm : arms->getArms()) {
                    arm->accept(&this->td_visitor);
                }
            }
        }
    }

    std::string PopEDGenerator::genParameterModel() {
        TextFormatter form;
        form.indentAdd("sfg <- function(x, a, bpop, b, bocc) {");
        form.openVector("parameters = c()", 1, ", ");
        PopEDSymbols symbgen;
        PopEDAstGenerator astgen(&symbgen);
        for (IndividualParameter *parameter : model->getModelDefinition()->getParameterModel()->getIndividualParameters()) {
            std::string result;
            if (!parameter->isStructured()) {
                parameter->getAssignment()->accept(&astgen);
                std::string assign = astgen.getValue();
                result = parameter->getSymbId() + "=bpop[" + assign + "]";
            }
            form.add(result);
        }

        // Remaining THETAs. From all parameters remove those referenced from individual parmaters and random variables
        // FIXME: refactor using Consolidator
        for (PopulationParameter *parameter : model->getModelDefinition()->getParameterModel()->getPopulationParameters()) {
            remaining_parameters.addSymbol(parameter);
        }
        for (Parameter *parameter : model->getModelDefinition()->getParameterModel()->getParameters()) {
            remaining_parameters.addSymbol(parameter);
        }
        for (IndividualParameter *parameter : model->getModelDefinition()->getParameterModel()->getIndividualParameters()) {
            remaining_parameters.remove(parameter->referencedSymbols);
        }
        for (RandomVariable *var : model->getModelDefinition()->getParameterModel()->getRandomVariables()) {
            remaining_parameters.remove(var->referencedSymbols);
        }
        for (Symbol *symb : remaining_parameters) {
            symb->accept(&symbgen);
            form.add(symb->getSymbId() + "=bpop[" + symbgen.getValue() + "]");
        }

        std::vector<std::string> time_names = this->td_visitor.getTimeNames();
        std::vector<std::string> amount_names = this->td_visitor.getDoseNames();

        for (std::vector<std::string>::size_type i = 0; i != time_names.size(); i++) {
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
        form.indentAdd("ode_func <- function(Time, State, Pars) {");
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
            this->derivs.push_back(symbol);
            name_list.push_back("d" + symbol->getSymbId());
        }

        // Return list
        form.add("return(list(" + TextFormatter::createInlineVector(name_list, "c()", ", ") + "))");
        form.outdentAdd("})");
        form.outdentAdd("}");
        form.emptyLine();

        return form.createString();
    }

    // Get the name of the dose variable.
    std::string PopEDGenerator::getDoseVariable() {
        // FIXME: Assumes a specific structure
        Administration *adm = this->model->getTrialDesign()->getInterventions()->getAdministrations()[0];
        if (adm->getTargetSymbRef()) {
            adm->getTargetSymbRef()->accept(&this->ast_gen);
            return this->ast_gen.getValue();
        } else {
            TargetMapping *target = adm->getTargetMapping();
            return target->getMaps()[0].modelSymbol;  // FIXME: What should really be the constraints here? 
        }
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
        TextFormatter dini_formatter;
        dini_formatter.openVector("d_ini <- c()", 0, ", ");
        for (Symbol *symbol : this->derivs) {
            DerivativeVariable *derivative_variable = static_cast<DerivativeVariable *>(symbol);
            derivative_variable->getInitialValue()->accept(&this->ast_gen);
            dini_formatter.add(symbol->getSymbId() + "=" + this->ast_gen.getValue());
        }
        dini_formatter.closeVector();
        form.add(dini_formatter.createString());

        // Dose times
        form.add("times_xt <- drop(xt)");
        form.add("dose_times <- c(" + TextFormatter::createCommaSeparatedList(this->td_visitor.getTimeNames()) + ")");
        form.add("dose_amt <- c(" + TextFormatter::createCommaSeparatedList(this->td_visitor.getDoseNames()) + ")");
        form.add("integration_start_time <- 0");

        // Event data
        // TODO: Consolidate and use actual dosing information (e.g. dose variable, linkage method and dosing compartment)
        form.indentAdd("eventdat <- data.frame(var = c('" + this->getDoseVariable() +  "'),");
        form.add("time = dose_times,");
        form.add("value = dose_amt, method = c('add'))");
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
        form.indentAdd("with(as.list(parameters), {");
        form.add("returnArgs <- do.call(poped.db$model$ff_pointer,list(model_switch,xt,parameters,poped.db))");
        form.add(result_name +" <- returnArgs[[1]]");
        form.add("poped.db <- returnArgs[[2]]");
        form.emptyLine();

        form.add(output_name + " <- " + result_name);

        // Get weight definition
        // TODO: Figure out how to get the dependencies of w in here
        RAstGenerator error_ast_gen;
        om->getErrorModel()->accept(&error_ast_gen);
        form.add("w <- " + error_ast_gen.getValue());

        // Increase y by error fraction (weight * epsilon)
        // TODO: Figure out how to resolve this with multiple EPS
        form.add(result_name + " = " + result_name + " + w*epsi[,1]");

        // Return list
        form.emptyLine();
        form.add("return(list(y=" + result_name + ",poped.db=poped.db))");
        form.outdentAdd("})");
        form.outdentAdd("}");

        return form.createString();
    }

    std::string PopEDGenerator::genDatabaseCall() {
        TextFormatter form;

        form.openVector("poped.db <- create.poped.database()", 1, ", ");
        form.add("ff_fun = 'ff'");
        form.add("fg_fun = 'sfg'");
        form.add("fError_fun = 'feps'");

        TextFormatter bpop;
        bpop.openVector("bpop = c()", 0, ", ");
        TextFormatter notfixed_bpop;
        notfixed_bpop.openVector("notfixed_bpop = c()", 0, ", ");
        auto pop_params_obj = this->model->getConsolidator()->getPopulationParameters();
        /* Note: One more level in-between. Should make support of multiple parameter models easier and present a nice place (CPharmML::PopulationParameters)
         * for convenience functions that can do more than only get the consolidated objects. */
        auto pop_params = pop_params_obj->getPopulationParameters();
        for (auto pop_param : pop_params) {
            if (pop_param->getIndividualParameters().size() != 0) {     // Check if individual parameter is connected
                std::string indiv_name = pop_param->getIndividualParameters()[0]->getSymbId();  // FIXME: When will there be more than one?
                if (pop_param->getParameterEstimation()) {
                    bpop.add(indiv_name + "=" + this->accept(pop_param->getParameterEstimation()->getInitValue()));
                    notfixed_bpop.add(pop_param->getParameterEstimation()->isFixed() ? "0" : "1");
                } else {
                    bpop.add(indiv_name + "=0");
                    notfixed_bpop.add("1");
                }
            }
            PopulationParameter *symbol = pop_param->getPopulationParameter();
            if (this->remaining_parameters.hasSymbol(symbol)) {
                bpop.add(symbol->getSymbId() + "=" + this->accept(pop_param->getParameterEstimation()->getInitValue()));
                notfixed_bpop.add(pop_param->getParameterEstimation()->isFixed() ? "0" : "1");
            }
        }
        bpop.closeVector();
        bpop.noFinalNewline();
        notfixed_bpop.closeVector();
        notfixed_bpop.noFinalNewline();
        form.add(bpop.createString());
        form.add(notfixed_bpop.createString());

        // Sigmas
        TextFormatter sigma_init_formatter;
        sigma_init_formatter.openVector("sigma = c()", 0, ", ");
        TextFormatter sigma_fixed_formatter;
        sigma_fixed_formatter.openVector("notfixed_sigma = c()", 0, ", ");

        // FIXME: Find sigma. More than one sigma? Move into general method
        SymbRef *error_ref = this->model->getModelDefinition()->getObservationModel()->getResidualError();
        Symbol *rand_var = error_ref->getSymbol();

        // If RandomVariable has constant parameters then it cannot be found in consolidator
        // FIXME: Can we assume RandomVariable type of ResidualError?
        // Initial value of RandomVariable with constant parameters is the variance parameter
        RandomVariable *random = static_cast<RandomVariable*>(rand_var);
        AstAnalyzer analyzer;
        bool scalar = false;
        for (DistributionParameter *dist_par : random->getDistribution()->getDistributionParameters()) {
            if (dist_par->getName() == "var") {
                dist_par->getAssignment()->accept(&analyzer);
                if (analyzer.getPureScalar()) {
                    scalar = true;
                    sigma_init_formatter.add(this->accept(dist_par->getAssignment()));
                    sigma_fixed_formatter.add("0");
                }
            }
        } 

        if (!scalar) {
            for (auto pop_param : pop_params) {
                auto consolidatedRandom = pop_param->getRandomVariables();
                bool found = std::find(std::begin(consolidatedRandom), std::end(consolidatedRandom), rand_var) != std::end(consolidatedRandom);
                if (pop_param->isVariabilityParameter() && found) {
                    sigma_init_formatter.add(this->accept(pop_param->getParameterEstimation()->getInitValue()));
                    bool sigma_fix = pop_param->getParameterEstimation()->isFixed();
                    sigma_fixed_formatter.add(sigma_fix ? "0" : "1");
                    break;
                }
            }
        }

        sigma_init_formatter.closeVector();
        sigma_init_formatter.noFinalNewline();
        sigma_fixed_formatter.closeVector();
        sigma_fixed_formatter.noFinalNewline();
        form.add(sigma_init_formatter.createString());
        form.add(sigma_fixed_formatter.createString());

        form.add("groupsize = 1");

        form.add("m = " + std::to_string(this->nArms));
        form.addMany(this->td_visitor.getDatabaseXT());
        form.addMany(this->td_visitor.getDatabaseA());

        if (scalar) {
            form.add("iFIMCalculationType = 0");
        }

        form.closeVector();

        return form.createString();
    }

    // Visitors
    void PopEDGenerator::visit(FunctionDefinition *node) {}

    void PopEDGenerator::visit(FunctionArgumentDefinition *node) {}

    void PopEDGenerator::visit(PopulationParameter *node) {}

    void PopEDGenerator::visit(IndividualParameter *node) {
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
    void PopEDGenerator::visit(TargetMapping *node) {}

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

    void PopEDGenerator::visit(PKMacro *node) {}
}
