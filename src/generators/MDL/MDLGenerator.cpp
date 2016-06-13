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
        node->accept(this->ast_gen.get());
        return ast_gen->getValue();
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
    std::string MDLGenerator::generateModel(Model *model) {
        // FIXME: Bad design to put in model here? A smell of visitor pattern breakdown. Solution might be visitor on Model level.

        // Store generated objects here
        MDLObjects objects;

        std::string name;

        // Generate the MDL data object(s)
        std::vector<ExternalDataset *> ext_dss = model->getTrialDesign()->getExternalDatasets();
        for (ExternalDataset *ext_ds : ext_dss) {
            MDLObject object;
            object.name = ext_ds->getOid();
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
        std::vector<EstimationStep *> estim_steps = model->getModellingSteps()->getEstimationSteps();
        //~ form.addMany(name + " = " + this->genParObj(par_model, estim_steps));
        CPharmML::PopulationParameters *populationParameters = model->getConsolidator()->getPopulationParameters(); // TODO: Plurality support for multiple parameter models
        object.code = this->genParObj(populationParameters);
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

    std::string MDLGenerator::genDataInputVariablesBlock(Dataset *node, stringmap &column_mappings) {
        // TODO: Consolidator here
        // TODO: idv only implicit if indpendent variable is T I believe
        stringmap implicit_mappings = {{"id", "ID"}, {"idv", "T"}};
        if (node->isExternal()) {
            TextFormatter form;

            // Output one row for each column definition
            int num_cols = node->getDefinition()->getNumColumns();
            for (int num = 1; num <= num_cols; ++num) {
                // Get column id
                ColumnDefinition *col_def = node->getDefinition()->getColumnDefinition(num);
                std::string id = col_def->getId();

                // Open vector with column id as header
                form.openVector(id + " : {}", 0, ", ");

                // Set type as MDL expects
                std::string type = col_def->getType();
                if (type == "undefined") {
                    type = "ignore";
                } else if (type == "reg") {
                    type = "covariate";
                } else if (type == "dose") {
                    type = "amt";
                }
                form.add("use is " + type);

                // Prune column map for caller
                if (type == "covariate" && id == column_mappings[id]) {
                    // Trim column_mappings to not contain implicit (same-name) covariate mappings
                    column_mappings.erase(id);
                } else {
                    // Find match in column mappings and add it (if found and not implicit)
                    auto got_map = column_mappings.find(id);
                    if (got_map != column_mappings.end()) {
                        if (column_mappings[id] != implicit_mappings[type]) {
                            form.add("variable = " + column_mappings[id]);
                        } else {
                            // Trim column_mappings to only contain maps required by MDL
                            column_mappings.erase(id);
                        }
                    }
                }

                // Add type value to caller map
                if (type == "amt") {
                    column_mappings[id] = column_mappings[id] + "::dosingTarget";
                } else if (type == "dv") {
                    // FIXME: Consolidate the map with the targets (to determine type)
                    column_mappings[id] = column_mappings[id] + "::continuousObs";
                }

                form.closeVector();
            }

            return form.createString();
        } else {
            // Yes, what else?
            return "";
        }
    }

    std::string MDLGenerator::genParObj(CPharmML::PopulationParameters *populationParameters) {
        TextFormatter form;

        form.indentAdd("parObj {");

        std::vector<CPharmML::PopulationParameter *> cpop_params = populationParameters->getPopulationParameters();
        // Split into structural and variability parameters
        std::vector<CPharmML::PopulationParameter *> structuralParameters;
        std::vector<CPharmML::PopulationParameter *> variabilityParameters;
        std::vector<std::string> correlatedVariables;
        for (CPharmML::PopulationParameter *cpop_param : cpop_params) {
            if (cpop_param->isVariabilityParameter()) {
                variabilityParameters.push_back(cpop_param);
            } else if (cpop_param->isCorrelation()) {
                variabilityParameters.push_back(cpop_param);
                // Get correlated variable names
                std::vector<SymbRef *> symbRefs = cpop_param->getCorrelation()->getPairwiseSymbRefs();
                correlatedVariables.push_back(this->accept(symbRefs[0]));
                correlatedVariables.push_back(this->accept(symbRefs[1]));
            } else {
                structuralParameters.push_back(cpop_param);
            }
        }

        // Fill DECLARED_VARIABLES with correlated variable names
        if (!correlatedVariables.empty()) {
            form.openVector("DECLARED_VARIABLES {}", 0, " ");
            for (std::string corr_name : correlatedVariables) {
                form.add(corr_name);
            }
            form.closeVector();
            form.emptyLine();
        }

        // Generate STRUCTURAL and VARIABILITY block
        form.addMany(this->genStructuralBlock(structuralParameters));
        form.emptyLine();
        form.addMany(this->genVariabilityBlock(variabilityParameters));

        form.outdentAdd("}");

        return form.createString();
    }

    std::string MDLGenerator::genStructuralBlock(std::vector<CPharmML::PopulationParameter *> structuralParameters) {
        // Generate MDL STRUCTURAL block
        TextFormatter form;
        form.indentAdd("STRUCTURAL {");

        for (CPharmML::PopulationParameter *structuralParameter : structuralParameters) {
            // TODO: Implement CPharmMLVisitor (instead of visiting the pharmmlcpp::PopulationParameter objects, which is better suited for model object)
            structuralParameter->getPopulationParameter()->accept(this);
            std::string name = this->getValue();
            this->structuralParameterNames.push_back(name);

            // Add the init attributes
            structuralParameter->getParameterEstimation()->accept(this);
            std::vector<std::string> init_attr = this->getValues();
            form.openVector(name + " : {}", 0, ", ");
            form.addMany(init_attr);

            form.closeVector();
        }

        form.outdentAdd("}");
        return form.createString();
    }

    std::string MDLGenerator::genVariabilityBlock(std::vector<CPharmML::PopulationParameter *> variabilityParameters) {
        // Generate MDL VARIABILITY block
        TextFormatter form;
        form.indentAdd("VARIABILITY {");

        for (CPharmML::PopulationParameter *variabilityParameter : variabilityParameters) {
            // TODO: Implement CPharmMLVisitor (instead of visiting the pharmmlcpp::PopulationParameter objects, which is better suited for model object)
            if (variabilityParameter->isCorrelation()) {
                // Correlations
                pharmmlcpp::Correlation *corr = variabilityParameter->getCorrelation();
                std::string name = variabilityParameter->getName();
                if (corr->isPairwise()) {
                    std::string value = this->accept(corr->getPairwiseAssignment());
                    form.add(name + " : {value = " + value + "}");
                    this->variabilityParameterNames.push_back(name); // TODO: Fix this ugly global variable
                } else {
                    this->logger->error("Correlation '" + name + "' is of unsupported Matrix type", corr);
                    form.add("# " + name + " correlation of unsupported matrix type");
                    // TODO: Matrix support
                }
            } else {
                // Ordinary variability parameters
                variabilityParameter->getPopulationParameter()->accept(this);
                std::string name = this->getValue();
                this->variabilityParameterNames.push_back(name);

                // Add the init attributes
                variabilityParameter->getParameterEstimation()->accept(this);
                std::vector<std::string> init_attr = this->getValues();
                form.openVector(name + " : {}", 0, ", ");
                form.addMany(init_attr);

                // Try to handle Normal1/2 (stdev/var) of ProbOnto and warn if model steps outside
                std::string dist_name = variabilityParameter->getDistributionName();
                std::string dist_param = variabilityParameter->getDistributionParameterType();
                std::string comment;
                if (variabilityParameter->inDifferentParameterizations()) {
                    comment = " # Parameter in different distributions/parameterizations!";
                } else {
                    if (dist_name == "Normal1" || dist_name == "Normal2") {
                        if (dist_param == "stdev") {
                            form.add("type is sd");
                        } else if (dist_param == "var") {
                            form.add("type is var");
                        } else {
                            comment = " # Unknown ProbOnto " + dist_name + " parameter type (" + dist_param + ")!";
                        }
                    } else {
                        comment = " # Unknown ProbOnto distribution (" + dist_name + ") and parameter type (" + dist_param + ")!";
                    }
                }
                form.closeVector();
                form.append(comment);
            }
        }

        form.outdentAdd("}");
        return form.createString();
    }

    std::string MDLGenerator::genMdlObj(pharmmlcpp::Model *model) {
        TextFormatter form;

        form.indentAdd("mdlObj {");

        // Generate IDV block
        model->getIndependentVariable()->accept(this);
        form.add("IDV {" + this->getValue() + "}");
        form.emptyLine();

        // Generate COVARIATES block
        form.openVector("COVARIATES {}", 1, "");
        std::vector<CPharmML::Covariate *> covs = model->getConsolidator()->getCovariates();
        for (CPharmML::Covariate *cov : covs) {
            std::string name = cov->getName();
            AstNode *assign = cov->getDefinition();
            form.add(name);
            if (assign) {
                form.append(" = " + this->accept(assign));
            }
        }
        form.closeVector();
        form.emptyLine();

        // Generate VARIABILITY_LEVELS block
        form.openVector("VARIABILITY_LEVELS {}", 1, "");
        std::vector<pharmmlcpp::VariabilityLevel *> par_levels = model->getConsolidator()->getVariabilityModels()->getParameterLevelChain();
        std::vector<pharmmlcpp::VariabilityLevel *> err_levels = model->getConsolidator()->getVariabilityModels()->getResidualErrorLevelChain();
        std::vector<int>::size_type level = par_levels.size() + err_levels.size();
        for (; level - err_levels.size() > 0; level--) {
            std::string name = par_levels[level - err_levels.size() - 1]->getSymbId();
            form.openVector(name + " : {}", 0, ", ");
            form.add("level = " + std::to_string(level));
            form.add("type is parameter");
            form.closeVector();
        }
        for (; level > 0; level--) {
            std::string name = err_levels[level - 1]->getSymbId();
            form.openVector(name + " : {}", 0, ", ");
            form.add("level = " + std::to_string(level));
            form.add("type is observation");
            form.closeVector();
        }
        form.closeVector();
        form.emptyLine();

        // Generate STRUCTURAL_PARAMETERS block
        form.openVector("STRUCTURAL_PARAMETERS {}", 1, "");
        form.addMany(this->structuralParameterNames);
        form.closeVector();
        form.emptyLine();

        // Generate VARIABILITY_PARAMETERS block
        form.openVector("VARIABILITY_PARAMETERS {}", 1, "");
        form.addMany(this->variabilityParameterNames);
        form.closeVector();
        form.emptyLine();

        // Generate RANDOM_VARIABLE_DEFINITION blocks (for parameter variability)
        for (auto it = par_levels.rbegin(); it != par_levels.rend(); ++it) {
            std::vector<pharmmlcpp::RandomVariable *> random_vars = model->getConsolidator()->getVariabilityModels()->getRandomVariablesOnLevel(*it);
            std::vector<pharmmlcpp::Correlation *> corrs = model->getConsolidator()->getVariabilityModels()->getCorrelationsOnLevel(*it);
            std::vector<CPharmML::PopulationParameter *> cpop_corrs = model->getConsolidator()->getPopulationParameters()->getPopulationParameters(corrs);
            std::string block = this->genRandomVariableDefinitionBlock(*it, random_vars, cpop_corrs);
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
        for (auto it = err_levels.rbegin(); it != err_levels.rend(); ++it) {
            std::vector<pharmmlcpp::RandomVariable *> random_vars = model->getConsolidator()->getVariabilityModels()->getRandomVariablesOnLevel(*it);
            std::vector<pharmmlcpp::Correlation *> corrs = model->getConsolidator()->getVariabilityModels()->getCorrelationsOnLevel(*it);
            std::vector<CPharmML::PopulationParameter *> cpop_corrs = model->getConsolidator()->getPopulationParameters()->getPopulationParameters(corrs);
            std::string block = this->genRandomVariableDefinitionBlock(*it, random_vars, cpop_corrs);
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

    std::string MDLGenerator::genRandomVariableDefinitionBlock(pharmmlcpp::VariabilityLevel *level, std::vector<pharmmlcpp::RandomVariable *> random_vars, std::vector<CPharmML::PopulationParameter *> cpop_corrs) {
        TextFormatter form;
        this->visit(level);
        std::string name = getValue();
        form.openVector("RANDOM_VARIABLE_DEFINITION(level=" + name + ") {}", 1, "");

        for (pharmmlcpp::RandomVariable *random_var : random_vars) {
            this->visit(random_var);
            form.add(this->getValue());
        }
        for (CPharmML::PopulationParameter *cpop_corr : cpop_corrs) {
            std::string name = cpop_corr->getName();

            pharmmlcpp::Correlation *corr = cpop_corr->getCorrelation();
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
                form.add("value = " + name);

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
                AstNode *tlag = macro->getAssignment("Tlag");
                AstNode *p = macro->getAssignment("p");
                AstNode *tk0 = macro->getAssignment("Tk0");
                AstNode *ka = macro->getAssignment("ka");
                AstNode *ktr = macro->getAssignment("Ktr");
                AstNode *mtt = macro->getAssignment("Mtt");
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
                AstNode *tlag = macro->getAssignment("Tlag");
                AstNode *p = macro->getAssignment("p");
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
                AstNode *vol = macro->getAssignment("volume");
                AstNode *conc = macro->getAssignment("concentration");
                AstNode *ke0 = macro->getAssignment("ke0");
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
                AstNode *vol = macro->getAssignment("volume");
                AstNode *k = macro->getAssignment("k");
                AstNode *cl = macro->getAssignment("CL");
                AstNode *v = macro->getAssignment("V");
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
                AstNode *km = macro->getAssignment("Km");
                AstNode *vm = macro->getAssignment("Vm");
                if (km) {
                    form.add("km=" + this->accept(km));
                }
                if (vm) {
                    form.add("vm=" + this->accept(vm));
                }

                // Get transfer parameterization
                AstNode *from = macro->getAssignment("from");
                AstNode *to = macro->getAssignment("to");
                AstNode *kt = macro->getAssignment("kt");
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

    std::string MDLGenerator::genObservationBlock(pharmmlcpp::Model *model) {
        pharmmlcpp::ObservationModel *om = model->getModelDefinition()->getObservationModel();
        std::vector<pharmmlcpp::FunctionDefinition *> funcs = model->getFunctionDefinitions();
        TextFormatter form;
        form.openVector("OBSERVATION {}", 1, "");

        std::string obs_name = om->getSymbId();
        if (om->hasStandardErrorModel()) {
            // Determine if error model is a pure function call
            AstNode *error_model = om->getErrorModel();
            this->ast_analyzer.reset();
            error_model->accept(&this->ast_analyzer);
            FunctionCall *function_call = this->ast_analyzer.getPureFunctionCall();
            if (function_call) {
                // Resolve the call
                FunctionDefinition *function_def = model->resolveFunctionCall(function_call);

                // Get the caller arguments
                std::vector<FunctionArgument *> call_args = function_call->getFunctionArguments();
                std::unordered_map<std::string, FunctionArgument *> call_arg_map;
                for (FunctionArgument *call_arg : call_args) {
                    call_arg_map[call_arg->getSymbId()] = call_arg;
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
                        std::string actual_arg_name = def_arg.second->getSymbId();

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
                    form.add("type is \"" + function_def->getSymbId() + "\"");
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
            AstNode *assignment = om->getAssignment();
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

    std::string MDLGenerator::genMogObj(MDLObjects &objects) {
        TextFormatter form;

        form.indentAdd("mogObj {");
        form.openVector("OBJECTS {}", 1, "");

        // Object selection
        std::vector<std::pair<std::string, std::vector<MDLObject>>> typed_objects({
            {"dataObj", objects.data},
            {"parObj", objects.parameter},
            {"mdlObj", objects.model},
            {"taskObj", objects.task}
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

        std::string name = node->getSymbId();
        form.openVector("FUNCTION(" + name + "){}", 1, "");
        form.addMany(this->accept(node->getDefinition()));
        form.closeVector();

        this->setValue(form.createString());
    }

    void MDLGenerator::visit(FunctionArgumentDefinition *node) { }

    void MDLGenerator::visit(Covariate *node) { }

    void MDLGenerator::visit(PopulationParameter *node) {
        setValue(node->getSymbId());
    }

    void MDLGenerator::visit(IndividualParameter *node) {
        TextFormatter form;

        // Get name
        std::string name = node->getSymbId();

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
                std::string pop = this->accept(node->getPopulationValue());
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
                            coeff = this->accept(fix_eff->getScalar());
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
            std::string assign = this->accept(node->getAssignment());
            form.add(name + " = " + assign);
        }

        form.noFinalNewline();
        this->setValue(form.createString());
    }

    void MDLGenerator::visit(RandomVariable *node) {
        TextFormatter form;

        // Get name of random variable and associated distribution
        std::string name = node->getSymbId();
        pharmmlcpp::Distribution *dist = node->getDistribution();

        // Try to handle Normal1/2 (stdev/var) of ProbOnto and warn if model steps outside
        std::string dist_name = dist->getName();
        std::vector<pharmmlcpp::DistributionParameter *> dist_params = dist->getDistributionParameters();
        if (dist_name == "Normal1" || dist_name == "Normal2") {
            form.openVector(name + " ~ Normal()", 0, ", ");
            std::vector<std::string> unknown_param_types;
            for (pharmmlcpp::DistributionParameter *dist_param : dist_params) {
                std::string name = dist_param->getName();
                std::string assign = this->accept(dist_param->getAssignment());
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
        this->setValue(node->getSymbId());
    }

    void MDLGenerator::visit(Correlation *node) {
        TextFormatter form;

        std::vector<std::string> attr;
        if (node->isPairwise()) {
            attr.push_back("value = " + this->accept(node->getPairwiseAssignment()));
        } else {
            // TODO: Matrix support
        }

        this->setValue(attr);
    }

    void MDLGenerator::visit(IndependentVariable *node) {
        this->setValue(node->getSymbId());
    }

    void MDLGenerator::visit(Variable *node) {
        if (node->getAssignment()) {
            this->setValue(node->getSymbId() + " = " + this->accept(node->getAssignment()));
        } else {
            this->setValue(node->getSymbId());
        }
    }

    void MDLGenerator::visit(DerivativeVariable *node) {
        TextFormatter form;

        std::string name = node->getSymbId();
        form.openVector(name + " : {}", 0, ", ");
        form.add("deriv = " + this->accept(node->getAssignment()));
        form.add("init = " + this->accept(node->getInitialValue()));
        form.add("x0 = " + this->accept(node->getInitialTime()));
        form.closeVector();

        this->setValue(form.createString());
    }

    void MDLGenerator::visit(ObservationModel *node) { }

    void MDLGenerator::visit(Distribution *node) { }

    void MDLGenerator::visit(ColumnMapping *node) {
        std::string id = node->getColumnIdRef();
        std::string name = "UNDEF";
        if (node->getMappedSymbol()) {
            name = node->getMappedSymbol()->getSymbId();
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

        if (tool == "NONMEM") {
            // Generate associative array of mapping targets (to be trimmed before output)
            stringmap mappings;
            std::vector<ColumnMapping *> col_maps = node->getColumnMappings();
            for (ColumnMapping *col_map : col_maps) {
                col_map->accept(this);
                stringpair pair = this->getPairValue();
                mappings.insert(pair);
            }

            Dataset *dataset = node->getDataset();
            if (dataset->isExternal()) {
                // Generate DATA_INPUT_VARIABLES and output DECLARED_VARIABLES
                std::string data_input_vars = this->genDataInputVariablesBlock(dataset, mappings);
                if (!mappings.empty()) {
                    // Output pruned and formatted map from genDataInputVariablesBlock
                    form.openVector("DECLARED_VARIABLES {}", 0, " ");
                    for (stringpair pair : mappings) {
                        form.add(pair.second);
                    }
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
                form.add("inputFormat is nonmemFormat");
                form.closeVector();
                form.closeVector();
            } else {
                this->logger->error("Table as opposed to external resource not supported in ExternalDataset", dataset);
                form.add("# No external dataset file");
            }
        } else {
            this->logger->error("Unknown dataset encoding tool/style '" + tool + "'", node);
            form.add("# Unknown dataset encoding tool/style: \"" + tool + "\"!");
            form.add("# Current support is limited to NONMEM datasets");
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
            attr.push_back("value = " + this->accept(node->getInitValue()));
        }
        if (node->hasLoBound()) {
            attr.push_back("lo = " + this->accept(node->getLoBound()));
        }
        if (node->hasHiBound()) {
            attr.push_back("hi = " + this->accept(node->getHiBound()));
        }
        if (node->isFixed()) {
            attr.push_back("fix = true");
        }
        this->setValue(attr);
    }

    // Class PKMacro
    void MDLGenerator::visit(PKMacro *node) { }
}
