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

namespace PharmML
{
    // private
    void MDLGenerator::setValue(std::string str) {
        this->value = str;
    }
    
    void MDLGenerator::setValue(std::pair<std::string, std::string> pair) {
        this->hvalue = pair;
    }
    
    void MDLGenerator::setValue(std::unordered_map<std::string, std::string> hash) {
        this->hvalues = hash;
    }

    std::string MDLGenerator::accept(AstNode *node) {
        node->accept(&this->ast_gen);
        return ast_gen.getValue();
    }

    // public
    std::string MDLGenerator::getValue() {
        return this->value;
    }
    
    std::pair<std::string, std::string> MDLGenerator::getPairValue() {
        return this->hvalue;
    }
    
    std::unordered_map<std::string, std::string> MDLGenerator::getHashValue() {
        return this->hvalues;
    }
    
    // Generators
    std::string MDLGenerator::generateModel(Model *model) {
        // FIXME: Bad design to put in model here? A smell of visitor pattern breakdown. Solution might be visitor on Model level.
        RFormatter form;
        
        // Generate the MDL data object(s)
        std::vector<ExternalDataset *> ext_dss = model->getTrialDesign()->getExternalDatasets();
        for (ExternalDataset *ext_ds : ext_dss) {
            std::string name = ext_ds->getOid();
            form.addMany(name + " = " + this->genDataObj(ext_ds));
            form.add("");
        }
        
        // Generate the three other MDL objects
        form.addMany("model_par = " + this->genParObj());
        form.add("");
        form.addMany("model_mdl = " + this->genMdlObj());
        form.add("");
        form.addMany("model_task = " + this->genTaskObj());
        form.add("");
        form.addMany("model_mog = " + this->genMogObj());
        
        return form.createString();
    }
    
    std::string MDLGenerator::genDataObj(ExternalDataset* ext_ds) {
        RFormatter form;
        
        form.indentAdd("dataObj {");
        
        ext_ds->accept(this);
        form.addMany(this->getValue());
        
        form.outdentAdd("} # end data object");
        
        return form.createString();
    }
    
    std::string MDLGenerator::genParObj() {
        RFormatter form;
        
        form.indentAdd("parObj {");
        form.outdentAdd("} # end parameter object");
        
        return form.createString();
    }
    
    std::string MDLGenerator::genMdlObj() {
        RFormatter form;
        
        form.indentAdd("mdlObj {");
        form.outdentAdd("} # end model object");
        
        return form.createString();
    }
    
    std::string MDLGenerator::genTaskObj() {
        RFormatter form;
        
        form.indentAdd("taskObj {");
        form.outdentAdd("} # end task object");
        
        return form.createString();
    }
    
    std::string MDLGenerator::genMogObj() {
        RFormatter form;
        
        form.indentAdd("mogObj {");
        form.outdentAdd("} # end mog object");
        
        return form.createString();
    }
    
    // MDL visitors
    void MDLGenerator::visit(FunctionDefinition *node) { }

    void MDLGenerator::visit(Covariate *node) { }

    void MDLGenerator::visit(PopulationParameter *node) { }

    void MDLGenerator::visit(IndividualParameter *node) { }

    void MDLGenerator::visit(RandomVariable *node) { }

    void MDLGenerator::visit(IndependentVariable *node) { }

    void MDLGenerator::visit(Variable *node) { }
    
    void MDLGenerator::visit(DerivativeVariable *node) { }

    void MDLGenerator::visit(ObservationModel *node) { }

    void MDLGenerator::visit(Distribution *node) { }

    void MDLGenerator::visit(ColumnMapping *node) {
       std::pair<std::string, std::string> pair = {
            node->getColumnIdRef(), this->accept(node->getFirstSymbol())
        };
        this->setValue(pair);
    }
    
    // Class ExternalFile
    void MDLGenerator::visit(ExternalFile *node) {
        std::string s = "'" + node->getPath() + "'";
        setValue(s);
    }
    
    // Class DataColumn
    void MDLGenerator::visit(DataColumn *node) { }
    
    // Class Dataset
    void MDLGenerator::visit(Dataset *node) { }
    
    std::string MDLGenerator::genDATA_INPUT_VARIABLES(Dataset *node, std::unordered_map<std::string, std::string> column_mappings) {
        if (node->isExternal()) {
            RFormatter form;
            form.openVector("DATA_INPUT_VARIABLES {}", 1, "");
            
            // Loop through each column definition
            int num_cols = node->getDefinition()->getNumColumns();
            for (int num = 1; num <= num_cols; ++num) {
                // Get column id
                ColumnDefinition *col_def = node->getDefinition()->getColumnDefinition(num);
                std::string id = col_def->getId();
                
                // Open vector with column id as header
                form.openVector(id + " : {}", 0, ", ");
                form.add("use is " + col_def->getType());
                
                // Find match in column mappings and add it (if found)
                auto got = column_mappings.find(id);
                if (got != column_mappings.end()) {
                    form.add("variable = " + column_mappings[id]);
                }
                
                form.closeVector();
            }
            form.closeVector();
            
            form.indentAdd("SOURCE {");
            form.openVector("srcfile : {}", 0, ", ");
            
            ExternalFile *extFile = node->getExternal();
            extFile->accept(this);
            form.add(this->getValue());
            
            form.closeVector();
            form.outdentAdd("}");
            return form.createString();
        } else {
            // Yes, what else?
        }
    }
    
    // Class ExternalDataset
    void MDLGenerator::visit(ExternalDataset *node) {
        RFormatter form;
        
        // Produce associative array of column mappings
        form.openVector("DECLARED_VARIABLES{}", 0, " ");
        std::vector<ColumnMapping *> col_maps = node->getColumnMappings();
        std::unordered_map<std::string, std::string> mappings;
        for (ColumnMapping *col_map : col_maps) {
            col_map->accept(this);
            std::pair<std::string, std::string> pair = this->getPairValue();
            form.add(pair.first);
            mappings.insert(pair);
        }
        form.closeVector();
        form.add("");
        
        // Get dataset and generate DATA_INPUT_VARIABLES
        Dataset *dataset = node->getDataset();
        form.addMany(this->genDATA_INPUT_VARIABLES(dataset, mappings));
        
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
}
