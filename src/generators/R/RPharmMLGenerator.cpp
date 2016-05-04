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

namespace Text
{
    // Helper function to reduce redundant code
    // TODO: Overload with similar function accepting vector of nodes and performing element->accept(this) instead (how?)
    std::string formatVector(std::vector<std::string> vector, std::string prefix, std::string quote, int pre_indent) {
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
    
    // Indented code generator helper class
    std::string Indenter::genIndentation() {
        return std::string(this->indentLevel * this->indentSize, this->indentSymbol);
    }
    
    // Methods to add units (csv, rows, appends, blocks)
    void Indenter::addCSV(std::string str) {
        str = str + ",";
        if (this->multilineCSV) {
            // Multiple lines; add row (with indent)
            this->addRow(str);
        } else {
            // Single line; push back on last row (without indent)
            if (this->firstCSV == false) {
                str = " " + str;
            }
            this->appendRow(str);
        }
        if (this->firstCSV) {
            this->firstCSV = false;
        }
    }

    void Indenter::addRow(std::string str) {
        // Add a single row (at current indentation level)
        this->rows.push_back(this->genIndentation() + str);
    }
    
    void Indenter::appendRow(std::string str) {
        // Append last row added
        std::string &last_row = this->rows.back();
        last_row.append(str);
    }

    void Indenter::addRowIndent(std::string str) {
        // Add row and THEN increase indent
        this->addRow(str);
        this->openIndent();
    }

    void Indenter::addRowOutdent(std::string str) {
        // Decrease indent and THEN add row
        this->closeIndent();
        this->addRow(str);
    }
    
    void Indenter::addBlock(std::string str) {
        // Split multi-line string into rows and then add them
        std::stringstream ss(str);
        std::string row;
        std::vector<std::string> rows;
        
        while(std::getline(ss,row,'\n')){
            rows.push_back(row);
        }

        this->addBlock(rows);
    }

    void Indenter::addBlock(std::vector<std::string> strs) {
        // Add each row
        for (std::string row : strs) {
            this->addRow(row);
        }
    }
    
    // Methods to open and close (indent, csv)
    void Indenter::openIndent() {
        this->indentLevel++;
    }

    void Indenter::closeIndent() {
        this->indentLevel--;
    }
    
    void Indenter::closeCSVlist() {
        // Pop last applied separator and reset flag to close the list
        if (this->firstCSV == false) {
            std::string &last_row = this->rows.back();
            last_row.pop_back();
            this->firstCSV = true;
        }
    }
    
    // Methods to setup indenter
    void Indenter::setCSVformat(bool multiline, char separator) {
        this->multilineCSV = multiline;
        this->separator = separator;
    }
    
    void Indenter::setIndent(int size, char symbol) {
        this->indentSize = size;
        this->indentSymbol = symbol;
    }
    
    // Call to produce final multilined result
    std::string Indenter::createString(bool final_newline) {
        // Create a single multi-line string from all rows
        std::string result;
        for (auto &row : this->rows) {
            if (!final_newline && &row == &this->rows.back()) {
                result += row;
            } else {
                result += row + "\n";
            }
        }
        return result;
    }
}

namespace PharmML
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
    std::vector<std::string> RPharmMLGenerator::genFunctionDefinitions(Model *model) {
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
        Text::Indenter ind;
        
        std::string head = node->getSymbId() + " <- ";
        std::vector<std::string> args = node->getArguments();
        ind.addRowIndent(head + Text::formatVector(args, "function", "") + " {");
        
        ind.addRow("return " + this->accept(node->getAssignment()));
        ind.addRowOutdent("}");
        
        this->setValue(ind.createString());
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
        // Consolidate for more powerful output
        if (node->getAssignment()) {
            this->consol.vars.addVariable(node->getSymbId(), this->accept(node->getAssignment())); 

            // General (non-mandatory) output
            if (node->getAssignment()) {
                this->setValue(node->getSymbId() + " <- " + this->accept(node->getAssignment()));
            } else {
                this->setValue(std::string());
            }
        }
    }
    
    void RPharmMLGenerator::visit(DerivativeVariable *node) {
        // Consolidate for more powerful output
        this->consol.derivs.addDerivative(node->getSymbId(),
                this->accept(node->getAssignment()),
                this->accept(node->getInitialTime()),
                this->accept(node->getInitialValue())); 
        
        // General (non-mandatory) output
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
        //~ this->setValue(node->getColumnIdRef() + " -> " + this->accept(node->getAssignment()));
        this->setValue(node->getColumnIdRef() + " -> " + this->accept(node->getFirstSymbol()));
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
        std::vector<AstNode *> data = node->getData();
        for (AstNode *element: data) {
            list.push_back(this->accept(element));
        }
        s += Text::formatVector(list, "c", "");
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
            s += Text::formatVector(list, "data.frame", "", s.size());
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
        
        std::vector<Dataset *> datasets = node->getDatasets();
        if (!datasets.empty()) {
            s += "# Datasets\n";
            s += "datasets <- vector(mode=\"list\", length=" + std::to_string(datasets.size()) + ")\n";
            int i = 1;
            for (Dataset *ds : datasets) {
                ds->setName("datasets[[" + std::to_string(i) + "]]");
                ds->accept(this);
                s += this->getValue() + "\n";
                i++;
            }
        }
        
        this->setValue(s + "\n");
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
        s += Text::formatVector(list, "c") + ", ";
        
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
                adm_oids.push_back(adm->getOid());
            }
            s += "administration_oids <- " + Text::formatVector(adm_oids, "c") + "\n";
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
        s += Text::formatVector(list, "c") + ", ";
        
        s += "dataset = " + node->getDataset()->getName();
        
        this->setValue(s + ")");
    }
    
    void RPharmMLGenerator::visit(ObservationCombination *node) {
        std::string s = node->getOid() + " <- list(";
        
        s += "refs = " + Text::formatVector(node->getOidRefs(), "c");
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
            s += "simulation_obs_oids = " + Text::formatVector(obs_oids, "c") + "\n";
        }
        
        std::vector<IndividualObservations *> ind_observations = node->getIndividualObservations();
        if (!ind_observations.empty()) {
            s += "# Dataset observations\n";
            std::vector<std::string> obs_oids;
            for (IndividualObservations *ind_observation : ind_observations) {
                std::string oid = ind_observation->getOid();
                obs_oids.push_back(oid);
                // PharmML dataset has no oid so we generate a name here and accept the dataset separately
                ind_observation->getDataset()->setName("dataset_obs_" + oid + "_ds");
                ind_observation->getDataset()->accept(this);
                s += this->getValue();
                // Accept complete node
                ind_observation->accept(this);
                s += this->getValue() + "\n";
            }
            s += "dataset_obs_oids = " + Text::formatVector(obs_oids, "c") + "\n";
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
            s += "combination_oids <- " + Text::formatVector(comb_oids, "c") + "\n";
        }
        
        this->setValue(s);
    }
    
    // Class Arms and all its contents
    void RPharmMLGenerator::visit(InterventionSequence *node) {
        std::string s = "list(";
        
        s += "refs = " + Text::formatVector(node->getOidRefs(), "c");
        if (node->getStart()) {
            s += ", start = " + this->accept(node->getStart());
        }
        
        this->setValue(s + ")");
    }
    
    void RPharmMLGenerator::visit(ObservationSequence *node) {
        std::string s = "list(";
        
        s += "refs = " + Text::formatVector(node->getOidRefs(), "c");
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
        
        s += Text::formatVector(list, "list", "");
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
            s += "arms = " + Text::formatVector(top, "list") + "\n";
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
        
        list.push_back("intervention_refs = " + Text::formatVector(node->getInterventionRefs(), "c"));
        list.push_back("observation_refs = " + Text::formatVector(node->getObservationRefs(), "c"));
        list.push_back("arm_refs = " + Text::formatVector(node->getArmRefs(), "c"));
        AstNode *dosing_times = node->getDosingTimes();
        if (dosing_times) {
            list.push_back("dosing_times=" + this->accept(dosing_times));
        }
        
        s += Text::formatVector(list, "list", "");
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
