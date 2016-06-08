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
#include <visitors/SymbolNameVisitor.h>
#include <visitors/SymbRefFinder.h>

namespace PharmML
{
    // private
    std::string PopEDGenerator::accept(AstNode *node) {
        node->accept(&this->ast_gen);
        return ast_gen.getValue();
    }

    // public
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
        if (this->model->getModelDefinition()->getStructuralModel()->hasDerivatives()) {
            form.add("library(deSolve)");
        }
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
        form.emptyLine();

        // Generate PopED database call (initial design and space)
        form.addMany(this->genDatabaseCall());

        return form.createString();
    }

    void PopEDGenerator::collectTrialDesignInformation() {
        // Generate trial design information
        TrialDesign *td = this->model->getTrialDesign();
        if (td) {
            Arms *arms = td->getArms();
            if (arms) {
                this->nArms = arms->getArms().size();

                // Need to get all IndividualAdministrations separately as these cannot be Objects and referenced.
                // This might change in future versions of PharmML
                Interventions *interventions = td->getInterventions();
                if (interventions) {
                    std::vector<IndividualAdministration *> ia = interventions->getIndividualAdministrations();
                    this->td_visitor.setIndividualAdministrations(ia);
                }

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

        SymbolSet needed_symbols = this->model->getModelDefinition()->getObservationModel()->getNeededSymbols();

        if (this->model->getModelDefinition()->getParameterModel()) {
            // Declare population parameters except variability parameters
            for (CPharmML::PopulationParameter *param : this->model->getConsolidator()->getPopulationParameters()->getPopulationParameters()) {
                if (!param->isVariabilityParameter() and !param->isCorrelation()) {
                    param->getPopulationParameter()->accept(&symbgen);
                    form.add(param->getPopulationParameter()->getSymbId() + "=bpop[" + symbgen.getValue() + "]");
                }
            }

            // Declare ETAs
            SymbolSet random_vars = needed_symbols.getRandomVariables();
            Symbol *error = this->model->getModelDefinition()->getObservationModel()->getResidualError()->getSymbol();
            random_vars.removeSymbol(error);

            for (Symbol *symbol : random_vars) {
                symbol->accept(&symbgen);
                form.add(symbol->getSymbId() + "=b[" + symbgen.getValue() + "]");
                this->etas.push_back(static_cast<RandomVariable *>(symbol));        // Save for later use
            }
        }

        // Declare dose/time
        int index = 1;
        if (this->model->getTrialDesign()) {
            std::vector<std::string> time_names = this->td_visitor.getTimeNames();
            std::vector<std::string> amount_names = this->td_visitor.getDoseNames();

            for (std::vector<std::string>::size_type i = 0; i != time_names.size(); i++) {
                form.add(amount_names[i] + "=a[" + std::to_string(2*i + 1) + "]");
                form.add(time_names[i] + "=a[" + std::to_string(2*i + 2) + "]");
                index += 2;
            }
        }

        // Declare covariates
        SymbolSet covariates = needed_symbols.getCovariates();
        for (Symbol *symbol : covariates) {
            Covariate *cov = static_cast<Covariate *>(symbol);
            if (!cov->isTransformed()) {
                form.add(symbol->getSymbId() + "=a[" + std::to_string(index++) + "]");
            }
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

        ObservationModel *observationModel = this->model->getModelDefinition()->getObservationModel();

        SymbolSet needed_symbols = observationModel->getNeededSymbols();

        SymbolSet derivs_set = needed_symbols.getDerivatives();

        SymbolSet nopass;
        nopass.merge(needed_symbols.getParameters());
        nopass.merge(needed_symbols.getPopulationParameters());
        nopass.merge(needed_symbols.getRandomVariables());

        std::vector<Symbol *> deriv_deps = derivs_set.getOrderedDependenciesNoPass(nopass);
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
        // FIXME: Can probably be safely removed now
        // Visit all CommonVariable's to build consolidating objects
        for (CommonVariable *var : model->getModelDefinition()->getStructuralModel()->getVariables()) {
            var->accept(&this->r_gen);
        }

        bool has_derivatives = this->model->getModelDefinition()->getObservationModel()->getNeededSymbols().hasDerivatives();

        TextFormatter form;

        // Generate separate ODE function
        if (has_derivatives) {
            form.addMany(this->genODEFunc());
        }

        // Function header
        form.indentAdd("ff <- function(model_switch, xt, parameters, poped.db) {");
        form.indentAdd("with(as.list(parameters), {");

        // Init values
        if (has_derivatives) {
            TextFormatter dini_formatter;
            dini_formatter.openVector("d_ini <- c()", 0, ", ");
            for (Symbol *symbol : this->derivs) {
                DerivativeVariable *derivative_variable = static_cast<DerivativeVariable *>(symbol);
                AstNode *init = derivative_variable->getInitialValue();
                init->accept(&this->ast_gen);
                dini_formatter.add(symbol->getSymbId() + "=" + this->ast_gen.getValue());
                SymbRefFinder finder;     // Needed to find SymbRefs in the initial value
                init->accept(&finder);
                for (SymbRef *symbref : finder.getSymbRefs()) {
                    Symbol *symbol = symbref->getSymbol();
                    symbol->accept(&this->r_symb);
                    std::string res = this->r_symb.getValue();
                    if (!res.empty()) {         // Some symbols give empty strings. Ignore these.
                        form.add(this->r_symb.getValue());
                    }
                }
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
        }

        // Y definition

        if (!has_derivatives) {
            form.indentAdd("mod <- function(xt) {");
            form.add(model->getIndependentVariable()->getSymbId() + " <- xt");
        }

        // Don't want to have derivatives or pass through dependencies of derivatives
        SymbolSet needed_symbols = this->model->getModelDefinition()->getObservationModel()->getNeededSymbols();
        SymbolSet derivs_set = needed_symbols.getDerivatives();
        SymbolSet indiv_params = needed_symbols.getIndividualParameters();

        // Don't want to pass through ordinary parameters except IndividualParameters
        derivs_set.merge(needed_symbols.getAllParameters());
        derivs_set.remove(indiv_params);

        SymbRef *output = this->model->getModelDefinition()->getObservationModel()->getOutput();
        SymbolSet output_set;
        output_set.addSymbol(output->getSymbol());

        // Special case if output is derivative
        if (output_set.hasDerivatives()) {
            form.add("y <- out[, '" + output->getSymbol()->getSymbId() + "']");
        } else {
            SymbolSet post_ode_symbol_set = output_set.getDependenciesNoPass(derivs_set);

            // Remove non-transformed covariates
            SymbolSet covariates = post_ode_symbol_set.getCovariates();
            for (Symbol *symbol : covariates) {
                Covariate *cov = static_cast<Covariate *>(symbol);
                if (!cov->isTransformed()) {
                    post_ode_symbol_set.removeSymbol(symbol);
                }
            }

            std::vector<Symbol *> post_ode_symbols = post_ode_symbol_set.getOrdered();
            post_ode_symbols.push_back(output->getSymbol());

            // Need R symbol generator with non-default AST generator that use non-default symbol generator
            PopEDPastDerivativesSymbols *symbgen = new PopEDPastDerivativesSymbols();   // Symbol name generator
            PopEDAstGenerator *astgen = new PopEDAstGenerator(symbgen);     // Ast generator taking the symbol name generator as argument
            RSymbols rsymb_past(astgen);                                    // Symbol expression generator with ast generator as argument
            for (Symbol *symbol : post_ode_symbols) {
                symbol->accept(&rsymb_past);
                form.add(rsymb_past.getValue());
            }

            form.add("y <- " + output->getSymbIdRef());
        }

        if (!has_derivatives) {
            form.outdentAdd("}");
            form.emptyLine();
            form.add("y <- sapply(xt, mod)");
        }

        if (has_derivatives) {
            form.add("y=y[match(times_xt, out[,'time'])]");
            form.add("y=cbind(y)");
        }

        // Return list
        form.add("return(list(y=y, poped.db=poped.db))");
        form.outdentAdd("})");
        form.outdentAdd("}");
        form.emptyLine();

        return form.createString();
    }

    std::string PopEDGenerator::genErrorFunction() {
        TextFormatter form;

        ObservationModel *om = this->model->getModelDefinition()->getObservationModel();
        std::string result_name = om->getSymbId();
        std::string output_name = om->getOutput()->getSymbIdRef();

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

        if (!this->model->getModelDefinition()->getParameterModel()) {
            return "";
        }

        TextFormatter bpop;
        bpop.openVector("bpop = c()", 0, ", ");
        TextFormatter notfixed_bpop;
        notfixed_bpop.openVector("notfixed_bpop = c()", 0, ", ");
        auto pop_params_obj = this->model->getConsolidator()->getPopulationParameters();
        /* Note: One more level in-between. Should make support of multiple parameter models easier and present a nice place (CPharmML::PopulationParameters)
         * for convenience functions that can do more than only get the consolidated objects. */
        auto pop_params = pop_params_obj->getPopulationParameters();
        for (auto pop_param : pop_params) {
            if (!pop_param->isVariabilityParameter()) {
                std::string parameter_name = pop_param->getPopulationParameter()->getSymbId();
                if (pop_param->getParameterEstimation()) {
                    bpop.add(parameter_name + "=" + this->accept(pop_param->getParameterEstimation()->getInitValue())); 
                    notfixed_bpop.add(parameter_name + "=" + (pop_param->getParameterEstimation()->isFixed() ? "0" : "1"));
                } else {
                    bpop.add(parameter_name + "=0");
                    notfixed_bpop.add(parameter_name + "=1");
                }
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

        // Values for ETAs
        // FIXME: This code is a bit experimental. Ideas exist to make this more reusable, but these ideas are to involved to fit in this comment.
        if (this->etas.size() > 0) {
            TextFormatter d_formatter;
            d_formatter.openVector("d = c()", 0, ", ");
            for (RandomVariable *rand_var : this->etas) {
                for (auto pop_param : pop_params) {
                    auto consolidatedRandom = pop_param->getRandomVariables();
                    bool found = std::find(std::begin(consolidatedRandom), std::end(consolidatedRandom), rand_var) != std::end(consolidatedRandom);

                    if (found) {
                        AstNode *value;
                        for (DistributionParameter *dist_par : rand_var->getDistribution()->getDistributionParameters()) {
                            if (dist_par->getName() == "var") {
                                value = pop_param->getParameterEstimation()->getInitValue();
                            } else if (dist_par->getName() == "stdev") {
                                BinopPower *power = new BinopPower();
                                power->setLeft(pop_param->getParameterEstimation()->getInitValue());
                                power->setRight(new ScalarInt(2));
                                value = power;
                            }
                        } 
                        d_formatter.add(rand_var->getSymbId() + "=" + this->accept(value));
                    }
                }
            }

            d_formatter.closeVector();
            d_formatter.noFinalNewline();
            form.add(d_formatter.createString());

            TextFormatter covd_formatter;
            covd_formatter.openVector("covd = c()", 0, ", ");

            ParameterModel *parameterModel = this->model->getModelDefinition()->getParameterModel();
            std::vector<ParameterEstimation *> parameterEstimations = this->model->getModellingSteps()->getOptimalDesignSteps()[0]->getParameters();
                
            for (std::vector<RandomVariable *>::size_type col = 0; col < this->etas.size(); col++) {
                for (std::vector<RandomVariable *>::size_type row = col + 1; row < this->etas.size(); row++) {
                    AstNode *cov = parameterModel->initialCovariance(this->etas[col], this->etas[row], parameterEstimations);
                    covd_formatter.add(this->accept(cov));
                }
            }

            covd_formatter.closeVector();
            covd_formatter.noFinalNewline();
            form.add(covd_formatter.createString());
        }

        // TrialDesign
        form.add("groupsize = 1");

        form.add("m = " + std::to_string(this->nArms));
        form.addMany(this->td_visitor.getDatabaseXT());
        form.addMany(this->td_visitor.getDatabaseA());


        // Handle the first DesignSpace. FIXME: Generalization needed. More design spaces? Should use oid
        TrialDesign *td = model->getTrialDesign();
        if (td) {
            DesignSpaces *ds = td->getDesignSpaces();
            if (ds) {
                DesignSpace *designSpace = ds->getDesignSpaces()[0];
                AstAnalyzer intervalAnalyzer;
                AstNode *dosingTimes = designSpace->getDosingTimes();
                if (dosingTimes) {
                    designSpace->getDosingTimes()->accept(&intervalAnalyzer);
                    Interval *interval = intervalAnalyzer.getPureInterval();
                    if (interval) {
                        interval->getLeftEndpoint()->accept(&this->ast_gen);
                        form.add("minxt=" + this->ast_gen.getValue());
                        interval->getRightEndpoint()->accept(&this->ast_gen);
                        form.add("maxxt=" + this->ast_gen.getValue());
                    }
                }
            }
        }

        if (scalar) {
            form.add("iFIMCalculationType = 0");
        }

        form.closeVector();

        return form.createString();
    }

    Symbol *PopEDGenerator::findSigmaSymbol() {
        SymbRef *error_ref = this->model->getModelDefinition()->getObservationModel()->getResidualError();
        Symbol *rand_var = error_ref->getSymbol();

        auto pop_params = this->model->getConsolidator()->getPopulationParameters()->getPopulationParameters();

        for (auto pop_param : pop_params) {
            auto consolidatedRandom = pop_param->getRandomVariables();
            bool found = std::find(std::begin(consolidatedRandom), std::end(consolidatedRandom), rand_var) != std::end(consolidatedRandom);
            if (pop_param->isVariabilityParameter() && found) {
                return pop_param->getPopulationParameter();
            }
        }

        return nullptr;
    }
}
