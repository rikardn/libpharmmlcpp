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

#include "MDLGenerator.h"
#include <AST/AstTransformation.h>

namespace pharmmlcpp
{
    // private
    void MDLGenerator::setValue(std::string str) {
        this->value = str;
    }

    void MDLGenerator::setValue(std::vector<std::string> strs) {
        this->values = strs;
    }

    void MDLGenerator::setValue(stringpair pair) {
        this->hvalue = pair;
    }

    void MDLGenerator::setValue(stringmap hash) {
        this->hvalues = hash;
    }

    std::string MDLGenerator::accept(AstNode *node) {
        return this->ast_gen->acceptRoot(node);
    }

    // public
    MDLGenerator::MDLGenerator() {
        this->logger = std::make_shared<Logger>("MDL");

        std::unique_ptr<MDLAstGenerator> ast_gen(new MDLAstGenerator(this->logger));
        this->ast_gen = std::move(ast_gen);
        std::unique_ptr<MDLSymbols> symb_gen(new MDLSymbols(this->logger));
        this->symb_gen = std::move(symb_gen);
    }

    std::string MDLGenerator::getValue() {
        return this->value;
    }

    std::vector<std::string> MDLGenerator::getValues() {
        return this->values;
    }

    stringpair MDLGenerator::getPairValue() {
        return this->hvalue;
    }

    stringmap MDLGenerator::getHashValue() {
        return this->hvalues;
    }

    // Generators
    std::string MDLGenerator::generateModel(PharmML *model) {
        // Use the MDLSymbolNamer for symbol naming
        model->setSymbolNamer(&this->symbol_namer);

        // Store generated objects here
        MDLObjects objects;

        std::string name;

        // Generate the MDL data object(s)
        std::vector<ExternalDataset *> ext_dss = model->getTrialDesign()->getExternalDatasets();
        for (ExternalDataset *ext_ds : ext_dss) {
            MDLObject object;
            object.name = ext_ds->getOid(); // FIXME: getOid() is not optimal. Ideally, SymbolNamer should be able to accept oid objects as well (and generate names)!
            this->data_object_names.insert(object.name); // Store for selection by parameter object and linkage in MOG later
            object.code = this->genDataObj(ext_ds);
            objects.data.push_back(object);
        }

        // Generate the MDL parameter object(s)
        // TODO: Implement support for multiple parameter models as per schema
        //~ std::vector<ParameterModel *> par_models = model->getModelDefinition()->getParameterModels();
        //~ for (par_model : par_models) {
        ParameterModel *par_model = model->getModelDefinition()->getParameterModel();
        MDLObject object;
        object.name = par_model->getBlkId();
        ModellingSteps *msteps = model->getModellingSteps();
        object.code = this->genParObj(par_model, msteps);
        objects.parameter.push_back(object);
        //~ }


        // Generate the MDL model object(s)
        object.name = "mdl_object";
        object.code = this->genMdlObj(model);
        objects.model.push_back(object);

        // Generate the MDL task object(s)
        object.name = "task_object";
        object.code = this->genTaskObj();
        objects.task.push_back(object);

        // Generate the MDL design object(s)
        object.name = "design_obj";
        object.code = this->genDesignObj(model);
        if (!object.code.empty()) {
            objects.design.push_back(object);
        }

        // Generate the MDL mog object(s)
        object.name = "mog_obj";
        object.code = this->genMogObj(objects);
        objects.mog.push_back(object);

        // Output collection of MDL object(s)
        return this->genCompleteMDL(objects);
    }

    std::string MDLGenerator::genDataObj(ExternalDataset* ext_ds) {
        TextFormatter form;

        form.indentAdd("dataObj {");

        ext_ds->accept(this);
        form.addMany(this->getValue());

        form.outdentAdd("}");

        return form.createString();
    }

    std::string MDLGenerator::genDataInputVariablesBlock(Dataset *ds, std::vector<ColumnMapping *> col_maps, std::vector<std::string> &declared_vars) {
        // Fetch mappings from column id's to name of (model) symbols/macros (with data symbol codes if present) in string map
        std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> variables; // vec<col_id, vec<data_symbol, symbol/macro_name>>
        for (ColumnMapping *col_map : col_maps) {
            // Create entry for this column id
            std::string col_id = col_map->getColumnIdRef();
            std::vector<std::pair<std::string, std::string>> data_to_name;
            variables.emplace(col_id, data_to_name);

            // Get the mapping itself
            if (col_map->getMappedSymbol()) {
                // Column maps single symbol (w/o associated data symbol)
                Symbol *symbol = col_map->getMappedSymbol();
                data_to_name.push_back(std::make_pair("", symbol->getName()));
            } else if (col_map->getMappedMacro()) {
                // Column maps single macro (w/o associated data symbol)
                PKMacro *macro = col_map->getMappedMacro();
                data_to_name.push_back(std::make_pair("", macro->getName()));
            } else if (col_map->getTargetMapping()) {
                // Column maps multiple symbols/macros (w/ associated data symbols)
                TargetMapping *target_map = col_map->getTargetMapping();
                std::vector<MapType> maps = target_map->getMaps();
                for (MapType map : maps) {
                    if (map.symbol) {
                        data_to_name.push_back(std::make_pair(map.dataSymbol, map.symbol->getName()));
                    } else if (map.macro) {
                        data_to_name.push_back(std::make_pair(map.dataSymbol, map.macro->getName()));
                    }
                }
            }

            // Store the fetched mapping
            variables[col_id] = data_to_name;
        }

        // Parse mappings and create the DATA_INPUT_VARIABLES block for external datasets
        if (ds->isExternal()) {
            int num_cols = ds->getDefinition()->getNumColumns();
            // Create associative array of column types to column names (used to find column id containing dataSymbols refered by other column)
            std::unordered_map<std::string, std::vector<std::string>> columns_of_type;
            for (int num = 1; num <= num_cols; ++num) {
                ColumnDefinition *col_def = ds->getDefinition()->getColumnDefinition(num);
                std::string col_id = col_def->getId();
                std::string col_type = col_def->getType();
                auto got = columns_of_type.find(col_type);
                if (got == columns_of_type.end()) {
                    columns_of_type.emplace(col_type, std::vector<std::string>());
                }
                columns_of_type[col_type].push_back(col_id);
            }

            TextFormatter form;

            // Output one row for each column definition
            for (int num = 1; num <= num_cols; ++num) {
                // Get column id and type
                ColumnDefinition *col_def = ds->getDefinition()->getColumnDefinition(num);
                std::string col_id = col_def->getId();
                std::string col_type = col_def->getType();

                // Get the mapping entry for this column id (and the lone var name if present)
                std::vector<std::pair<std::string, std::string>> col_maps = variables[col_id];
                std::string lone_mapped_var;
                if (col_maps.size() == 1) {
                    lone_mapped_var = (col_maps[0].first == "") ? col_maps[0].second : "";
                }

                // Open vector with column id as header
                form.openVector(col_id + " : {}", 0, ", ");

                // Set type as MDL expects (not same keywords used as in PharmML), and set suffix
                std::string suffix = "::UNKNOWN"; // Suffix for DECLARED_VARIABLES block
                if (col_type == "undefined") {
                    // Likely means that this column is to be ignored, but are we sure?
                    form.add("use is ignore");
                } else if (col_type == "reg") {
                    // 'reg' stands for regressor which MDL uses 'covariate' for
                    form.add("use is covariate");
                    suffix = "::covariate"; // Unsure about this suffix
                } else if (col_type == "dose") {
                    form.add("use is amt");
                    suffix = "::doseTarget";
                } else if (col_type == "dv") {
                    suffix = "::observation";
                } else {
                    form.add("use is " + col_type);
                }

                // Add variable/define attribute for mapped model symbols/macros
                if (lone_mapped_var != "") {
                    // Add single-variable variable attribute (unless implicit rules)
                    if (!(col_type == "id" && lone_mapped_var == "ID") && !(col_type == "idv" && lone_mapped_var == "T") && !(col_type == "covariate" && lone_mapped_var == col_id)) {
                        form.add("variable = " + lone_mapped_var);
                        // Model symbol is declared elsewhere, let caller output DECLARED_VARIABLES block
                        declared_vars.push_back(lone_mapped_var + suffix);
                    }
                } else if (!col_maps.empty()) {
                    // Add define attribute for (multiple-variable) data symbol -> model symbol/macro
                    form.openVector("define = {}", 0, ", ");

                    // Find column name which is supposed to contain the data symbols (i.e. X in "define = {1 in X as TARGET_A, 2 in X as TARGET_B, ..}")
                    std::string data_symbol_column = "UNDEF";
                    if (col_type == "dose") {
                        // 'dose' columns seems to expect this to be of type 'adm' (often called 'CMT')
                        auto got = columns_of_type.find("adm");
                        if (got != columns_of_type.end()) {
                            this->logger->error("Dose column '" + col_id + "' maps multiple symbols but no 'adm' type column found (containing data symbols)", ds);
                        } else if (got->second.size() > 1) {
                            this->logger->error("Dose column '" + col_id + "' maps multiple symbols but multiple 'adm' type columns found (containing data symbols)", ds);
                        } else {
                            data_symbol_column = got->second[0];
                        }
                    } else if (col_type == "dv") {
                        // 'dv' columns seems to expect this to be of type 'dvid' (often called 'DVID')
                        // FIXME: MultipleDVMapping is often (always?) used instead of TargetMapping in the ColumnMapping itself!
                        auto got = columns_of_type.find("DVID");
                        if (got != columns_of_type.end()) {
                            this->logger->error("Observation column '" + col_id + "' maps multiple symbols but no 'dvid' type column found (containing data symbols)", ds);
                        } else if (got->second.size() > 1) {
                            this->logger->error("Observation column '" + col_id + "' maps multiple symbols but multiple 'dvid' type columns found (containing data symbols)", ds);
                        } else {
                            data_symbol_column = got->second[0];
                        }
                    }

                    // Add all the mappings to the define vector
                    for (std::pair<std::string, std::string> map: col_maps) {
                        form.add(map.first + " in " + data_symbol_column + " as " + map.second);
                         // Model symbol/macro is declared elsewhere, let caller output DECLARED_VARIABLES block
                        declared_vars.push_back(map.second + suffix);
                    }
                    form.closeVector();
                }

                form.closeVector();
            }

            return form.createString();
        } else {
            // Yes, what else?
            return "";
        }
    }

    std::string MDLGenerator::genParObj(ParameterModel *par_model, ModellingSteps *msteps) {
        TextFormatter form;

        form.indentAdd("parObj {");

        std::vector<PopulationParameter *> pop_params = par_model->getPopulationParameters();

        // Split into structural and variability parameters
        std::vector<PopulationParameter *> structural_params;
        std::vector<PopulationParameter *> variability_params;
        for (PopulationParameter *pop_param : pop_params) {
            std::vector<RandomVariable *> ref_rand_vars = par_model->getRandomVariables(pop_param);
            std::vector<IndividualParameter *> ref_ind_params = par_model->getIndividualParameters(pop_param);
            std::vector<Correlation *> ref_corrs = par_model->getCorrelations(pop_param);
            if (!ref_ind_params.empty()) {
                // Refered by IndividualParameter -> structural parameter
                structural_params.push_back(pop_param);
            }
            if (!ref_rand_vars.empty()) {
                // Refered by RandomVariable -> variability parameter
                variability_params.push_back(pop_param);
            }
            if (!ref_corrs.empty()) {
                // Refered by Correlation -> variability parameter
                for (Correlation *corr : ref_corrs) {
                    if (corr->isPairwise()) {
                        variability_params.push_back(pop_param);
                    } else {
                        pop_param->accept(this);
                        std::string name = this->getValue();
                        this->logger->error("Correlation refered by parameter '" + name + "' is of unsupported Matrix type", corr);
                        // TODO: Matrix support
                    }
                }
            }
            if (ref_rand_vars.empty() && ref_ind_params.empty() && ref_corrs.empty()) {
                // Refered by neither IP, RV or C -> assumed to be structural parameter (e.g. in ErrorModel in ObservationModel)
                pop_param->accept(this);
                std::string name = this->getValue();
                this->logger->warning("PopulationParameter '" + name + "' not associated with RandomVariable, IndividualParameter or Correlation; Assumed to be structural parameter", pop_param);
                structural_params.push_back(pop_param);
            }
        }

        // Get an EstimationStep to use (by matching to a data object generated)
        EstimationStep *selected_est_step = nullptr;
        if (msteps) {
            std::vector<EstimationStep *> est_steps = msteps->getEstimationSteps();
            if (!est_steps.empty()) {
                for (EstimationStep *est_step : est_steps) {
                    std::string data_object_reference = est_step->getExternalDatasetRef(); // FIXME: As pointed out in data object name fetch, this should be handled by SymbolNamer to avoid collisions and illegals
                    auto got = this->data_object_names.find(data_object_reference);
                    if (data_object_reference != "" && got != this->data_object_names.end()) {
                        if (selected_est_step == nullptr) {
                            selected_est_step = est_step;
                            this->selected_data_object = data_object_reference;
                        } else {
                            this->logger->warning("Another EstimationStep refers to a valid ExternalDataSet in model; First match selected (%a --> '" + this->selected_data_object + "')", selected_est_step);
                        }
                    }
                }
                if (selected_est_step == nullptr) {
                    this->logger->warning("ModellingSteps contains EstimationStep(s) but none refers to an ExternalDataSet in model; No initial values or bounds available", msteps);
                }
            } else {
                this->logger->error("ModellingSteps contains no EstimationStep; No initial values or bounds available", msteps);
            }
        } else {
            this->logger->error("No ModellingSteps found; No initial values or bounds available");
        }

        // Generate STRUCTURAL and VARIABILITY block
        form.addMany(this->genStructuralBlock(structural_params, selected_est_step));
        form.emptyLine();
        form.addMany(this->genVariabilityBlock(variability_params, selected_est_step));

        form.outdentAdd("}");

        return form.createString();
    }

    std::string MDLGenerator::genStructuralBlock(std::vector<PopulationParameter *> structural_params, EstimationStep *est_step) {
        // Generate MDL STRUCTURAL block
        TextFormatter form;
        form.indentAdd("STRUCTURAL {");

        for (PopulationParameter *param : structural_params) {
            param->accept(this);
            std::string name = this->getValue();
            this->structural_parameters.push_back(name);

            // Add the init attributes (if available)
            form.openVector(name + " : {}", 0, ", ");
            if (est_step) {
                ParameterEstimation *par_est = est_step->getParameterEstimation(param);
                if (par_est) {
                    par_est->accept(this);
                    std::vector<std::string> init_attr = this->getValues();
                    form.addMany(init_attr);
                } else {
                    this->logger->error("No ParameterEstimation for structural parameter '" + name + "' found in EstimationStep; No initial values or bounds available", est_step);
                }
            }

            form.closeVector();
        }

        form.outdentAdd("}");
        return form.createString();
    }

    std::string MDLGenerator::genVariabilityBlock(std::vector<PopulationParameter *> variability_params, EstimationStep *est_step) {
        // Generate MDL VARIABILITY block
        TextFormatter form;
        form.indentAdd("VARIABILITY {");

        for (PopulationParameter *param : variability_params) {
            param->accept(this);
            std::string name = this->getValue();
            this->variability_parameters.push_back(name);

            // Add the init attributes (if available)
            form.openVector(name + " : {}", 0, ", ");
            if (est_step) {
                ParameterEstimation *par_est = est_step->getParameterEstimation(param);
                if (par_est) {
                    par_est->accept(this);
                    std::vector<std::string> init_attr = this->getValues();
                    form.addMany(init_attr);
                } else {
                    this->logger->error("No ParameterEstimation for variability parameter '" + name + "' found in EstimationStep; No initial values or bounds available", est_step);
                }
            }

            form.closeVector();
        }

        form.outdentAdd("}");
        return form.createString();
    }

    std::string MDLGenerator::genMdlObj(pharmmlcpp::PharmML *model) {
        TextFormatter form;

        form.indentAdd("mdlObj {");
        ModelDefinition *mdef = model->getModelDefinition();

        // Generate IDV block
        model->getIndependentVariable()->accept(this);
        form.add("IDV {" + this->getValue() + "}");
        form.emptyLine();

        // Generate COVARIATES block
        form.openVector("COVARIATES {}", 1, "");
        std::vector<CPharmML::Covariate *> covs = model->getConsolidator()->getCovariates();
        for (CPharmML::Covariate *cov : covs) {
            std::string name = cov->getName();
            AstNode *assign = cov->getDefinition().get();
            form.add(name);
            if (assign) {
                form.append(" = " + this->accept(assign));
            }
        }
        form.closeVector();
        form.emptyLine();

        // Generate VARIABILITY_LEVELS block
        form.openVector("VARIABILITY_LEVELS {}", 1, "");
        std::vector<VariabilityModel *> var_models = mdef->getVariabilityModels();
        // Split into parameter and residual error variability model (TODO: This + warnings might be better in the libpharmmlcpp class)
        int level_num = 0;
        std::vector<VariabilityLevel *> par_levels;
        std::vector<VariabilityLevel *> err_levels;
        if (!var_models.empty()) {
            for (VariabilityModel *var_model : var_models) {
                if (var_model->onParameter()) {
                    if (par_levels.empty()) {
                        par_levels = var_model->getVariabilityLevelHierarchy();
                        level_num += par_levels.size();
                    } else {
                        // TODO: Make VariabilityModel a PharmMLSection
                        this->logger->warning("Another parameter variability model found: Only first will be used");
                    }
                } else if (var_model->onResidualError()) {
                    if (err_levels.empty()) {
                        err_levels = var_model->getVariabilityLevelHierarchy();
                        level_num += err_levels.size();
                    } else {
                        // TODO: Make VariabilityModel a PharmMLSection
                        this->logger->warning("Another residual error variability model found: Only first will be used");
                    }
                }
            }
        } else {
            // TODO: Make ModelDefinition a PharmMLSection
            this->logger->warning("ModelDefinition contains no VariabilityModel's: VARIABILITY_LEVELS absent");
        }
        // Output levels in hierarchial order with parameter levels before residual error levels
        // TODO: Allow collision/link names for Column's with names for VariabilityLevel's. Apparently MDL
        // likes these to share name (e.g. DV, ID) but in PharmML it's two different symbols altogether.
        // Also, FIXME: getName() override in VariabilityLevel of Symbol getName() (before someone gets hurt)!
        for (VariabilityLevel *level : par_levels) {
            // std::string name = level->getName(); // See above comment
            std::string name = level->getSymbId();
            form.openVector(name + " : {}", 0, ", ");
            form.add("level = " + std::to_string(level_num--));
            form.add("type is parameter");
            form.closeVector();
        }
        for (VariabilityLevel *level : err_levels) {
            // std::string name = level->getName(); // See above comment
            std::string name = level->getSymbId();
            form.openVector(name + " : {}", 0, ", ");
            form.add("level = " + std::to_string(level_num--));
            form.add("type is observation");
            form.closeVector();
        }
        form.closeVector();
        form.emptyLine();

        // Generate STRUCTURAL_PARAMETERS block
        form.openVector("STRUCTURAL_PARAMETERS {}", 1, "");
        form.addMany(this->structural_parameters);
        form.closeVector();
        form.emptyLine();

        // Generate VARIABILITY_PARAMETERS block
        form.openVector("VARIABILITY_PARAMETERS {}", 1, "");
        form.addMany(this->variability_parameters);
        form.closeVector();
        form.emptyLine();

        // Generate RANDOM_VARIABLE_DEFINITION blocks (for parameter variability)
        for (VariabilityLevel *var_level : par_levels) {
            std::vector<pharmmlcpp::RandomVariable *> rand_vars = mdef->getParameterModel()->getRandomVariablesOnLevel(var_level);
            std::vector<pharmmlcpp::Correlation *> corrs = mdef->getParameterModel()->getCorrelationsOnLevel(var_level);
            std::string block = this->genRandomVariableDefinitionBlock(var_level, rand_vars, corrs);
            form.addMany(block);
            form.emptyLine();
        }

        // Generate INDIVIDUAL_VARIABLES block
        std::vector<pharmmlcpp::IndividualParameter *> indiv_params = model->getModelDefinition()->getParameterModel()->getIndividualParameters();
        form.addMany(this->genIndividualVariablesBlock(indiv_params));
        form.emptyLine();

        // Generate MODEL_PREDICTION
        std::string model_pred = this->genModelPredictionBlock(model->getModelDefinition()->getStructuralModel());
        form.addMany(model_pred);
        form.emptyLine();

        // Generate RANDOM_VARIABLE_DEFINITION blocks (for residual error)
        for (VariabilityLevel *var_level : err_levels) {
            std::vector<pharmmlcpp::RandomVariable *> rand_vars = mdef->getParameterModel()->getRandomVariablesOnLevel(var_level);
            std::vector<pharmmlcpp::Correlation *> corrs = mdef->getParameterModel()->getCorrelationsOnLevel(var_level);
            std::string block = this->genRandomVariableDefinitionBlock(var_level, rand_vars, corrs);
            form.addMany(block);
            form.emptyLine();
        }

        // Generate OBSERVATION block
        std::string obs_block = this->genObservationBlock(model);
        form.addMany(obs_block);
        form.emptyLine();

        form.closeVector();
        form.outdentAdd("}");

        return form.createString();
    }

    std::string MDLGenerator::genRandomVariableDefinitionBlock(VariabilityLevel *level, std::vector<RandomVariable *> rand_vars, std::vector<Correlation *> corrs) {
        TextFormatter form;
        std::string name = level->getSymbId();
        form.openVector("RANDOM_VARIABLE_DEFINITION(level=" + name + ") {}", 1, "");

        for (RandomVariable *rand_var : rand_vars) {
            this->visit(rand_var);
            form.add(this->getValue());
        }
        for (Correlation *corr : corrs) {
            if (corr->isPairwise()) {
                form.openVector(":: {}", 0, ", ");

                std::vector<SymbRef *> symbRefs = corr->getPairwiseSymbRefs();
                form.add("rv1 = " + this->accept(symbRefs[0]) + ", rv2 = " + this->accept(symbRefs[1]));
                std::string type = corr->getPairwiseType();
                if (type == "CorrelationCoefficient") {
                    form.add("type is correlation");
                } else if (type == "Covariance") {
                    form.add("type is covariance");
                }
                form.add("value = " + this->accept(corr->getPairwiseAssignment().get()));

                form.closeVector();
            } else {
                form.add("# " + name + " correlation of unsupported matrix type");
            }
        }

        form.closeVector();
        return form.createString();
    }

    std::string MDLGenerator::genIndividualVariablesBlock(std::vector<pharmmlcpp::IndividualParameter *> individualParameters) {
        TextFormatter form;

        form.openVector("INDIVIDUAL_VARIABLES {}", 1, "");
        for (pharmmlcpp::IndividualParameter *ind_par : individualParameters) {
            this->visit(ind_par);
            form.add(this->getValue());
        }
        form.closeVector();

        return form.createString();
    }

    std::string MDLGenerator::genModelPredictionBlock(pharmmlcpp::StructuralModel *structuralModel) {
        // TODO: Consolidator for CommonVariable (Variable and DerivativeVariable)!
        TextFormatter form;
        form.openVector("MODEL_PREDICTION {}", 1, "");

        // Generate MDL COMPARTMENT block (if there's PKMacro's)
        pharmmlcpp::PKMacros *pk_macros = structuralModel->getPKMacros();
        if (pk_macros) {
            form.addMany(this->genCompartmentBlock(pk_macros));
        }

        // Get CommonVariable's (Variable's and Derivative's), sort and generate
        std::vector<CommonVariable *> vars = structuralModel->getVariables();
        SymbolSet var_set(std::unordered_set<Symbol *>(vars.begin(), vars.end()));
        for (Symbol *var : var_set.getOrdered()) {
            // FIXME: Think about MDLSymbols... Is it needed? It appears to ONLY be used here. Why?
            var->accept(this->symb_gen.get());
            form.addMany(symb_gen->getValue());
        }



        form.closeVector();
        return form.createString();
    }

    std::string MDLGenerator::genCompartmentBlock(pharmmlcpp::PKMacros *pk_macros) {
        TextFormatter form;

        // Get PKMacro's, sort and generate
        form.openVector("COMPARTMENT {}", 1, "");

        // Figure out max length to copy MDL spacing standard
        int max_length = 0;
        for (pharmmlcpp::PKMacro *macro : pk_macros->getMacros()) {
            int length = macro->getName().length();
            max_length = (length > max_length) ? length : max_length;
        }

        // Number administrations consecutively (depreceated MDL 'modelCmt' argument)
        int mdl_cmt_iterator = 1;
        std::unordered_map<pharmmlcpp::PKMacro *, int> mdl_cmt;
        std::vector<pharmmlcpp::PKMacro *> adm_macros = pk_macros->getAdministrations();
        for (pharmmlcpp::PKMacro *adm_macro : adm_macros) {
            if (adm_macro->getSubType() != MacroType::IV) {
                // IV are called "direct" in MDL and gets no number
                mdl_cmt[adm_macro] = mdl_cmt_iterator++;
            }
        }

        // Number compartments consecutively and build ordered list of compartments and mass transfers
        std::vector<pharmmlcpp::PKMacro *> cmt_macros = pk_macros->getCompartments();
        std::vector<pharmmlcpp::PKMacro *> trans_macros = pk_macros->getTransfers();
        std::vector<pharmmlcpp::PKMacro *> cmt_trans_macros; // To contain both of the above in good order
        for (pharmmlcpp::PKMacro *cmt_macro : cmt_macros) {
            mdl_cmt[cmt_macro] = mdl_cmt_iterator++;

            // Find transfers FROM this compartment
            int cmt_num = cmt_macro->getCmtNum();
            for (pharmmlcpp::PKMacro *trans_macro : trans_macros) {
                cmt_trans_macros.push_back(cmt_macro);
                if (cmt_num == trans_macro->getSourceNum()) {
                    cmt_trans_macros.push_back(trans_macro);
                }
            }
        }

        // Output administrations before all else
        for (pharmmlcpp::PKMacro *macro : adm_macros) {
            // Construct enclosure
            std::string name = macro->getName();
            std::string pad = std::string(max_length - name.length(), ' ');
            std::string prefix = name + pad;
            form.openVector(prefix + " : {}", 0, ", ");

            // Get target of administration (MDL uses the names when refering)
            int target_num = macro->getTargetNum();
            std::string target_name = pk_macros->getCompartment(target_num)->getName();

            // Add differently if depot-ish administration or direct (IV)
            if (macro->getSubType() != MacroType::IV) {
                form.add("type is depot");
                form.add("modelCmt=" + std::to_string(mdl_cmt[macro]));

                // Output compartment target
                form.add("to=" + target_name);

                // Get parameterization (and assume validation makes sure no strange combinations are present)
                AstNode *tlag = macro->getAssignment("Tlag").get();
                AstNode *p = macro->getAssignment("p").get();
                AstNode *tk0 = macro->getAssignment("Tk0").get();
                AstNode *ka = macro->getAssignment("ka").get();
                AstNode *ktr = macro->getAssignment("Ktr").get();
                AstNode *mtt = macro->getAssignment("Mtt").get();
                if (tlag) {
                    form.add("tlag=" + this->accept(tlag));
                }
                if (p) {
                    form.add("finput=" + this->accept(p));
                }
                if (tk0) {
                    form.add("tk0=" + this->accept(tk0));
                }
                if (ka) {
                    form.add("ka=" + this->accept(ka));
                }
                if (ktr) {
                    form.add("ktr=" + this->accept(ktr));
                }
                if (mtt) {
                    form.add("mtt=" + this->accept(mtt));
                }
            } else if (macro->getSubType() == MacroType::IV) {
                form.add("type is direct");

                // Output compartment target
                form.add("to=" + target_name);

                // Get parameterization
                AstNode *tlag = macro->getAssignment("Tlag").get();
                AstNode *p = macro->getAssignment("p").get();
                if (tlag) {
                    form.add("tlag=" + this->accept(tlag));
                }
                if (p) {
                    form.add("finput=" + this->accept(p));
                }
            }
            form.closeVector();
        }

        // Output compartments and mass transfers (in a order of transfers directly following associated compartment)
        for (pharmmlcpp::PKMacro *macro : cmt_trans_macros) {
            if (macro->isCompartment()) { // Treat all compartments similarly
                // Construct enclosure
                std::string name = macro->getName();
                std::string pad = std::string(max_length - name.length(), ' ');
                std::string prefix = name + pad;
                form.openVector(prefix + " : {}", 0, ", ");

                // Output type and compartment number
                std::string type = macro->getType();
                if (type == "Compartment") {
                    form.add("type is compartment");
                } else if (type ==  "Peripheral") {
                    form.add("type is distribution");
                } else if (type ==  "Effect") {
                    form.add("type is effect");
                }
                form.add("modelCmt=" + std::to_string(mdl_cmt[macro]));

                // Get parameterization
                AstNode *vol = macro->getAssignment("volume").get();
                AstNode *conc = macro->getAssignment("concentration").get();
                AstNode *ke0 = macro->getAssignment("ke0").get();
                if (vol) {
                    form.add("volume=" + this->accept(vol));
                }
                if (conc) {
                    form.add("conc=" + this->accept(conc));
                }
                if (ke0) {
                    form.add("ke0=" + this->accept(ke0));
                }
                // TODO: How should kij and k_i_j of Peripheral be treated? What are even i and j here?

                form.closeVector();
            } else if (macro->isMassTransfer()) {
                // Construct enclosure (mass transfers only have a colon as "name" in MDL)
                std::string pad = std::string(max_length - 1, ' ');
                std::string prefix = pad;
                form.openVector(prefix + " :: {}", 0, ", ");

                // Treat elimination and transfer the same
                if (macro->getSubType() == MacroType::Elimination) {
                    form.add("type is elimination");
                } else if (macro->getSubType() == MacroType::Transfer) {
                    form.add("type is transfer");
                }

                // In MDL with mass transfers, 'modelCmt' now refers to what compartment we're transfering from
                int source_num = macro->getSourceNum();
                pharmmlcpp::PKMacro *from_macro = pk_macros->getCompartment(source_num);
                form.add("modelCmt=" + std::to_string(mdl_cmt[from_macro]));

                // Output source compartment
                std::string from_name = pk_macros->getCompartment(source_num)->getName();
                form.add("from=" + from_name);

                // Output target compartment (for transfers only)
                if ( macro->hasAttribute("to") ) {
                    int target_num = macro->getTargetNum();
                    std::string target_name = pk_macros->getCompartment(target_num)->getName();
                    form.add("to=" + target_name);
                }

                // Get (linear) elimination parameterization
                AstNode *vol = macro->getAssignment("volume").get();
                AstNode *k = macro->getAssignment("k").get();
                AstNode *cl = macro->getAssignment("CL").get();
                AstNode *v = macro->getAssignment("V").get();
                if (vol) {
                    form.add("volume=" + this->accept(vol));
                }
                if (k) {
                    form.add("k=" + this->accept(k));
                }
                if (cl) {
                    form.add("cl=" + this->accept(cl));
                }
                if (v) {
                    form.add("v=" + this->accept(v));
                }

                // Get (MM) elimination parameterization
                AstNode *km = macro->getAssignment("Km").get();
                AstNode *vm = macro->getAssignment("Vm").get();
                if (km) {
                    form.add("km=" + this->accept(km));
                }
                if (vm) {
                    form.add("vm=" + this->accept(vm));
                }

                // Get transfer parameterization
                AstNode *from = macro->getAssignment("from").get();
                AstNode *to = macro->getAssignment("to").get();
                AstNode *kt = macro->getAssignment("kt").get();
                if (from) {
                    form.add("from=" + this->accept(from));
                }
                if (to) {
                    form.add("to=" + this->accept(to));
                }
                if (kt) {
                    form.add("kt=" + this->accept(kt));
                }

                form.closeVector();
            }
        }

        form.closeVector();
        return form.createString();
    }

    std::string MDLGenerator::genObservationBlock(pharmmlcpp::PharmML *model) {
        pharmmlcpp::ObservationModel *om = model->getModelDefinition()->getObservationModel();
        std::vector<pharmmlcpp::FunctionDefinition *> funcs = model->getFunctionDefinitions();
        TextFormatter form;
        form.openVector("OBSERVATION {}", 1, "");

        std::string obs_name = om->getName();
        if (om->hasStandardErrorModel()) {
            // Determine if error model is a pure function call
            AstNode *error_model = om->getErrorModel().get();
            this->ast_analyzer.reset();
            error_model->accept(&this->ast_analyzer);
            FunctionCall *function_call = this->ast_analyzer.getPureFunctionCall();
            if (function_call) {
                // Resolve the call
                FunctionDefinition *function_def = model->resolveFunctionCall(function_call);

                // Get the caller arguments
                auto &call_args = function_call->getFunctionArguments();
                std::unordered_map<std::string, FunctionArgument *> call_arg_map;
                for (const std::unique_ptr<FunctionArgument> &call_arg : call_args) {
                    call_arg_map[call_arg->getSymbId()] = call_arg.get(); // Note that SymbolNamer is NOT used since FA is not a Symbol (TODO: Use SymbolNamer?)
                }

                // Determine if function is known to MDL (tricky stuff)
                if (function_def->isStandardFunction()) {
                    form.openVector(obs_name + " : {}", 0, ", ");
                    switch (function_def->getStandardFunction()) {
                        case StandardFunction::additiveError     : form.add("type is additiveError");
                                                                   break;
                        case StandardFunction::proportionalError : form.add("type is proportionalError");
                                                                   break;
                        case StandardFunction::combinedError1    : form.add("type is combinedError1");
                                                                   break;
                        case StandardFunction::NA                : break;
                    }

                    // Get transformation is available (don't know MDL syntax definitely)
                    std::string trans = om->getTransformation();
                    if (trans == "log") {
                        form.add("trans is ln");
                    } else if (trans != "") {
                        form.add("trans is " + trans);
                    }

                    // Get structural model output and make a list of arguments referencing it
                    // FIXME: See below
                    //~ SymbRef *output = om->getOutput();
                    //~ std::vector<std::string> output_arg_names;

                    // Output the mapped arguments
                    auto def_arg_map = function_def->getStandardFunctionArgumentMap();
                    for (auto def_arg : def_arg_map) {
                        std::string standard_arg_name;
                        switch (def_arg.first) {
                            case StandardFunctionArgument::additive     : standard_arg_name = "additive";
                                                                          break;
                            case StandardFunctionArgument::proportional : standard_arg_name = "proportional";
                                                                          break;
                            case StandardFunctionArgument::prediction   : standard_arg_name = "prediction";
                                                                          break;
                        }
                        std::string actual_arg_name = def_arg.second->getName();

                        // Check if it's a prediction argument and output the argument in standardized form
                        FunctionArgument *call_arg = call_arg_map[actual_arg_name];
                        // FIXME: Figure out how to resolve this now when FunctionArgument has been demoted from Referer-status
                        //~ if (call_arg->referencedSymbols.dependsOn(output->getSymbol())) {
                            //~ output_arg_names.push_back(actual_arg_name);
                        //~ }
                        form.add(standard_arg_name + " = " + this->accept(call_arg->getArgument()));
                    }

                    // Add the residual error
                    form.add("eps = " + this->accept(om->getResidualError()));
                    form.closeVector();

                    // Warn if unexpected structure with regards to the output symbol
                    // FIXME: Same as above outcommented block
                    //~ if (output_arg_names.empty()) {
                        //~ this->logger->warning("Output from structural model (" + this->accept(output) + ") not in error model function call", om);
                    //~ } else if (output_arg_names.size() > 1) {
                        //~ this->logger->warning("Output from structural model (" + this->accept(output) + ") in multiple arguments "
                            //~ + TextFormatter::createInlineVector(output_arg_names, "()", ", ") + " of error model function call", om);
                    //~ }
                } else {
                    // TODO: Non-standard function call must be resolved
                    form.openVector(obs_name + " : {}", 0, ", ");
                    form.add("type is \"" + function_def->getName() + "\"");
                    form.closeVector();
                    form.append(" # Error function definition not recognized as MDL standard");
                }
            } else {
                // Not a pure function call, so dump explicit assignment
                AstNode *res_err = om->getResidualError();
                form.add(obs_name + " = " + this->accept(error_model) + " + " + this->accept(res_err));
                form.append(" # Is this how you expect the residual error to associate?");
            }
        } else if (om->hasGeneralErrorModel()) {
            // General error model, so dump explicit assignment
            AstNode *assignment = om->getAssignment().get();
            form.add(obs_name + " = " + this->accept(assignment));
        }

        form.closeVector();
        return form.createString();
    }

    std::string MDLGenerator::genTaskObj() {
        TextFormatter form;

        form.indentAdd("taskObj {");

        form.openVector("ESTIMATE {}", 1, "");
        form.add("set algo is saem");
        form.closeVector();

        form.outdentAdd("}");

        return form.createString();
    }

    std::string MDLGenerator::genDesignObj(PharmML *model) {
        TrialDesign *td = model->getTrialDesign();
        Arms *arms = nullptr;
        if (td) {
            arms = td->getArms();
        }
        // Do not create the object if no arms could be found.
        // This might be a too loose criteria for omission of the design object
        if (!arms) {
            return "";
        }

        TextFormatter form;

        form.indentAdd("designObj {");

        auto design_parameters = td->getDesignParameters();
        if (design_parameters.size() > 0) {
            genDesignParameters(form, design_parameters);
            form.emptyLine();
        }

        Observations *observations = td->getObservations();
        if (observations) {
            genDesignSampling(form, observations);
            form.emptyLine();
        }

        Interventions *interventions = td->getInterventions();
        if (interventions) {
            genDesignIntervention(form, interventions);
        }

        form.outdentAdd("}");

        return form.createString();
    }

    void MDLGenerator::genDesignParameters(TextFormatter &form, std::vector<DesignParameter *> designParameters) {
        form.indentAdd("DESIGN_PARAMETERS {");

        for (auto &param : designParameters) {
            this->visit(param);
            form.add(this->getValue());
        }

        form.outdentAdd("}");
    }

    void MDLGenerator::genDesignIntervention(TextFormatter &form, Interventions *interventions) {
        form.indentAdd("INTERVENTION {");

        // Administration
        for (Administration *administration : interventions->getAdministrations()) {
            std::string name = administration->getOid();
            std::string type = administration->getType();           // FIXME: Currently only supports bolus
            std::transform(type.begin(), type.end(), type.begin(), ::tolower);
            SymbRef *target_symbref = administration->getTargetSymbRef();
            std::string target = "UNDEF";
            if (target_symbref) {
                target = target_symbref->getSymbol()->getName();
            } else {
                TargetMapping *target_map = administration->getTargetMapping();
                std::vector<MapType> maps = target_map->getMaps();
                if (maps.size() > 1) {
                    // FIXME: Error here if more than one
                    this->logger->error("TargetMapping contains several references where only one is supported; Using first found", target_map);
                }
                // Pull single symbol or macro name
                if (maps[0].symbol) {
                    target = maps[0].symbol->getName();
                } else if (maps[0].macro) {
                    target = maps[0].macro->getName();
                }
            }
            std::string amount = this->accept(administration->getAmount().get());   // FIXME: Only support one amount
            std::vector<std::string> dose_times;
            for (auto &time_point : administration->getTimesAsVector()) {
                dose_times.push_back(this->accept(time_point.get()));
            }
            std::string doseTime = TextFormatter::createInlineVector(dose_times, "[]");

            form.add(name + " : { type is " + type + ", input=" + target + ", amount=" + amount + ", doseTime=" + doseTime + " }");
        }

        // Interventions combinations
        for (InterventionsCombination *combination : interventions->getInterventionsCombinations()) {
            std::string name = combination->getOid();
            SingleIntervention *si = combination->getSingleInterventions()[0];  // FIXME: Can MDL really handle more than one? Multiple starts?
            std::string start;
            if (si->getStart()) {
                std::vector<std::shared_ptr<AstNode>> start_vector = AstTransformation::toVector(si->getStart());
                std::vector<std::string> start_str;
                for (auto &s : start_vector) {
                    start_str.push_back(this->accept(s.get()));
                }
                start = TextFormatter::createInlineVector(start_str, ",start=[]");
            }
            std::string end;
            if (si->getEnd()) {
                std::vector<std::shared_ptr<AstNode>> end_vector = AstTransformation::toVector(si->getEnd());
                std::vector<std::string> end_str;
                for (auto &s : end_vector) {
                    end_str.push_back(this->accept(s.get()));
                }
                end = TextFormatter::createInlineVector(end_str, ",end=[]");
            }
            std::vector<std::string> oids;
            for (ObjectRef *objref : si->getOidRefs()) {
                oids.push_back(objref->getOidRef());
            }
            std::string combination_str = TextFormatter::createInlineVector(oids, "[]");

            // FIXME: Get parens round start if it is a vector with one element
            form.add(name + " : { type is combi, combination=" + combination_str + start + end + " }");
        }

        form.outdentAdd("}");
    }

    void MDLGenerator::genDesignSampling(TextFormatter &form, Observations *observations) {
        form.indentAdd("SAMPLING {");

        // FIXME: What does deltaTime, alq and blq convert to?
        // FIXME: xml line numbers for errors/warnings if present

        for (Observation *observation : observations->getObservations()) {
            std::string sampling = observation->getOid() + " : { type is simple, outcome=";
            // Do we have discrete variables?
            if (observation->getDiscreteVariables().size() > 0) {
                this->logger->warning("Discrete outcomes in observation not supported");
            }
            auto &continuous = observation->getContinuousVariables();
            if (continuous.size() == 0) {
                this->logger->error("No continuous outcomes in observation");
            }
            sampling += continuous[0]->getSymbIdRef() + ", ";

            if (observation->getNumberTimes()) {    // Either we have numberTimes or sampleTime
                sampling += "numberTimes=" + this->accept(observation->getNumberTimes().get());
            } else {
                auto times = observation->getObservationTimesAsVector();
                std::vector<std::string> samples;
                for (auto &time_point : times) {
                    samples.push_back(this->accept(time_point.get()));
                }
                sampling += TextFormatter::createInlineVector(samples, "sampleTime=[]");
            }
            sampling += " }";
            form.add(sampling);
        }

        form.outdentAdd("}");
    }

    std::string MDLGenerator::genMogObj(MDLObjects &objects) {
        TextFormatter form;

        form.indentAdd("mogObj {");
        form.openVector("OBJECTS {}", 1, "");

        // Object selection
        std::vector<std::pair<std::string, std::vector<MDLObject>>> typed_objects({
            {"dataObj", objects.data},
            {"parObj", objects.parameter},
            {"mdlObj", objects.model},
            {"taskObj", objects.task},
            {"designObj", objects.design},
        });
        for (auto it = typed_objects.begin(); it != typed_objects.end(); ++it) {
            std::string type = (*it).first;
            std::vector<MDLObject> objs = (*it).second;
            if (objs.empty()) {
                this->logger->warning("No objects of type '" + type + "' for Modelling Object Group generated");
            } else {
                form.add(objs[0].name + " : { type is " + type + " }");
                if (objs.size() > 1) {
                    this->logger->warning("Multiple objects of type '" + type + "' generated: Only one set active in Modelling Object Group");
                    for (auto it = objs.begin() + 1; it != objs.end(); ++it) {
                        form.add("# " + (*it).name + " : { type is " + type + " }");
                    }
                }
            }
        }

        form.closeVector();
        form.outdentAdd("}");

        return form.createString();
    }

    std::string MDLGenerator::genCompleteMDL(MDLObjects &objects) {
        TextFormatter form;

        // Output all objects
        std::vector<std::pair<std::string, std::vector<MDLObject>>> typed_objects({
            {"dataObj", objects.data},
            {"parObj", objects.parameter},
            {"mdlObj", objects.model},
            {"taskObj", objects.task},
            {"mogObj", objects.mog},
            {"designObj", objects.design},
        });
        for (auto it = typed_objects.begin(); it != typed_objects.end(); ++it) {
            std::string type = (*it).first;
            std::vector<MDLObject> objs = (*it).second;
            for (MDLObject obj : objs) {
                form.addMany(obj.name + " = " + obj.code);
                form.emptyLine();
            }
        }

        return form.createString();
    }

    // MDL visitors
    void MDLGenerator::visit(FunctionDefinition *node) {
        TextFormatter form;

        std::string name = node->getName();
        form.openVector("FUNCTION(" + name + "){}", 1, "");
        form.addMany(this->accept(node->getDefinition().get()));
        form.closeVector();

        this->setValue(form.createString());
    }

    void MDLGenerator::visit(FunctionArgumentDefinition *node) { }

    void MDLGenerator::visit(Covariate *node) { }

    void MDLGenerator::visit(PopulationParameter *node) {
        setValue(node->getName());
    }

    void MDLGenerator::visit(IndividualParameter *node) {
        TextFormatter form;

        // Get name
        std::string name = node->getName();

        if (node->isStructured()) {
            form.openVector(name + " : {}", 0, ", ");

            // Get type
            if (node->isLinear()) {
                form.add("type is linear");
            } else if (node->isGeneral()) {
                form.add("type is general");
            }

            // Get transformation (both sides)
            std::string trans = node->getTransformation();
            if (trans == "") {
            } else if (trans == "log") {
                form.add("trans is ln");
            } else {
                form.add("trans is " + trans);
            }

            if (node->isLinear()) {
                 // Get population value (how is 'grp' with 'general' in MDL translated to PharmML?)
                std::string pop = this->accept(node->getPopulationValue().get());
                form.add("pop = " + pop);

                // Get covariates and fixed effects
                std::vector<std::string> fix_effs;
                for (SymbRef *covariate : node->getCovariates()) {
                    std::vector<std::string> coeffs;
                    for (FixedEffect *fix_eff : node->getFixedEffects(covariate)) {
                        std::string coeff;
                        if (fix_eff->getReference()) {
                            coeff = this->accept(fix_eff->getReference());
                        } else {
                            coeff = this->accept(fix_eff->getScalar().get());
                        }
                        coeffs.push_back(coeff);
                    }
                    if (coeffs.size() == 1) {
                        fix_effs.push_back("{coeff=" + coeffs[0] + ",cov=" + this->accept(covariate) + "}");
                    } else if (coeffs.size() > 1) {
                        fix_effs.push_back("{coeff=" + TextFormatter::createInlineVector(coeffs, "[]", ",") + ",cov=" + this->accept(covariate) + "}");
                    }
                }
                if (fix_effs.size() == 1) {
                    form.add("fixEff = " + fix_effs[0]);
                } else if (fix_effs.size() > 1) {
                    form.add("fixEff = " + TextFormatter::createInlineVector(fix_effs, "[]", ", "));
                }
            }

            // Get random effects
            std::vector<std::string> rands;
            for (SymbRef * rand : node->getRandomEffects()) {
                rands.push_back(this->accept(rand));
            }
            if (rands.size() == 1) {
                form.add("ranEff = " + rands[0]);
            } else if (rands.size() > 1) {
                form.add("ranEff = " + TextFormatter::createInlineVector(rands, "[]", ", "));
            }
        } else {
            std::string assign = this->accept(node->getAssignment().get());
            form.add(name + " = " + assign);
        }

        form.noFinalNewline();
        this->setValue(form.createString());
    }

    void MDLGenerator::visit(RandomVariable *node) {
        TextFormatter form;

        // Get name of random variable and associated distribution
        std::string name = node->getName();
        pharmmlcpp::Distribution *dist = node->getDistribution();

        // Try to handle Normal1/2 (stdev/var) of ProbOnto and warn if model steps outside
        std::string dist_name = dist->getName();
        std::vector<pharmmlcpp::DistributionParameter *> dist_params = dist->getDistributionParameters();
        if (dist_name == "Normal1" || dist_name == "Normal2") {
            form.openVector(name + " ~ Normal()", 0, ", ");
            std::vector<std::string> unknown_param_types;
            for (pharmmlcpp::DistributionParameter *dist_param : dist_params) {
                std::string name = dist_param->getName();
                std::string assign = this->accept(dist_param->getAssignment().get());
                if (name == "mean") {
                    form.add("mean = " + assign);
                } else if (name == "stdev") {
                    form.add("sd = " + assign);
                } else if (name == "var") {
                    form.add("var = " + assign);
                } else {
                    unknown_param_types.push_back(name);
                }
            }
            form.closeVector();
            if (!unknown_param_types.empty()) {
                form.append(" # Unknown ProbOnto " + dist_name + " parameter type (" + form.createInlineVector(unknown_param_types, "", ", ") + ")!");
            }
        } else {
            form.add(name + " # Unknown ProbOnto distribution (" + dist_name + ")!");
        }

        form.noFinalNewline();
        this->setValue(form.createString());
    }

    void MDLGenerator::visit(VariabilityLevel *node) {
        // this->setValue(node->getName());
    }

    void MDLGenerator::visit(Correlation *node) {
        TextFormatter form;

        std::vector<std::string> attr;
        if (node->isPairwise()) {
            attr.push_back("value = " + this->accept(node->getPairwiseAssignment().get()));
        } else {
            // TODO: Matrix support
        }

        this->setValue(attr);
    }

    void MDLGenerator::visit(IndependentVariable *node) {
        this->setValue(node->getName());
    }

    void MDLGenerator::visit(Variable *node) {
        if (node->getAssignment()) {
            this->setValue(node->getName() + " = " + this->accept(node->getAssignment().get()));
        } else {
            this->setValue(node->getName());
        }
    }

    void MDLGenerator::visit(DerivativeVariable *node) {
        TextFormatter form;

        std::string name = node->getName();
        form.openVector(name + " : {}", 0, ", ");
        form.add("deriv = " + this->accept(node->getAssignment().get()));
        form.add("init = " + this->accept(node->getInitialValue().get()));
        form.add("x0 = " + this->accept(node->getInitialTime().get()));
        form.closeVector();

        this->setValue(form.createString());
    }

    void MDLGenerator::visit(ObservationModel *node) { }

    void MDLGenerator::visit(Distribution *node) { }

    void MDLGenerator::visit(ColumnMapping *node) {
        std::string id = node->getColumnIdRef();
        std::string name = "UNDEF";
        if (node->getMappedSymbol()) {
            name = node->getMappedSymbol()->getName();
        } else if (node->getMappedMacro()) {
            name = node->getMappedMacro()->getName();
        }
        stringpair pair = {id, name};
        this->setValue(pair);
    }

    // Class ExternalFile (this class might be superfluous)
    void MDLGenerator::visit(ExternalFile *node) { }

    // Class DataColumn
    void MDLGenerator::visit(DataColumn *node) { }

    // Class Dataset
    void MDLGenerator::visit(Dataset *node) { }

    // Class TargetMapping
    void MDLGenerator::visit(TargetMapping *node) { }

    // Class ExternalDataset
    void MDLGenerator::visit(ExternalDataset *node) {
        TextFormatter form;
        std::string tool = node->getToolName();

        // Warn if encoding style is not "NONMEM" (TODO: Figure out heuristics for other encoding style's, such as "Monolix")
        if (tool != "NONMEM") {
            this->logger->warning("ExternalDataset refers tool '" + tool + "' instead of NONMEM: Heuristics may be wrong", node);
        }

        // Generate array of mapping targets (to be trimmed before output)
        // std::vector<std::string, std::vector<std::pair<std::string, Symbol *>>> mappings; // vector<col_id, vector<pair<data_symbol, symbol/macro>>>
        std::vector<ColumnMapping *> col_maps = node->getColumnMappings();
        // for (ColumnMapping *col_map : col_maps) {
        //     std::string id = col_map->getColumnIdRef();
        //     std::vector<std::pair<std::string, Symbol *>> data_to_symbols;
        //     if (col_map->getMappedSymbol()) {
        //         Symbol *symbol = col_map->getMappedSymbol();
        //
        //     } else if (col_map->getMappedMacro()) {
        //         Symbol *symbol = col_map->getMappedMacro();
        //     }
        //     stringpair pair = {id, name};
        //     mappings.insert(pair);
        // }

        Dataset *dataset = node->getDataset();
        if (dataset->isExternal()) {
            // Generate DATA_INPUT_VARIABLES and output DECLARED_VARIABLES
            std::vector<std::string> declared_vars;
            std::string data_input_vars = this->genDataInputVariablesBlock(dataset, col_maps, declared_vars);
            if (!declared_vars.empty()) {
                // Output pruned and formatted map from genDataInputVariablesBlock
                form.openVector("DECLARED_VARIABLES {}", 0, " ");
                form.addMany(declared_vars);
                form.closeVector();
                form.add("");
            }

            // Output DATA_INPUT_VARIABLES
            form.openVector("DATA_INPUT_VARIABLES {}", 1, "");
            form.addMany(data_input_vars);
            form.closeVector();
            form.add("");

            // Generate SOURCE
            form.openVector("SOURCE {}", 1, "");
            ExternalFile *file = dataset->getExternal();
            form.add("# Name: \"" + file->getOid() +
                "\", type: " + file->getFormat() +
                ", delimiter: \"" + file->getDelimiter() + "\"");
            form.openVector("srcfile : {}", 1, ", ");
            form.add("file = \"" + file->getPath() + "\"");
            form.add("inputFormat is nonmemFormat"); // TODO: What would change this?
            form.closeVector();
            form.closeVector();
        } else {
            this->logger->error("Table as opposed to external resource not supported in ExternalDataset", dataset);
            form.add("# No external dataset file");
        }

        this->setValue(form.createString());
    }

    // Class Interventions and all its content
    void MDLGenerator::visit(Administration *node) { }

    void MDLGenerator::visit(IndividualAdministration *node) { }

    void MDLGenerator::visit(Interventions *node) { }

    // Class Observations and all its content
    void MDLGenerator::visit(Observation *node) { }

    void MDLGenerator::visit(IndividualObservations *node) { }

    void MDLGenerator::visit(ObservationCombination *node) { }

    void MDLGenerator::visit(Observations *node) { }

    // Class Arms and all its contents
    void MDLGenerator::visit(InterventionSequence *node) { }

    void MDLGenerator::visit(ObservationSequence *node) { }

    void MDLGenerator::visit(OccasionSequence *node) { }

    void MDLGenerator::visit(Arm *node) { }

    void MDLGenerator::visit(Arms *node) { }

    // Class DesignSpaces and all its content
    void MDLGenerator::visit(DesignSpace *node) { }

    void MDLGenerator::visit(DesignSpaces *node) { }

    // Class ParameterEstimation
    void MDLGenerator::visit(ParameterEstimation *node) {
        std::vector<std::string> attr;
        if (node->hasInitValue()) {
            attr.push_back("value = " + this->accept(node->getInitValue().get()));
        }
        if (node->hasLoBound()) {
            attr.push_back("lo = " + this->accept(node->getLoBound().get()));
        }
        if (node->hasHiBound()) {
            attr.push_back("hi = " + this->accept(node->getHiBound().get()));
        }
        if (node->isFixed()) {
            attr.push_back("fix = true");
        }
        this->setValue(attr);
    }

    // Class PKMacro
    void MDLGenerator::visit(PKMacro *node) { }
}
