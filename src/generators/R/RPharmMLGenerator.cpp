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

#include "RPharmMLGenerator.h"

namespace pharmmlcpp
{
    // private
    void RPharmMLGenerator::setValue(std::string str) {
        this->value = str;
    }

    std::string RPharmMLGenerator::accept(AstNode *node) {
        node->accept(&this->ast_gen);
        return ast_gen.getValue();
    }

    // public
    std::string RPharmMLGenerator::getValue() {
        return this->value;
    }

    // General R generators
    std::vector<std::string> RPharmMLGenerator::genFunctionDefinitions(PharmML *model) {
        // Generate R code for each function definition in model
        std::vector<std::string> result;
        for (FunctionDefinition *f : model->getFunctionDefinitions()) {
            f->accept(this);
            result.push_back(this->getValue());
        }
        return result;
    }

    // General R visitors
    void RPharmMLGenerator::visit(FunctionDefinition *node) {
        TextFormatter form;

        std::string head = node->getSymbId() + " <- ";
        std::vector<std::string> argument_names;
        for (FunctionArgumentDefinition *argument : node->getArguments()) {
            argument->accept(this);
            argument_names.push_back(this->getValue());
        }
        form.indentAdd(head + TextFormatter::createInlineVector(argument_names, "function()", ", ") + " {");

        form.add("return " + this->accept(node->getDefinition().get()));
        form.outdentAdd("}");

        this->setValue(form.createString());
    }

    void RPharmMLGenerator::visit(FunctionArgumentDefinition *node) {
        std::string arg_name = node->getSymbId();
        this->setValue(arg_name);
    }

    void RPharmMLGenerator::visit(Covariate *node) {
        std::string s = node->getSymbId();
        std::shared_ptr<AstNode> assign = node->getAssignment();
        if (assign) {
            s += " <- " + this->accept(node->getAssignment().get());
        }
        this->setValue(s);
    }

    void RPharmMLGenerator::visit(PopulationParameter *node) {
        this->setValue("\"" + node->getSymbId() + "\"");
    }

    void RPharmMLGenerator::visit(IndividualParameter *node) {
        std::string result;

        if (node->isLinear()) {
            std::string pop = this->accept(node->getPopulationValue().get());
            if (node->getTransformation() != "") {
                pop = node->getTransformation() + "(" + pop + ")";
            }
            std::string cov;
            std::vector<SymbRef *> all_covs = node->getCovariates();
            SymbRef *first_cov;
            if (!all_covs.empty()) {
                first_cov = all_covs[0]; // TODO: Multiple covariate support
                std::string cov = this->accept(first_cov);
                std::vector<FixedEffect *> all_effects = node->getFixedEffects(first_cov);
                if (!all_effects.empty()) {
                    FixedEffect *first_effect = all_effects[0]; // TODO: Multiple fixed effect/covariate support
                    std::string fe = this->accept(first_effect->getReference());
                    cov = " + " + fe + " * " + cov;
                }
            }
            std::string rand;
            std::vector<SymbRef *> all_rand = node->getRandomEffects();
            SymbRef *first_rand; // TODO: Support multiple random effects
            if (!all_rand.empty()) {
                first_rand = all_rand[0];
                rand = this->accept(first_rand);
            }
            result = node->getTransformation() + node->getSymbId() + " <- " + pop + cov + " + " + rand;
        } else if (node->isExplicit()) {
            result = node->getSymbId() + " <- " + this->accept(node->getAssignment().get());
        }

        this->setValue(result);
    }

    void RPharmMLGenerator::visit(VariabilityLevel *node) {
        TextFormatter form;

        if (node->isReferenceLevel()) {
            form.add("# Level: (Reference level)");
        } else {
            form.add("# Level: (Not reference level)");
        }

        this->setValue(form.createString());
    }

    void RPharmMLGenerator::visit(Correlation *node) {}

    void RPharmMLGenerator::visit(RandomVariable *node) {
        TextFormatter form;
        form.openVector(node->getSymbId() + " <- list()", 0, ", ");

        std::vector<pharmmlcpp::VariabilityReference *> var_refs = node->getVariabilityReferences();
        form.openVector("variability_references=c()", 0, ", ");
        for (pharmmlcpp::VariabilityReference *var_ref : var_refs) {
            form.add("'" + this->accept(var_ref->getLevelReference()) + "'");
        }
        form.closeVector();

        node->getDistribution()->accept(this);
        form.add(this->getValue());

        form.closeVector();
        this->setValue(form.createString());
    }

    void RPharmMLGenerator::visit(IndependentVariable *node) {
        this->setValue("IndependentVariable <- \"" + node->getSymbId() + "\"");
    }

    void RPharmMLGenerator::visit(Variable *node) {
        // Consolidate for more powerful output
        if (node->getAssignment()) {
            this->consol.vars.addVariable(node->getSymbId(), this->accept(node->getAssignment().get()));

            // General (non-mandatory) output
            if (node->getAssignment()) {
                this->setValue(node->getSymbId() + " <- " + this->accept(node->getAssignment().get()));
            } else {
                this->setValue(std::string());
            }
        }
    }

    void RPharmMLGenerator::visit(DerivativeVariable *node) {
        // Consolidate for more powerful output
        this->consol.derivs.addDerivative(node->getSymbId(),
                this->accept(node->getAssignment().get()),
                this->accept(node->getInitialValue().get()),
                this->accept(node->getInitialTime().get()));

        // General (non-mandatory) output
        std::string expr;
        if (node->getAssignment()) {
            expr = node->getSymbId() + " <- " + this->accept(node->getAssignment().get());
        } else {
            expr = std::string();
        }
        std::string init_val = "x0=" + this->accept(node->getInitialValue().get());
        std::string init_t = "t0=" + this->accept(node->getInitialTime().get());
        this->setValue("deriv(" + expr + ", iv=" + this->accept(node->getIndependentVariable().get()) + ", " + init_val + ", " + init_t + ")");
    }

    void RPharmMLGenerator::visit(ObservationModel *node) {
        std::string error = "W <- " + this->accept(node->getErrorModel().get());
        std::string output = this->accept(node->getOutput());
        std::string res = this->accept(node->getResidualError());
        std::string y = node->getSymbId() + " <- " + output + " + W * " + res;
        this->setValue(error + "\n" + y);
    }

    void RPharmMLGenerator::visit(Distribution *node) {
        std::string result = "distribution=\"" + node->getName() + "\"";
        for (DistributionParameter *p : node->getDistributionParameters()) {
            result += ", " + p->getName() + "=" + this->accept(p->getAssignment().get());
        }
        this->setValue(result);
    }

    void RPharmMLGenerator::visit(ColumnMapping *node) {
        std::string id = node->getColumnIdRef();
        std::string name = "UNDEF";
        if (node->getMappedSymbol()) {
            name = node->getMappedSymbol()->getSymbId();
        }
        this->setValue(id + " -> " + name);
    }

    void RPharmMLGenerator::visit(MultipleDVMapping *node) {
    }

    // Class ExternalFile
    void RPharmMLGenerator::visit(ExternalFile *node) {
        std::string s = "READ_EXT_RESOURCE_" + node->getOid() + " <- function() {\n";
        s += "  format <- '" + node->getFormat() + "'\n";
        if (node->getFormat() == "CSV") {
            s += "  data <- read.csv(";
            s += "file = '" + node->getPath() + "'";
            s += ", header = TRUE"; // TODO: Check header definition!
            s += ", sep = '" + node->getDelimiter() + "'";
            s += ")\n";
            s += "  return(data)\n";
        }
        s += "}";
        setValue(s);
    }

    // Class DataColumn
    void RPharmMLGenerator::visit(DataColumn *node) {
        std::string s = node->getDefinition()->getId() + " = ";
        std::vector<std::string> list;
        std::vector<std::shared_ptr<AstNode>> data = node->getData();
        for (std::shared_ptr<AstNode> element: data) {
            list.push_back(this->accept(element.get()));
        }
        s += TextFormatter::createInlineVector(list, "c()", ", ");
        setValue(s);
    }

    // Class Dataset
    void RPharmMLGenerator::visit(Dataset *node) {
        std::string name = node->getName();
        std::string s;
        if (!node->isExternal()) {
            std::vector<std::string> list;
            std::vector<DataColumn *> columns = node->getColumns();
            for (DataColumn *column : columns) {
                column->accept(this);
                list.push_back(this->getValue());
            }
            s += name + " = ";
            s += TextFormatter::createIndentedVector(list, "data.frame()", ", ");
        } else {
            // TODO: Improve support for external resource
            // First, output reading function
            ExternalFile *extFile = node->getExternal();
            extFile->accept(this);
            s += this->getValue() + "\n";

            // Then output call to reading function (yes, it's not perfect)
            s += name + " = READ_EXT_RESOURCE_" + extFile->getOid() + "()";
        }
        setValue(s + "\n");
    }

    // Class TargetMapping
    void RPharmMLGenerator::visit(TargetMapping *node) { }

    // Class ExternalDataset
    void RPharmMLGenerator::visit(ExternalDataset *node) {
        std::string s;

        std::vector<ColumnMapping *> col_maps = node->getColumnMappings();
        if (!col_maps.empty()) {
            s += "# Data column mappings\n";
            for (ColumnMapping *col_map : col_maps) {
                col_map->accept(this);
                s += this->getValue() + "\n";
            }
        }

        Dataset *ds = node->getDataset();
        if (ds) {
            s += "# Dataset\n";
            ds->setName(node->getOid());
            ds->accept(this);
            s += this->getValue() + "\n";
        }

        this->setValue(s + "\n");
    }

    // Class Interventions and all its content
    void RPharmMLGenerator::visit(Administration *node) {
        std::string s = node->getOid() + " <- list(";

        s += "type = \"" + node->getType() + "\"";
        s += "target_type = \"" + node->getTargetType() + "\"";
        if (node->getTargetSymbRef()) {
            s += ", target = " + this->accept(node->getTargetSymbRef());
        } else {
            // TODO: TargetMapping
        }
        if (node->getTimes()) {
            s += ", times = " + this->accept(node->getTimes().get());
        }
        if (node->getSteady()) {
            s += ", steady = " + this->accept(node->getSteady().get());
        }
        if (node->getDuration()) {
            s += ", duration = " + this->accept(node->getDuration().get());
        }
        if (node->getRate()) {
            s += ", rate = " + this->accept(node->getRate().get());
        }

        this->setValue(s + ")");
    }

    void RPharmMLGenerator::visit(IndividualAdministration *node) {
        std::string s = " = list(";
        std::vector<std::string> list;

        if (node->getOidRef()->getOidRef() != "") {
            std::string s = "intervention_ref = '" + node->getOidRef()->getOidRef() + "', ";
            std::vector<std::string> list;
        }

        // See IndividualObservations visitor for confusion of formatting
        s += "mappings = ";
        std::vector<ColumnMapping *> column_mappings = node->getColumnMappings();
        for (ColumnMapping *map : column_mappings) {
            map->accept(this);
            list.push_back("'" + this->getValue() + "'");
        }
        s += TextFormatter::createInlineVector(list, "c()", ", ") + ", ";

        s += "dataset = " + node->getDataset()->getName();

        this->setValue(s + ")");
    }

    void RPharmMLGenerator::visit(Interventions *node) {
        std::string s;

        // <Administration>'s
        std::vector<Administration *> adms = node->getAdministrations();
        if (!adms.empty()) {
            s += "# Administrations\n";
            std::vector<std::string> adm_oids;
            for (Administration *adm : adms) {
                adm->accept(this);
                s += this->getValue() + "\n";
                adm_oids.push_back("'" + adm->getOid() + "'");
            }
            s += "administration_oids <- " + TextFormatter::createInlineVector(adm_oids, "c()", ", ") + "\n";
        }

        // <IndividualAdministration>'s
        std::vector<IndividualAdministration *> ind_adms = node->getIndividualAdministrations();
        if (!ind_adms.empty()) {
            s += "# Individual administrations\n";
            s += "individual_administrations <- vector(mode=\"list\", length=" + std::to_string(ind_adms.size()) + ")\n";
            int i = 1;
            for (IndividualAdministration *ind_adm : ind_adms) {
                std::string iteration = std::to_string(i);
                // PharmML dataset has no oid so we generate a name here and accept the dataset separately
                ind_adm->getDataset()->setName("ind_adm_" + iteration + "_ds");
                ind_adm->getDataset()->accept(this);
                s += this->getValue();
                // Accept complete node
                ind_adm->accept(this);
                s += "individual_administrations[[" + iteration + "]]" + this->getValue() + "\n";
                i++;
            }
        }

        this->setValue(s);
    }

    // Class Observations and all its content
    void RPharmMLGenerator::visit(Observation *node) {
        std::string s = node->getOid() + " <- list(";

        s += "times = " + this->accept(&*node->getObservationTimes());
        if (node->getOidRef()->getOidRef() != "") {
            s += ", oidRef = \"" + node->getOidRef()->getOidRef() + "\"";
        }
        if (node->getNumberTimes()) {
            s += ", number = " + this->accept(&*node->getNumberTimes());
        }
/*        if (!node->getContinuousVariables().empty()) {
            s += ", cont_vars = c(";
            bool first = true;
            for (SymbRef *symbol : node->getContinuousVariables()) {
                if (first) {
                    first = false;
                } else {
                    s += ", ";
                }
                s += "'" + this->accept(symbol) + "'";
            }
            s += ")";
        }
        if (!node->getDiscreteVariables().empty()) {
            s += ", disc_vars = c(";
            bool first = true;
            for (SymbRef *symbol : node->getDiscreteVariables()) {
                if (first) {
                    first = false;
                } else {
                    s += ", ";
                }
                s += "'" + this->accept(symbol) + "'";
            }
            s += ")";
        }
*/
        this->setValue(s + ")");
    }

    void RPharmMLGenerator::visit(IndividualObservations *node) {
        // Still don't really know how to output ColumnMapping objects in any context. bquote and .()
        // might be worth checking out in the future to evaluate expressions at run-time (e.g. symbols
        // to column's they are mapped to).
        std::string s = node->getOid() + " <- list(";
        std::vector<std::string> list;

        std::vector<ColumnMapping *> column_mappings = node->getColumnMappings();
        s += "mappings = ";
        for (ColumnMapping *map : column_mappings) {
            map->accept(this);
            list.push_back("'" + this->getValue() + "'");
        }
        s += TextFormatter::createInlineVector(list, "c()", ", ") + ", ";

        s += "dataset = " + node->getDataset()->getName();

        this->setValue(s + ")");
    }

    void RPharmMLGenerator::visit(ObservationCombination *node) {
        std::string s = node->getOid() + " <- list(";

        s += "refs = " + TextFormatter::createInlineVector(node->getOidRefs(), "c()", ", ");
        if (node->getRelative()) {
            s += ", relative = " + this->accept(node->getRelative().get());
        }

        this->setValue(s + ")");
    }

    void RPharmMLGenerator::visit(Observations *node) {
        std::string s;

        std::vector<Variable *> variables = node->getDesignParameters();
        if (!variables.empty()) {
            s += "# Design parameters\n";
            for (Variable *var : variables) {
                var->accept(this);
                s += this->getValue() + "\n";
            }
        }

        std::vector<Observation *> observations = node->getObservations();
        if (!observations.empty()) {
            s += "# Simulation observations\n";
            std::vector<std::string> obs_oids;
            for (Observation *observation : observations) {
                observation->accept(this);
                s += this->getValue() + "\n";
                obs_oids.push_back(observation->getOid());
            }
            s += "simulation_obs_oids = " + TextFormatter::createInlineVector(obs_oids, "c()", ", ") + "\n";
        }

        std::vector<IndividualObservations *> ind_observations = node->getIndividualObservations();
        if (!ind_observations.empty()) {
            s += "# Dataset observations\n";
            std::vector<std::string> obs_oids;
            for (IndividualObservations *ind_observation : ind_observations) {
                std::string oid = ind_observation->getOid();
                obs_oids.push_back("'" + oid + "'");
                // PharmML dataset has no oid so we generate a name here and accept the dataset separately
                ind_observation->getDataset()->setName("dataset_obs_" + oid + "_ds");
                ind_observation->getDataset()->accept(this);
                s += this->getValue();
                // Accept complete node
                ind_observation->accept(this);
                s += this->getValue() + "\n";
            }
            s += "dataset_obs_oids = " + TextFormatter::createInlineVector(obs_oids, "c()", ", ") + "\n";
        }

        std::vector<ObservationCombination *> combinations = node->getObservationCombinations();
        if (!combinations.empty()) {
            s += "# Observation combinations\n";
            std::vector<std::string> comb_oids;
            for (ObservationCombination *comb : combinations) {
                comb->accept(this);
                s += this->getValue() + "\n";
                comb_oids.push_back(comb->getOid());
            }
            s += "combination_oids <- " + TextFormatter::createInlineVector(comb_oids, "c()", ", ") + "\n";
        }

        this->setValue(s);
    }

    // Class Arms and all its contents
    void RPharmMLGenerator::visit(InterventionSequence *node) {
        std::string s = "list(";

        std::vector<std::string> refs;
        for (ObjectRef *ref : node->getOidRefs()) {
            refs.push_back("'" + ref->getOidRef() + "'");
        }
        s += "refs = " + TextFormatter::createInlineVector(refs, "c()", ", ");
        if (node->getStart()) {
            s += ", start = " + this->accept(node->getStart().get());
        }

        this->setValue(s + ")");
    }

    void RPharmMLGenerator::visit(ObservationSequence *node) {
        std::string s = "list(";

        std::vector<std::string> refs;
        for (ObjectRef *ref : node->getOidRefs()) {
            refs.push_back("'" + ref->getOidRef() + "'");
        }
        s += "refs = " + TextFormatter::createInlineVector(refs, "c()", ", ");
        if (node->getStart()) {
            s += ", start = " + this->accept(node->getStart().get());
        }

        this->setValue(s + ")");
    }

    void RPharmMLGenerator::visit(OccasionSequence *node) {
        this->setValue("[WIP]"); // Not implemented
    }

    void RPharmMLGenerator::visit(Arm *node) {
        std::string s = node->getOid() + " <- ";
        std::vector<std::string> list;

        if (node->getOidRef() != "") {
            list.push_back("oidRef = '" + node->getOidRef() + "'");
        }
        if (node->getArmSize()) {
            list.push_back("size = " + this->accept(node->getArmSize().get()));
        }
        if (node->getNumSamples()) {
            list.push_back("samples = " + this->accept(node->getNumSamples().get()));
        }
        if (node->getNumTimes()) {
            list.push_back("times = " + this->accept(node->getNumTimes().get()));
        }
        if (node->getSameTimes()) {
            list.push_back("same_times = " + this->accept(node->getSameTimes().get()));
        }
        if (!node->getInterventionSequences().empty()) {
            std::string s = "intervention_seq = c(";
            bool first = true;
            for (InterventionSequence *seq : node->getInterventionSequences()) {
                if (first) {
                    first = false;
                } else {
                    s += ", ";
                }
                seq->accept(this);
                s += this->getValue();
            }
            list.push_back(s + ")");
        }
        if (!node->getObservationSequences().empty()) {
            std::string s = "observation_seq = c(";
            bool first = true;
            for (ObservationSequence *seq : node->getObservationSequences()) {
                if (first) {
                    first = false;
                } else {
                    s += ", ";
                }
                seq->accept(this);
                s += this->getValue();
            }
            list.push_back(s);
        }
        // TODO: Implement output of node->getOccasionSequences

        s += TextFormatter::createInlineVector(list, "list()", ", ");
        this->setValue(s + ")");
    }

    void RPharmMLGenerator::visit(Arms *node) {
        std::string s;

        // Top-level settings that may or may not exist
        std::vector<std::string> top;
        // <ArmSize>
        if (node->getArmSize()) {
            top.push_back("arm_size = " + this->accept(node->getArmSize().get()));
        }
        // <CostFunction>
        if (node->getCostFunction()) {
            top.push_back("cost_function = " + this->accept(node->getCostFunction().get()));
        }
        // <NumberArms>
        if (node->getNumArms()) {
            top.push_back("num_arms = " + this->accept(node->getNumArms().get()));
        }
        // <NumberSamples>
        if (node->getNumSamples()) {
            top.push_back("num_samples = " + this->accept(node->getNumSamples().get()));
        }
        // <NumberTimes>
        if (node->getNumTimes()) {
            top.push_back("num_times = " + this->accept(node->getNumTimes().get()));
        }
        // <SameTimes>
        if (node->getSameTimes()) {
            top.push_back("same_times = " + this->accept(node->getSameTimes().get()));
        }
        // <TotalCost>
        if (node->getTotalCost()) {
            top.push_back("total_cost = " + this->accept(node->getTotalCost().get()));
        }
        // <TotalSize>
        if (node->getTotalSize()) {
            top.push_back("total_size = " + this->accept(node->getTotalSize().get()));
        }
        if (!top.empty()) {
            s += "arms = " + TextFormatter::createInlineVector(top, "list()", ", ") + "\n";
        }

        // <DesignParameter>'s
        std::vector<Variable *> variables = node->getDesignParameters();
        if (!variables.empty()) {
            s += "# Design parameters\n";
            for (Variable *var : variables) {
                var->accept(this);
                s += this->getValue() + "\n";
            }
        }

        // <Arm>'s
        std::vector<Arm *> arms = node->getArms();
        if (!arms.empty()) {
            s += "# Arm\n";
            std::vector<std::string> arm_oids;
            for (Arm *arm : arms) {
                arm->accept(this);
                s += this->getValue() + "\n";
                arm_oids.push_back(arm->getOid());
            }
            s += "arm_oids <- c(";
            bool first = true;
            for (std::string oid : arm_oids) {
                if (first) {
                    first = false;
                } else {
                    s += ", ";
                }
                s += "'" + oid + "'";
            }
        }

        this->setValue(s + ")" + "\n");
    }

    // Class DesignSpaces and all its content
    void RPharmMLGenerator::visit(DesignSpace *node) {
        std::string s = node->getOid() + " <- ";
        std::vector<std::string> list;

        std::vector<std::string> int_refs;
        for (ObjectRef *ref : node->getInterventionRefs()) {
            int_refs.push_back("'" + ref->getOidRef() + "'");
        }
        list.push_back("intervention_refs = " + TextFormatter::createInlineVector(int_refs, "c()", ", "));

        std::vector<std::string> obs_refs;
        for (ObjectRef *ref : node->getObservationRefs()) {
            obs_refs.push_back("'" + ref->getOidRef() + "'");
        }
        list.push_back("observation_refs = " + TextFormatter::createInlineVector(obs_refs, "c()", ", "));

        std::vector<std::string> arm_refs;
        for (ObjectRef *ref : node->getArmRefs()) {
            arm_refs.push_back("'" + ref->getOidRef() + "'");
        }
        list.push_back("arm_refs = " + TextFormatter::createInlineVector(arm_refs, "c()", ", "));

        AstNode *dosing_times = node->getDosingTimes().get();
        if (dosing_times) {
            list.push_back("dosing_times=" + this->accept(dosing_times));
        }

        s += TextFormatter::createInlineVector(list, "list()", ", ");
        this->setValue(s);
    }

    void RPharmMLGenerator::visit(DesignSpaces *node) {
        std::string s;

        std::vector<Variable *> variables = node->getDesignParameters();
        if (!variables.empty()) {
            s += "# Design parameters\n";
            for (Variable *var : variables) {
                var->accept(this);
                s += this->getValue() + "\n";
            }
        }

        std::vector<DesignSpace *> designSpaces = node->getDesignSpaces();
        if (!designSpaces.empty()) {
            s += "# Design spaces\n";
            s += "design_spaces <- vector(mode=\"list\", length=" + std::to_string(designSpaces.size()) + ")\n";
            int i = 1;
            for (DesignSpace *ds : designSpaces) {
                ds->accept(this);
                s += "design_spaces[[" + std::to_string(i) + "]]" + this->getValue() + "\n";
                i++;
            }
        }

        this->setValue(s + "\n");
    }

    void RPharmMLGenerator::visit(ParameterEstimation *node) { }

    void RPharmMLGenerator::visit(PKMacro *node) { }
}
