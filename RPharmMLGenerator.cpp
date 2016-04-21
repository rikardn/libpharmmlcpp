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

namespace PharmML
{
    // private
    void RPharmMLGenerator::setValue(std::string str) {
        this->value = str;
    }

    // Helper function to reduce redundant code
    // TODO: Overload with similar function accepting vector of nodes and performing element->accept(this) instead (how?)
    std::string RPharmMLGenerator::formatVector(std::vector<std::string> vector, std::string prefix, std::string quote) {
        std::string s = prefix + "(";
        
        bool first = true;
        for (std::string element : vector) {
            if (first) {
                first = false;
            } else {
                s += ", ";
            }
            s += quote + element + quote;
        }
        return(s + ")");
    }

    std::string RPharmMLGenerator::accept(AstNode *node) {
        node->accept(&this->ast_gen);
        return ast_gen.getValue();
    }

    // public
    std::string RPharmMLGenerator::getValue() {
        return this->value;
    }
    
    void RPharmMLGenerator::visit(FunctionDefinition *node) {
        std::string head = node->getSymbId() + " <- function(";
        std::vector<std::string> args = node->getArguments();
        for (int i = 0; i < args.size(); i++) {
            head += args[i];
            if (i != args.size() - 1) {
                head += ", ";
            }
        }
        head += ") {\n";
        std::string code = "  " + this->accept(node->getAssignment()) + "\n}";
        this->setValue(head + code);
    }

    void RPharmMLGenerator::visit(Covariate *node) {
        this->setValue(node->getTransformedName() + " <- " + this->accept(node->getAssignment()));
    }

    void RPharmMLGenerator::visit(PopulationParameter *node) {
        this->setValue("\"" + node->getSymbId() + "\"");
    }

    void RPharmMLGenerator::visit(IndividualParameter *node) {
        std::string result;

        if (node->isStructured()) {
            std::string pop = this->accept(node->getPopulationValue());
            if (node->getTransformation() != "") {
                pop = node->getTransformation() + "(" + pop + ")";
            }
            std::string cov;
            if (node->getFixedEffect()) {
                std::string fe = this->accept(node->getFixedEffect());
                std::string cov = this->accept(node->getCovariate());
                cov = " + " + fe + " * " + cov;
            }
            result = node->getTransformation() + node->getSymbId() + " <- " + pop + cov + " + " + this->accept(node->getRandomEffects());
        } else {
            result = node->getSymbId() + " <- " + this->accept(node->getAssignment());
        }
        this->setValue(result);
    }

    void RPharmMLGenerator::visit(RandomVariable *node) {
        std::string var_ref = "variability_reference=\"" + this->accept(node->getVariabilityReference()) + "\"";
        node->getDistribution()->accept(this);
        std::string dist = this->getValue();
        this->setValue(node->getSymbId() + " <- list(" + var_ref + ", " + dist + ")");
    }

    void RPharmMLGenerator::visit(IndependentVariable *node) {
        this->setValue("IndependentVariable <- \"" + node->getSymbId() + "\"");
    }

    void RPharmMLGenerator::visit(Variable *node) {
        if (node->getAssignment()) {
            this->setValue(node->getSymbId() + " <- " + this->accept(node->getAssignment()));
        } else {
            this->setValue(std::string());
        }
    }
    
    void RPharmMLGenerator::visit(DerivativeVariable *node) {
        std::string expr;
        if (node->getAssignment()) {
            expr = node->getSymbId() + " <- " + this->accept(node->getAssignment());
        } else {
            expr = std::string();
        }
        std::string init_val = "x0=" + this->accept(node->getInitialValue());
        std::string init_t = "t0=" + this->accept(node->getInitialTime());
        this->setValue("deriv(" + expr + ", iv=" + this->accept(node->getIndependentVariable()) + ", " + init_val + ", " + init_t + ")");
    }

    void RPharmMLGenerator::visit(ObservationModel *node) {
        std::string error = "W <- " + this->accept(node->getErrorModel());
        std::string output = this->accept(node->getOutput());
        std::string res = this->accept(node->getResidualError());
        std::string y = node->getSymbId() + " <- " + output + " + W * " + res;
        this->setValue(error + "\n" + y);
    }

    void RPharmMLGenerator::visit(Distribution *node) {
        std::string result = "distribution=\"" + node->getName() + "\"";
        for (DistributionParameter *p : node->getDistributionParameters()) {
            result += ", " + p->getName() + "=" + this->accept(p->getAssignment());
        }
        this->setValue(result);
    }

    void RPharmMLGenerator::visit(ColumnMapping *node) {
        this->setValue(node->getColumnIdRef() + " -> " + this->accept(node->getAssignment()));
    }
    
    // Class Interventions and all its content
    void RPharmMLGenerator::visit(Administration *node) {
        std::string s = node->getOid() + " <- list(";
        
        s += "type = \"" + node->getType() + "\"";
        s += ", target = " + this->accept(node->getTarget());
        if (node->getTimes()) {
            s += ", times = " + this->accept(node->getTimes());
        }
        if (node->getSteady()) {
            s += ", steady = " + this->accept(node->getSteady());
        }
        if (node->getDuration()) {
            s += ", duration = " + this->accept(node->getDuration());
        }
        if (node->getRate()) {
            s += ", rate = " + this->accept(node->getRate());
        }

        this->setValue(s + ")");
    }
    
    void RPharmMLGenerator::visit(IndividualAdministration *node) {
        std::string s = " = list(";
        std::vector<std::string> list;
        
        if (node->getOidRef() != "") {
            std::string s = "intervention_ref = '" + node->getOidRef() + "', ";
            std::vector<std::string> list;
        }
        
        // See IndividualObservations visitor for confusion of formatting
        s += "mappings = ";
        std::vector<ColumnMapping *> column_mappings = node->getColumnMappings();
        for (ColumnMapping *map : column_mappings) {
            map->accept(this);
            list.push_back(this->getValue());
        }
        s += formatVector(list, "c");
        
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
                adm_oids.push_back(adm->getOid());
            }
            s += "administration_oids <- " + formatVector(adm_oids, "c") + "\n";
        }
        
        // <IndividualAdministration>'s
        std::vector<IndividualAdministration *> ind_adms = node->getIndividualAdministrations();
        if (!ind_adms.empty()) {
            s += "# Individual administrations\n";
            s += "individual_administrations <- vector(mode=\"list\", length=" + std::to_string(ind_adms.size()) + ")\n";
            int i = 1;
            for (IndividualAdministration *ind_adm : ind_adms) {
                ind_adm->accept(this);
                s += "individual_administrations[[" + std::to_string(i) + "]]" + this->getValue() + "\n";
                i++;
            }
        }

        this->setValue(s);
    }
    
    // Class Observations and all its content
    void RPharmMLGenerator::visit(Observation *node) {
        std::string s = node->getOid() + " <- list(";
        
        s += "times = " + this->accept(node->getTimes());
        if (node->getOidRef() != "") {
            s += ", oidRef = \"" + node->getOidRef() + "\"";
        }
        if (node->getNumber()) {
            s += ", number = " + this->accept(node->getNumber());
        }
        if (!node->getContinuousVariables().empty()) {
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
            list.push_back(this->getValue());
        }
        s += formatVector(list, "c");
        
        this->setValue(s + ")");
    }
    
    void RPharmMLGenerator::visit(ObservationCombination *node) {
        std::string s = node->getOid() + " <- list(";
        
        s += "refs = " + formatVector(node->getOidRefs(), "c");
        if (node->getRelative()) {
            s += ", relative = " + this->accept(node->getRelative());
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
            s += "simulation_obs_oids = " + formatVector(obs_oids, "c") + "\n";
        }
        
        std::vector<IndividualObservations *> ind_observations = node->getIndividualObservations();
        if (!ind_observations.empty()) {
            s += "# Dataset observations\n";
            std::vector<std::string> obs_oids;
            for (IndividualObservations *ind_observation : ind_observations) {
                ind_observation->accept(this);
                s += this->getValue() + "\n";
                obs_oids.push_back(ind_observation->getOid());
            }
            s += "dataset_obs_oids = " + formatVector(obs_oids, "c") + "\n";
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
            s += "combination_oids <- " + formatVector(comb_oids, "c") + "\n";
        }
        
        this->setValue(s);
    }
    
    // Class Arms and all its contents
    void RPharmMLGenerator::visit(InterventionSequence *node) {
        std::string s = "list(";
        
        s += "refs = " + formatVector(node->getOidRefs(), "c");
        if (node->getStart()) {
            s += ", start = " + this->accept(node->getStart());
        }
        
        this->setValue(s + ")");
    }
    
    void RPharmMLGenerator::visit(ObservationSequence *node) {
        std::string s = "list(";
        
        s += "refs = " + formatVector(node->getOidRefs(), "c");
        if (node->getStart()) {
            s += ", start = " + this->accept(node->getStart());
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
            list.push_back("size = " + this->accept(node->getArmSize()));
        }
        if (node->getNumSamples()) {
            list.push_back("samples = " + this->accept(node->getNumSamples()));
        }
        if (node->getNumTimes()) {
            list.push_back("times = " + this->accept(node->getNumTimes()));
        }
        if (node->getSameTimes()) {
            list.push_back("same_times = " + this->accept(node->getSameTimes()));
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
        
        s += formatVector(list, "list", "");
        this->setValue(s + ")");
    }
    
    void RPharmMLGenerator::visit(Arms *node) {
        std::string s;
        
        // Top-level settings that may or may not exist
        std::vector<std::string> top;
        // <ArmSize>
        if (node->getArmSize()) {
            top.push_back("arm_size = " + this->accept(node->getArmSize()));
        }
        // <CostFunction>
        if (node->getCostFunction()) {
            top.push_back("cost_function = " + this->accept(node->getCostFunction()));
        }
        // <NumberArms>
        if (node->getNumArms()) {
            top.push_back("num_arms = " + this->accept(node->getNumArms()));
        }
        // <NumberSamples>
        if (node->getNumSamples()) {
            top.push_back("num_samples = " + this->accept(node->getNumSamples()));
        }
        // <NumberTimes>
        if (node->getNumTimes()) {
            top.push_back("num_times = " + this->accept(node->getNumTimes()));
        }
        // <SameTimes>
        if (node->getSameTimes()) {
            top.push_back("same_times = " + this->accept(node->getSameTimes()));
        }
        // <TotalCost>
        if (node->getTotalCost()) {
            top.push_back("total_cost = " + this->accept(node->getTotalCost()));
        }
        // <TotalSize>
        if (node->getTotalSize()) {
            top.push_back("total_size = " + this->accept(node->getTotalSize()));
        }
        if (!top.empty()) {
            s += "arms = " + formatVector(top, "list") + "\n";
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
        
        list.push_back("intervention_refs = " + formatVector(node->getInterventionRefs(), "c"));
        list.push_back("observation_refs = " + formatVector(node->getObservationRefs(), "c"));
        list.push_back("arm_refs = " + formatVector(node->getArmRefs(), "c"));
        AstNode *dosing_times = node->getDosingTimes();
        if (dosing_times) {
            list.push_back("dosing_times=" + this->accept(dosing_times));
        }
        
        s += formatVector(list, "list", "");
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
}
