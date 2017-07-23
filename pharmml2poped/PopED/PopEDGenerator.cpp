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
#include <sstream>
#include <iomanip> // std::setprecision (FIXME comment)
#include <fstream>
#include <algorithm>
#include <visitors/SymbolNameVisitor.h>
#include <visitors/SymbRefFinder.h>

namespace pharmmlcpp
{
    // private
    std::string PopEDGenerator::accept(AstNode *node) {
        return this->ast_gen.acceptRoot(node);
    }

    // public
    // Generators
    std::string PopEDGenerator::generateModel(PharmML *model) {
        model->setSymbolNamer(&this->symbolNamer);      // Use the RSymbolNamer for symbol naming
        this->logger.setToolName("PopED");
        // FIXME: Bad design to put in model here? A smell of visitor pattern breakdown. Solution might be visitor on Model level.
        // Note that this is now also present in RPharmMLGenerator::genFunctionDefinitions(PharmML *model); Maybe bad. Maybe not bad?
        this->model = model;
        TextFormatter form(2, ' ');

        this->collectTrialDesignInformation();

        // Preamble
        form.add("library(PopED)");
        this->has_derivatives = this->model->getModelDefinition()->getObservationModel()->getNeededSymbols().hasDerivatives();
        if (this->model->getModelDefinition()->getStructuralModel()->getPKMacros()) {       // PKMacros will be converted into derivatives later
            this->has_derivatives = true;               // FIXME: do PKMacros conversion earlier
        }
        if (this->has_derivatives) {
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
                    form.add(param->getPopulationParameter()->getName() + "=bpop[" + symbgen.getValue() + "]");
                }
            }

            // Declare ETAs
            SymbolSet random_vars = needed_symbols.getRandomVariables();
            Symbol *error = this->model->getModelDefinition()->getObservationModel()->getResidualError()->getSymbol();
            random_vars.remove(error);

            for (Symbol *symbol : random_vars) {
                symbol->accept(&symbgen);
                form.add(symbol->getName() + "=b[" + symbgen.getValue() + "]");
                this->etas.push_back(static_cast<RandomVariable *>(symbol));        // Save for later use
            }
        }

        TrialDesign *td = this->model->getTrialDesign();

        // Declare dose/time
        int index = 1;
        if (td) {
            // FIXME: Doses can be handled in two ways. Either directly fed into events or also added to a-vector.
            // It is not clear when to use which or if this can be unified. The current rule is very ad hoc.
            if (td->getOptimizationParameters().isEmpty() and td->numberOfArms() > 1) {      // Only handle boluses specially if no opt params
                std::vector<std::string> time_names = this->td_visitor.getTimeNames();
                std::vector<std::string> amount_names = this->td_visitor.getDoseNames();

                for (std::vector<std::string>::size_type i = 0; i != time_names.size(); i++) {
                    form.add(amount_names[i] + "=a[" + std::to_string(2*i + 1) + "]");
                    form.add(time_names[i] + "=a[" + std::to_string(2*i + 2) + "]");
                    index += 2;
                }
            }
        }

        // DesignParameters used from DesignSpaces
        if (td) {
            SymbolSet design_params = td->getOptimizationParameters();
            for (Symbol *symbol : design_params) {
                form.add(symbol->getName() + "=a[" + std::to_string(index++) + "]");
                this->designParameters.push_back(symbol);       // For later retrieval
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

        // Currently separate handling of DesignParameters
        // Output all DesignParameters except those optimized on.
        // FIXME: This could be reduced to only output those actually needed as per regular variables below
        // FIXME: Currently only use DesignParametes on TrialDesign level.
        if (this->td_visitor.hasInfusions()) {
            TrialDesign *td = this->model->getTrialDesign();
            if (td) {
                SymbolSet param_set;
                SymbolSet design_params = td->getOptimizationParameters();
                for (DesignParameter *param : td->getDesignParameters()) {
                    param_set.addSymbol(param);
                }
                param_set.remove(design_params);
                for (Symbol *symbol : param_set.getOrdered()) {
                    symbol->accept(&this->r_symb);
                    form.add(this->r_symb.getValue());
                }
            }
        }

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

        // Special case if infusions are present
        if (this->td_visitor.hasInfusions()) {
            form.addMany(this->InfusionFunction);
            form.emptyLine();

            this->ast_gen.acceptRoot(this->td_visitor.getCombinationStart().get());
            form.add("offset <- " + this->ast_gen.getValue());
            this->ast_gen.acceptRoot(this->td_visitor.getInterventionStart().get());
            form.add("offset <- offset + " + this->ast_gen.getValue());
            for (std::string row : this->td_visitor.getInfFuncCalls()) {
                form.add(row);
            }
            // Add infusions with same target together
            for (auto &pair : this->td_visitor.getInfusionMap()) {
                std::string infadd = "inf_" + pair.first->getName() + " <- ";
                infadd += TextFormatter::createInlineVector(pair.second, "", " + ");
                form.add(infadd);
            }
        }

        // Derivative definitions
        std::vector<std::string> name_list;

        for (Symbol *symbol : derivs_set) {
            std::string infusion_rate;  // Infusion rate to add if needed
            if (this->td_visitor.hasInfusions()) {
                if (this->td_visitor.getInfusionMap().count(symbol)) {
                    infusion_rate = " + inf_" + symbol->getName();
                }
            }
            symbol->accept(&this->r_symb);
            form.add(this->r_symb.getValue() + infusion_rate);
            this->derivs.push_back(symbol);
            name_list.push_back("d" + symbol->getName());
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
            this->ast_gen.acceptRoot(adm->getTargetSymbRef());
            return this->ast_gen.getValue();
        } else {
            TargetMapping *target = adm->getTargetMapping();
            // FIXME: This code should not be here. Resolving Macros should already have been done
            std::string admNumber = target->getMaps()[0].admNumber;
            if (!admNumber.empty()) {
                PKMacro *macro = this->model->getModelDefinition()->getStructuralModel()->getPKMacros()->getAdministration(std::stoi(admNumber));
                return macro->getName();
            }
            return target->getMaps()[0].modelSymbol;  // FIXME: What should really be the constraints here?
        }
    }

    std::string PopEDGenerator::genStructuralModel() {
        if (model->getModelDefinition()->getStructuralModel()->getPKMacros()) {
           model->getModelDefinition()->getStructuralModel()->convertMacrosIntoDEs(model); 
        }

        // FIXME: Can probably be safely removed now
        // Visit all CommonVariable's to build consolidating objects
        for (CommonVariable *var : model->getModelDefinition()->getStructuralModel()->getVariables()) {
            var->accept(&this->r_gen);
        }

        TextFormatter form;

        // Generate separate ODE function
        if (this->has_derivatives) {
            form.addMany(this->genODEFunc());
        }

        // Function header
        form.indentAdd("ff <- function(model_switch, xt, parameters, poped.db) {");
        form.indentAdd("with(as.list(parameters), {");

        if (this->td_visitor.hasBoluses()) {
            TrialDesign *td = this->model->getTrialDesign();
            if (td) {
                SymbolSet param_set;
                SymbolSet design_params = td->getOptimizationParameters();
                for (DesignParameter *param : td->getDesignParameters()) {
                    param_set.addSymbol(param);
                }
                param_set.remove(design_params);
                for (Symbol *symbol : param_set.getOrdered()) {
                    symbol->accept(&this->r_symb);
                    form.add(this->r_symb.getValue());
                }
            }
        }

        // Init values
        if (this->has_derivatives) {
            TextFormatter dini_formatter;
            dini_formatter.openVector("d_ini <- c()", 0, ", ");
            for (Symbol *symbol : this->derivs) {
                DerivativeVariable *derivative_variable = static_cast<DerivativeVariable *>(symbol);
                std::shared_ptr<AstNode> init = derivative_variable->getInitialValue();
                this->ast_gen.acceptRoot(init.get());
                dini_formatter.add(symbol->getName() + "=" + this->ast_gen.getValue());
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
            if (this->td_visitor.hasBoluses()) {
                if (this->model->getTrialDesign()->getOptimizationParameters().isEmpty() and this->model->getTrialDesign()->numberOfArms() > 1) {      // Only handle boluses specially if no opt params
                    form.add("dose_times <- c(" + TextFormatter::createCommaSeparatedList(this->td_visitor.getTimeNames()) + ")");
                    form.add("dose_amt <- c(" + TextFormatter::createCommaSeparatedList(this->td_visitor.getDoseNames()) + ")");
                } else {
                    TextFormatter dosetimes_formatter;
                    dosetimes_formatter.openVector("dose_times <- c()", 0, ", ");
                    for (auto &pair : this->td_visitor.getBolusAmounts()) {
                        TextFormatter amount_formatter;
                        amount_formatter.openVector("dose_amt_" + pair.first->getName() + " <- c()", 0, ", ");
                        for (std::shared_ptr<AstNode> node : pair.second) {
                            this->accept(node.get());
                            amount_formatter.add(ast_gen.getValue());
                        }
                        amount_formatter.closeVector();
                        amount_formatter.noFinalNewline();
                        form.add(amount_formatter.createString());
                    }
                    for (auto &pair : this->td_visitor.getBolusTimes()) {
                        TextFormatter times_formatter;
                        times_formatter.openVector("dose_times_" + pair.first->getName() + " <- c()", 0, ", ");
                        for (std::shared_ptr<AstNode> node : pair.second) {
                            this->accept(node.get());
                            times_formatter.add(ast_gen.getValue());
                        }
                        times_formatter.closeVector();
                        times_formatter.noFinalNewline();
                        form.add(times_formatter.createString());
                        dosetimes_formatter.add("dose_times_" + pair.first->getName());
                    }
                    dosetimes_formatter.noFinalNewline();
                    form.add(dosetimes_formatter.createString());
                }
            } else if (this->td_visitor.hasInfusions()) {     // FIXME: Should not be mutually exclusive
                TextFormatter dt_formatter;
                dt_formatter.openVector("dose_times <- c()", 0, ", ");
                for (std::shared_ptr<AstNode> dt_node : this->td_visitor.getDoseTimes()) {
                    dt_node->accept(&this->ast_gen);
                    dt_formatter.add(this->ast_gen.getValue());
                }
                dt_formatter.closeVector();
                form.add(dt_formatter.createString());
            }

            form.add("integration_start_time <- 0");

            // Event data
            // TODO: Consolidate and use actual dosing information (e.g. dose variable, linkage method and dosing compartment)
            if (this->td_visitor.hasBoluses()) {
                if (this->model->getTrialDesign()->getOptimizationParameters().isEmpty() and this->model->getTrialDesign()->numberOfArms() > 1) {      // Only handle boluses specially if no opt params
                    form.indentAdd("eventdat <- data.frame(var = c('" + this->getDoseVariable() +  "'),");
                    form.add("time = dose_times,");
                    form.add("value = dose_amt, method = c('add'))");
                    form.closeIndent();
                } else {
                    for (auto &pair : this->td_visitor.getBolusAmounts()) {
                        form.indentAdd("eventdat_" + pair.first->getName() + " <- data.frame(var = c('" + pair.first->getName() + "'),");
                        form.add("time = dose_times_" + pair.first->getName() + ",");
                        form.add("value = dose_amt_" + pair.first->getName() + ",");
                        form.add("method = c('add')");
                        form.outdentAdd(")");
                    }
                    TextFormatter rbind_formatter;
                    rbind_formatter.openVector("eventdat <- rbind()", 0, ", ");
                    for (auto &pair : this->td_visitor.getBolusAmounts()) {
                       rbind_formatter.add("eventdat_" + pair.first->getName());
                    }
                    rbind_formatter.closeVector();
                    rbind_formatter.noFinalNewline();
                    form.add(rbind_formatter.createString());
                }
            }
            form.add("times <- sort(unique(c(0, times_xt, dose_times)))");

            // ODE call
            TextFormatter ode_call;
            ode_call.openVector("out <- ode()", 0);
            ode_call.add("d_ini");
            ode_call.add("times");
            ode_call.add("ode_func");
            ode_call.add("parameters");
            if (this->td_visitor.getNumObservations() == 1) {
                ode_call.add("hmax=0.01");      // Default hmax will probably be to big for single observations
            }
            if (this->td_visitor.hasBoluses()) {
                ode_call.add("events=list(data=eventdat)");
            }
            ode_call.add("method=poped.db$settings$iDiffSolverMethod");
            ode_call.add("atol=poped.db$settings$AbsTol");
            ode_call.add("rtol=poped.db$settings$RelTol");
            ode_call.noFinalNewline();
            ode_call.closeVector();
            form.add(ode_call.createString());
            form.emptyLine();
        }

        // Y definition

        if (!this->has_derivatives) {
            form.indentAdd("mod <- function(xt) {");
            form.add(model->getIndependentVariable()->getName() + " <- xt");
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
            form.add("y <- out[, '" + output->getSymbol()->getName() + "']");
        } else {
            SymbolSet post_ode_symbol_set = output_set.getDependenciesNoPass(derivs_set);

            // Remove non-transformed covariates
            /*SymbolSet covariates = post_ode_symbol_set.getCovariates();
            for (Symbol *symbol : covariates) {
                Covariate *cov = static_cast<Covariate *>(symbol);
                if (!cov->isTransformed()) {
                    post_ode_symbol_set.remove(symbol);
                }
            }*/

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

            form.add("y <- " + output->getSymbol()->getName());
        }

        if (!this->has_derivatives) {
            form.outdentAdd("}");
            form.emptyLine();
            form.add("y <- sapply(xt, mod)");
        }

        if (this->has_derivatives) {
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
        this->accept(om->getErrorModel().get());
        form.add("w <- " + ast_gen.getValue());

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

        // Try to get PopED algorithm (containing settings) from PharmML
        Algorithm *algo = nullptr;
        pharmmlcpp::ModellingSteps *msteps = this->model->getModellingSteps();
        if (msteps) {
            std::vector<OptimalDesignStep *> od_steps = msteps->getOptimalDesignSteps();
            for (OptimalDesignStep *od_step : od_steps) {
                // Get and erase non-PopED operations
                std::vector<Operation *> ops = od_step->getOperations();
                ops.erase(std::remove_if(ops.begin(), ops.end(), [&](Operation *x) {
                    if (x->getAlgorithm() && (x->getAlgorithm()->isNamed("PopED") || x->getAlgorithm()->isDefinedAs("PopED"))) {
                        return false;
                    } else {
                        this->logger.warning("No PopED operation algorithm found in design step", x);
                        return true;
                    }
                }), ops.end());

                // Filter forward primary PopED operation
                if (ops.size() > 1) {
                    int min_o = ops.back()->getOrder();
                    ops.erase(std::remove_if(ops.begin(), ops.end(), [&min_o](Operation *x) {
                        if (min_o > x->getOrder()) {
                            min_o = x->getOrder();
                            return false;
                        } else {
                            return true;
                        }
                    }), ops.end());
                    this->logger.warning("Multiple PopED operations in same optimal design step, selecting lowest in order number", ops.at(0));
                }

                // Warn if multiple OptimalDesignStep's found with PopED algorithms in them
                if (algo && !ops.empty()) {
                    this->logger.warning("Multiple PopED algorithms in multiple design steps, selecting first seen", algo);
                } else if (!ops.empty()) {
                    algo = ops.at(0)->getAlgorithm();
                }
            }

            // Warn if no PopED algorithm found
            if (!algo) {
                this->logger.warning("No PopED-specific settings could be retrieved from modelling steps", msteps);
            }
        }

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
            if (!pop_param->isVariabilityParameter() and !pop_param->isCorrelation()) {
                std::string parameter_name = pop_param->getPopulationParameter()->getSymbId();
                if (pop_param->getParameterEstimation()) {
                    bpop.add(parameter_name + "=" + this->ast_gen.acceptRoot(pop_param->getParameterEstimation()->getInitValue().get()));
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
        for (auto const &dist_par : random->getDistribution()->getDistributionParameters()) {
            if (dist_par->getName() == "var") {
                dist_par->getAssignment()->accept(&analyzer);
                if (analyzer.getPureScalar()) {
                    scalar = true;
                    sigma_init_formatter.add(this->accept(dist_par->getAssignment().get()));
                    sigma_fixed_formatter.add("0");
                }
            }
        }

        if (!scalar) {
            for (auto pop_param : pop_params) {
                auto consolidatedRandom = pop_param->getRandomVariables();
                bool found = std::find(std::begin(consolidatedRandom), std::end(consolidatedRandom), rand_var) != std::end(consolidatedRandom);
                if (pop_param->isVariabilityParameter() && found) {
                    sigma_init_formatter.add(this->accept(pop_param->getParameterEstimation()->getInitValue().get()));
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
                        for (auto const &dist_par : rand_var->getDistribution()->getDistributionParameters()) {
                            if (dist_par->getName() == "var") {
                                value = pop_param->getParameterEstimation()->getInitValue().get();
                            } else if (dist_par->getName() == "stdev") {
                                std::unique_ptr<AstNode> left = pop_param->getParameterEstimation()->getInitValue()->clone();
                                std::unique_ptr<AstNode> right = std::make_unique<ScalarInt>(2);
                                std::unique_ptr<BinopPower> power(new BinopPower(std::move(left), std::move(right)));
                                value = power.release();    // Until value is a smart pointer
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
                    std::unique_ptr<AstNode> cov = parameterModel->initialCovariance(this->etas[col], this->etas[row], parameterEstimations);
                    covd_formatter.add(this->accept(cov.get()));
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
        if (!(this->designParameters.empty() and this->model->getTrialDesign()->numberOfArms() > 1)) {
            if (!this->designParameters.empty()) {
                TextFormatter a_formatter;
                a_formatter.openVector("a = list(c()", 0, ", ");
                for (Symbol *param : this->designParameters) {
                    this->accept(static_cast<DesignParameter *>(param)->getAssignment().get());
                    a_formatter.add(this->ast_gen.getValue());
                }
                a_formatter.closeVector();
                a_formatter.noFinalNewline();
                form.add(a_formatter.createString() + ")");
            }
        } else {
            form.addMany(this->td_visitor.getDatabaseA());
        }

        TrialDesign *td = model->getTrialDesign();
        if (td) {
            DesignSpaces *ds = td->getDesignSpaces();
            if (ds) {
                if (this->designParameters.size() > 0) {
                    TextFormatter mina_formatter;
                    mina_formatter.openVector("mina = list(c()", 0, ", ");
                    TextFormatter maxa_formatter;
                    maxa_formatter.openVector("maxa = list(c()", 0, ", ");
                    for (Symbol *param : this->designParameters) {
                        DesignSpace *space = ds->getDesignSpaceFromSymbol(param);
                        Interval *interval = static_cast<Interval *>(space->getAssignment().get());       // FIXME: Assume Interval here. Discussion ongoing
                        this->accept(interval->getLeftEndpoint());
                        mina_formatter.add(this->ast_gen.getValue());
                        this->accept(interval->getRightEndpoint());
                        maxa_formatter.add(this->ast_gen.getValue());
                    }
                    mina_formatter.closeVector();
                    mina_formatter.noFinalNewline();
                    form.add(mina_formatter.createString() + ")");
                    maxa_formatter.closeVector();
                    maxa_formatter.noFinalNewline();
                    form.add(maxa_formatter.createString() + ")");
                } else {
                    // Handle the first DesignSpace. FIXME: Generalization needed. More design spaces? Should use oid
                    DesignSpace *designSpace = ds->getDesignSpaces()[0];
                    AstAnalyzer intervalAnalyzer;
                    AstNode *dosingTimes = designSpace->getDosingTimes().get();
                    if (dosingTimes) {
                        this->accept(designSpace->getDosingTimes().get());
                        Interval *interval = intervalAnalyzer.getPureInterval();
                        if (interval) {
                            this->accept(interval->getLeftEndpoint());
                            form.add("minxt=" + this->ast_gen.getValue());
                            this->accept(interval->getRightEndpoint());
                            form.add("maxxt=" + this->ast_gen.getValue());
                        }
                    }
                }
            }
        }

        // Use PopED settings from PharmML if found
        bool fim_approx_type_set = false;
        bool diff_solver_method_set = false;
        bool abs_tol_set = false;
        bool rel_tol_set = false;
        if (algo) {
            // Store recognized and parsed settings in these (NA means no setting read and UNDEF undefined/illegal value read)
            enum Criterion {EXPLICIT, UNDEF, NA};
            Criterion criterion = Criterion::NA;

            std::string penalty_file;

            enum class FIMCalcType {FO, FOCE, FOCEI, FOI, UNDEF, NA};
            FIMCalcType fim_calc_type = FIMCalcType::NA;

            enum class FIMApproxType {FULL, REDUCED, UNDEF, NA};
            FIMApproxType fim_approx_type = FIMApproxType::NA;

            bool e_family_use = false;

            enum class EIntegrationType {MC, LAPLACE, BFGS, UNDEF, NA};
            EIntegrationType e_integration_type = EIntegrationType::NA;

            enum class ESamplingType {RANDOM, LHC, UNDEF, NA};
            ESamplingType e_sampling_type = ESamplingType::NA;

            int e_samples = -1;

            enum class DiffSolverMethod {LSODA, LSODE, LSODES, LSODAR, VODE, DASPK, EULER, RK4, ODE23, ODE45, RADAU, BDF, BDF_D, ADAMS, IMPADAMS, IMPADAMS_D, ITERATION, UNDEF, NA};
            DiffSolverMethod diff_solver_method = DiffSolverMethod::NA;
            double abs_tol = 0, rel_tol = 0;

            // Recognize and parse settings (and warn of all unexpectedness)
            for (OperationProperty *prop : algo->getProperties()) {
                if (prop->isNamed("criterion")) {
                    if (prop->isString()) {
                        if (prop->isFoldedCaseString("explicit")) {
                            criterion = Criterion::EXPLICIT;
                        } else {
                            criterion = Criterion::UNDEF;
                            this->warnOperationPropertyUnexpectedValue(prop, std::vector<std::string>{"explicit"});
                        }
                    } else if (!prop->isString()) {
                        this->warnOperationPropertyUnexpectedType(prop, "string");
                    }
                } else if (prop->isNamed("file")) {
                    if (prop->isString()) {
                        penalty_file = prop->getString();
                    } else {
                        this->warnOperationPropertyUnexpectedType(prop, "string");
                    }
                } else if (prop->isNamed("computeFIM")) {
                    if (prop->isString()) {
                        if (prop->isFoldedCaseString("FO")) {
                            fim_calc_type = FIMCalcType::FO;
                        } else if (prop->isFoldedCaseString("FOCE")) {
                            fim_calc_type = FIMCalcType::FOCE;
                        } else if (prop->isFoldedCaseString("FOCEI")) {
                            fim_calc_type = FIMCalcType::FOCEI;
                        } else if (prop->isFoldedCaseString("FOI")) {
                            fim_calc_type = FIMCalcType::FOI;
                        } else {
                            fim_calc_type = FIMCalcType::UNDEF;
                            this->warnOperationPropertyUnexpectedValue(prop, std::vector<std::string>{"FO","FOCE","FOCEI","FOI"});
                        }
                    } else {
                        this->warnOperationPropertyUnexpectedType(prop, "string");
                    }
                } else if (prop->isNamed("approximationFIM")) {
                    if (prop->isString()) {
                        if (prop->isFoldedCaseString("full")) {
                            fim_approx_type = FIMApproxType::FULL;
                        } else if (prop->isFoldedCaseString("reduced")) {
                            fim_approx_type = FIMApproxType::REDUCED;
                        } else {
                            fim_approx_type = FIMApproxType::UNDEF;
                            this->warnOperationPropertyUnexpectedValue(prop, std::vector<std::string>{"full","reduced"});
                        }
                    } else {
                        this->warnOperationPropertyUnexpectedType(prop, "string");
                    }
                } else if (prop->isNamed("E_family_value")) {
                    // E family master switch: Determines if other E_family_* properties should be used
                    if (prop->isBool() && prop->getBool() == true) {
                        e_family_use = true;
                    } else if (!prop->isBool()) {
                        this->warnOperationPropertyUnexpectedType(prop, "bool");
                    }
                } else if (prop->isNamed("E_family_calc_type")) {
                    if (prop->isString()) {
                        if (prop->isFoldedCaseString("MC") || prop->isFoldedCaseString("Monte-Carlo")) {
                            e_integration_type = EIntegrationType::MC;
                        } else if (prop->isFoldedCaseString("LAPLACE")) {
                            e_integration_type = EIntegrationType::LAPLACE;
                        } else if (prop->isFoldedCaseString("BFGS")) {
                            e_integration_type = EIntegrationType::BFGS;
                        } else {
                            e_integration_type = EIntegrationType::UNDEF;
                            this->warnOperationPropertyUnexpectedValue(prop, std::vector<std::string>{"MC","LAPLACE","BFGS"});
                        }
                    } else {
                        this->warnOperationPropertyUnexpectedType(prop, "string");
                    }
                } else if (prop->isNamed("E_family_sampling")) {
                    if (prop->isString()) {
                        if (prop->isFoldedCaseString("random")) {
                            e_sampling_type = ESamplingType::RANDOM;
                        } else if (prop->isFoldedCaseString("LHC")) {
                            e_sampling_type = ESamplingType::LHC;
                        } else {
                            e_sampling_type = ESamplingType::UNDEF;
                            this->warnOperationPropertyUnexpectedValue(prop, std::vector<std::string>{"random","LHC"});
                        }
                    } else {
                        this->warnOperationPropertyUnexpectedType(prop, "string");
                    }
                } else if (prop->isNamed("E_family_edsampling")) {
                    if (prop->isInt()) {
                        if (prop->getInt() >= 0) {
                            e_samples = prop->getInt();
                        } else {
                            this->warnOperationPropertyUnderflow(prop, 0);
                        }
                    } else {
                        this->warnOperationPropertyUnexpectedType(prop, "integer");
                    }
                } else if (prop->isNamed("diff_solver_method")) {
                    if (prop->isString()) {
                        // Methods of deSolve (lsoda is default)
                        if (prop->isFoldedCaseString("LSODA")) {
                            diff_solver_method = DiffSolverMethod::LSODA;
                        } else if (prop->isFoldedCaseString("LSODE")) {
                            diff_solver_method = DiffSolverMethod::LSODE;
                        } else if (prop->isFoldedCaseString("LSODES")) {
                            diff_solver_method = DiffSolverMethod::LSODES;
                        } else if (prop->isFoldedCaseString("LSODAR")) {
                            diff_solver_method = DiffSolverMethod::LSODAR;
                        } else if (prop->isFoldedCaseString("VODE")) {
                            diff_solver_method = DiffSolverMethod::VODE;
                        } else if (prop->isFoldedCaseString("DASPK")) {
                            diff_solver_method = DiffSolverMethod::DASPK;
                        } else if (prop->isFoldedCaseString("EULER")) {
                            diff_solver_method = DiffSolverMethod::EULER;
                        } else if (prop->isFoldedCaseString("RK4")) {
                            diff_solver_method = DiffSolverMethod::RK4;
                        } else if (prop->isFoldedCaseString("ODE23")) {
                            diff_solver_method = DiffSolverMethod::ODE23;
                        } else if (prop->isFoldedCaseString("ODE45")) {
                            diff_solver_method = DiffSolverMethod::ODE45;
                        } else if (prop->isFoldedCaseString("RADAU")) {
                            diff_solver_method = DiffSolverMethod::RADAU;
                        } else if (prop->isFoldedCaseString("BDF")) {
                            diff_solver_method = DiffSolverMethod::BDF;
                        } else if (prop->isFoldedCaseString("BDF_D")) {
                            diff_solver_method = DiffSolverMethod::BDF_D;
                        } else if (prop->isFoldedCaseString("ADAMS")) {
                            diff_solver_method = DiffSolverMethod::ADAMS;
                        } else if (prop->isFoldedCaseString("IMPADAMS")) {
                            diff_solver_method = DiffSolverMethod::IMPADAMS;
                        } else if (prop->isFoldedCaseString("IMPADAMS_D")) {
                            diff_solver_method = DiffSolverMethod::IMPADAMS_D;
                        } else if (prop->isFoldedCaseString("ITERATION")) {
                            diff_solver_method = DiffSolverMethod::ITERATION;
                        } else {
                            diff_solver_method = DiffSolverMethod::UNDEF;
                            this->warnOperationPropertyUnexpectedValue(prop, std::vector<std::string>{"LSODA", "LSODE", "LSODES", "LSODAR", "VODE", "DASPK", "EULER", "RK4", "ODE23", "ODE45", "RADAU", "BDF", "BDF_D", "ADAMS", "IMPADAMS", "IMPADAMS_D", "ITERATION"});
                        }
                    } else {
                        this->warnOperationPropertyUnexpectedType(prop, "string");
                    }
                } else if (prop->isNamed("abs_tol")) {
                    if (prop->isInt() || prop->isReal()) {
                        if (prop->getReal() > 0) {
                            abs_tol = prop->getReal();
                        } else {
                            this->warnOperationPropertyUnderflow(prop, 0, true);
                        }
                    } else {
                        this->warnOperationPropertyUnexpectedType(prop, "int or real");
                    }
                } else if (prop->isNamed("rel_tol")) {
                    if (prop->isInt() || prop->isReal()) {
                        if (prop->getReal() > 0) {
                            rel_tol = prop->getReal();
                        } else {
                            this->warnOperationPropertyUnderflow(prop, 0, true);
                        }
                    } else {
                        this->warnOperationPropertyUnexpectedType(prop, "int or real");
                    }
                } else {
                    this->warnOperationPropertyUnknown(prop);
                }
            }

            // Use the parsed settings (separate to prepare for function breakout)
            if (criterion == Criterion::EXPLICIT) {
                if (!penalty_file.empty()) {
                    form.add("ofv_fun = '" + penalty_file + "'");
                    std::ifstream file(penalty_file);
                    if (!file.good()) {
                        this->logger.warning("File '" + penalty_file + "' (penalty function) not found or not accessible");
                    }
                    file.close();
                } else {
                    this->logger.warning("Explicit criterion requested but no 'file' property set (penalty function)");
                }
            } else if (!penalty_file.empty()) {
                this->logger.warning("Explicit criterion not requested but 'file' property set (penalty function), ignored");
            }
            switch (fim_calc_type) {
                case FIMCalcType::FO    : form.add("iApproximationMethod = 0");
                                          break;
                case FIMCalcType::FOCE  : form.add("iApproximationMethod = 1");
                                          break;
                case FIMCalcType::FOCEI : form.add("iApproximationMethod = 2");
                                          break;
                case FIMCalcType::FOI   : form.add("iApproximationMethod = 3");
                                          break;
                case FIMCalcType::UNDEF : form.add("# iApproximationMethod NOT SET from 'computeFIM' (value not supported)");
                                          break;
                case FIMCalcType::NA    : break;
            }
            switch (fim_approx_type) {
                case FIMApproxType::FULL    : form.add("iFIMCalculationType = 0");
                                              fim_approx_type_set = true;
                                              break;
                case FIMApproxType::REDUCED : form.add("iFIMCalculationType = 1");
                                              fim_approx_type_set = true;
                                              break;
                // TODO: iFIMCalculationType=2 to =7 (see page 15 of PopED 0.3 manual)
                case FIMApproxType::UNDEF   : form.add("# iFIMCalculationType NOT SET from 'approximationFIM' (value not supported)");
                                              break;
                case FIMApproxType::NA      : break;
            }
            if (this->has_derivatives) {
                diff_solver_method_set = true;
                switch (diff_solver_method) {
                    case DiffSolverMethod::LSODA      : form.add("iDiffSolverMethod = 'lsoda'");
                                                        break;
                    case DiffSolverMethod::LSODE      : form.add("iDiffSolverMethod = 'lsode'");
                                                        break;
                    case DiffSolverMethod::LSODES     : form.add("iDiffSolverMethod = 'lsodes'");
                                                        break;
                    case DiffSolverMethod::LSODAR     : form.add("iDiffSolverMethod = 'LSODAR'");
                                                        break;
                    case DiffSolverMethod::VODE       : form.add("iDiffSolverMethod = 'vode'");
                                                        break;
                    case DiffSolverMethod::DASPK      : form.add("iDiffSolverMethod = 'daspk'");
                                                        break;
                    case DiffSolverMethod::EULER      : form.add("iDiffSolverMethod = 'euler'");
                                                        break;
                    case DiffSolverMethod::RK4        : form.add("iDiffSolverMethod = 'rk4'");
                                                        break;
                    case DiffSolverMethod::ODE23      : form.add("iDiffSolverMethod = 'ode23'");
                                                        break;
                    case DiffSolverMethod::ODE45      : form.add("iDiffSolverMethod = 'ode45'");
                                                        break;
                    case DiffSolverMethod::RADAU      : form.add("iDiffSolverMethod = 'radau'");
                                                        break;
                    case DiffSolverMethod::BDF        : form.add("iDiffSolverMethod = 'bdf'");
                                                        break;
                    case DiffSolverMethod::BDF_D      : form.add("iDiffSolverMethod = 'bdf_d'");
                                                        break;
                    case DiffSolverMethod::ADAMS      : form.add("iDiffSolverMethod = 'adams'");
                                                        break;
                    case DiffSolverMethod::IMPADAMS   : form.add("iDiffSolverMethod = 'impadams'");
                                                        break;
                    case DiffSolverMethod::IMPADAMS_D : form.add("iDiffSolverMethod = 'impadams_d'");
                                                        break;
                    case DiffSolverMethod::ITERATION  : form.add("iDiffSolverMethod = 'iteration'");
                                                        break;
                    case DiffSolverMethod::UNDEF      :
                    case DiffSolverMethod::NA         : diff_solver_method_set = false; // Inverse cond set to save some code lines
                                                        break;
                }
                if (abs_tol > 0) {
                    std::ostringstream os; // FIXME: Not pretty when there's good float pretty printing code in RAstGenerator ScalarReal visitor (break out somewhere?)
                    os << std::scientific << std::setprecision(1) << abs_tol;
                    std::string s = os.str();
                    form.add("AbsTol = " + s);
                    abs_tol_set = true;
                }
                if (rel_tol > 0) {
                    std::ostringstream os; // FIXME: See above comment. Would really like to reuse that ScalarReal code.
                    os << std::scientific << std::setprecision(1) << rel_tol;
                    std::string s = os.str();
                    form.add("RelTol = " + s);
                    rel_tol_set = true;
                }
            } else {
                if (diff_solver_method != DiffSolverMethod::NA || abs_tol > 0 || rel_tol > 0) {
                    logger.warning("Differential equation solver settings found but no differential equations: Ignored");
                }
            }
            if (e_family_use) {
                form.add("d_switch = 0");
                switch (e_integration_type) {
                    case EIntegrationType::MC      : form.add("iEDCalculationType = 0");
                                                     break;
                    case EIntegrationType::LAPLACE : form.add("iEDCalculationType = 1");
                                                     break;
                    case EIntegrationType::BFGS    : form.add("iEDCalculationType = 2");
                                                     break;
                    case EIntegrationType::UNDEF   : form.add("# iEDCalculationType NOT SET from 'E_family_calc_type' (value not supported)");
                                                     break;
                    case EIntegrationType::NA      : break;
                }
                switch (e_sampling_type) {
                    case ESamplingType::RANDOM : form.add("bLHS = 0");
                                                 break;
                    case ESamplingType::LHC    : form.add("bLHS = 1");
                                                 break;
                    case ESamplingType::UNDEF  : form.add("# bLHS NOT SET from 'E_family_sampling' (value not supported)");
                                                 break;
                    case ESamplingType::NA     : break;
                }
                if (e_samples >= 0) {
                    form.add("ED_samp_size = " + std::to_string(e_samples));
                } else {
                    form.add("# ED_samp_size NOT SET from 'E_family_ed_sampling' (out of bounds)");
                }
            } else if (e_integration_type != EIntegrationType::NA || e_sampling_type != ESamplingType::NA || e_samples >= 0) {
                // Warn if E_family_* settings have been read but not flagged for usage
                this->logger.warning("E family settings found but not actively used ('E_family_value' is 'false' or missing)", algo);
            }
        }

        // Set settings not already overriden by PopED settings in PharmML
        if (scalar && !fim_approx_type_set) { // FIM approximation
            form.add("iFIMCalculationType = 0");
        }
        if (has_derivatives) { // Differential equation settings
            if (!diff_solver_method_set) {
                form.add("iDiffSolverMethod = 'lsoda'");
            }
            if (!abs_tol_set) {
                form.add("AbsTol = 1E-6");
            }
            if (!rel_tol_set) {
                form.add("RelTol = 1E-6");
            }
        }

        form.closeVector();

        return form.createString();
    }

    // OperationProperty has unexpected type: Warn and inform of expected type
    void PopEDGenerator::warnOperationPropertyUnexpectedType(OperationProperty *prop, std::string exp_type) {
        std::string name = prop->getName();
        this->logger.warning("Property '" + name + "' has unexpected type (expected: " + exp_type + ")", prop);
    }

    // OperationProperty is out of lower bound: Warn and inform of expected minimum
    void PopEDGenerator::warnOperationPropertyUnderflow(OperationProperty *prop, double min, bool open_interval) {
        std::string name = prop->getName();
        std::string value;
        if (prop->isInt()) {
            value = std::to_string(prop->getInt());
        } else {
            value = std::to_string(prop->getReal());
        }
        if (open_interval) {
            this->logger.warning("Property '" + name + "' value (" + value + ") is illegal (restriction: > " + std::to_string(min) + ")", prop);
        } else {
            this->logger.warning("Property '" + name + "' value (" + value + ") is illegal (restriction: >= " + std::to_string(min) + ")", prop);
        }
    }

    // OperationProperty has unexpected string value: Warn and inform of expected string value
    void PopEDGenerator::warnOperationPropertyUnexpectedValue(OperationProperty *prop, std::vector<std::string> exp_strings) {
        std::string name = prop->getName();
        TextFormatter form;
        form.openVector("", 0, ",");
        std::for_each(std::begin(exp_strings), std::end(exp_strings), [&](std::string x){ form.add("'" + x + "'"); });
        form.noFinalNewline();
        this->logger.warning("Property '" + name + "' has unknown or unsupported value '" + prop->getString() + "' (supported: " + form.createString() + ")", prop);
    }

    // OperationProperty is unknown or unsupported: Warn and inform of all known properties
    void PopEDGenerator::warnOperationPropertyUnknown(OperationProperty *prop) {
        const std::vector<std::string> known_props = {"criterion","file","computeFIM","approximationFIM","E_family_value","E_family_calc_type","E_family_sampling","E_family_edsampling","diff_solver_method","abs_tol","rel_tol"};

        std::string name = prop->getName();
        TextFormatter form;
        form.openVector("", 0, ",");
        std::for_each(std::begin(known_props), std::end(known_props), [&](std::string x){ form.add("'" + x + "'"); });
        form.noFinalNewline();
        this->logger.warning("Property '" + prop->getName() +  "' unknown or unsupported (supported: " + form.createString() + ")", prop);
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
