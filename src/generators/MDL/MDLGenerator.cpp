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
    
    void MDLGenerator::setValue(stringpair pair) {
        this->hvalue = pair;
    }
    
    void MDLGenerator::setValue(stringmap hash) {
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
    
    stringpair MDLGenerator::getPairValue() {
        return this->hvalue;
    }
    
    stringmap MDLGenerator::getHashValue() {
        return this->hvalues;
    }
    
    // Generators
    std::string MDLGenerator::generateModel(Model *model) {
        // FIXME: Bad design to put in model here? A smell of visitor pattern breakdown. Solution might be visitor on Model level.
        RFormatter form;
        
        // Generate the MDL data object(s)
        std::vector<ExternalDataset *> ext_dss = model->getTrialDesign()->getExternalDatasets();
        std::vector<std::string> dataObjNames;
        for (ExternalDataset *ext_ds : ext_dss) {
            std::string name = ext_ds->getOid();
            dataObjNames.push_back(name);
            form.addMany(name + " = " + this->genDataObj(ext_ds));
            form.add("");
        }
        
        // Generate the MDL parameter object(s)
        // TODO: Implement support for multiple parameter models as per schema
        //~ std::vector<ParameterModel *> par_models = model->getModelDefinition()->getParameterModels();
        //~ for (par_model : par_models) {
        ParameterModel *par_model = model->getModelDefinition()->getParameterModel();
        std::vector<std::string> parObjNames;
        std::string name = par_model->getBlkId();
        parObjNames.push_back(name);
        std::vector<EstimationStep *> estim_steps = model->getModellingSteps()->getEstimationSteps();
        form.addMany(name + " = " + this->genParObj(par_model, estim_steps));
        form.add("");
        //~ }
        
        
        // Generate the MDL model object(s)
        std::vector<std::string> mdlObjNames;
        mdlObjNames.push_back("mdl_object");
        form.addMany(mdlObjNames[0] + " = " + this->genMdlObj());
        form.add("");
        
        // Generate the MDL task object(s)
        std::vector<std::string> taskObjNames;
        taskObjNames.push_back("task_object");
        form.addMany(taskObjNames[0] + " = " + this->genTaskObj());
        form.add("");
        
        // Generate the MDL mog object(s)
        form.addMany("mog_object = " + this->genMogObj(dataObjNames[0], parObjNames[0], mdlObjNames[0], taskObjNames[0]));
        
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
    
    std::string MDLGenerator::genParObj(ParameterModel *par_model, std::vector<EstimationStep *> estim_steps) {
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
    
    std::string MDLGenerator::genMogObj(std::string dataObj, std::string parObj, std::string mdlObj, std::string taskObj) {
        RFormatter form;
        
        form.indentAdd("mogObj {");
        form.openVector("OBJECTS {}", 1, "");
        form.add(dataObj + " : { type is dataObj }");
        form.add(parObj + " : { type is parObj }");
        form.add(mdlObj + " : { type is mdlObj }");
        form.add(taskObj + " : { type is taskObj }");
        form.closeVector();
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
        stringpair pair = {
            node->getColumnIdRef(), this->accept(node->getFirstSymbol())
        };
        this->setValue(pair);
    }
    
    // Class ExternalFile (this class might be superfluous)
    void MDLGenerator::visit(ExternalFile *node) { }
    
    // Class DataColumn
    void MDLGenerator::visit(DataColumn *node) { }
    
    // Class Dataset
    void MDLGenerator::visit(Dataset *node) { }
    
    std::string MDLGenerator::genDataInputVariablesBlock(Dataset *node, stringmap &column_mappings) {
        stringmap implicit_mappings = {{"id", "ID"}, {"idv", "T"}};
        if (node->isExternal()) {
            RFormatter form;
            
            // Output one row for each column definition
            int num_cols = node->getDefinition()->getNumColumns();
            for (int num = 1; num <= num_cols; ++num) {
                // Get column id
                ColumnDefinition *col_def = node->getDefinition()->getColumnDefinition(num);
                std::string id = col_def->getId();
                
                // Open vector with column id as header
                form.openVector(id + " : {}", 0, ", ");
                std::string type = col_def->getType();
                form.add("use is " + type);
                
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
                
                form.closeVector();
            }
            
            return form.createString();
        } else {
            // Yes, what else?
        }
    }
    
    // Class ExternalDataset
    void MDLGenerator::visit(ExternalDataset *node) {
        RFormatter form;
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
                form.openVector("DECLARED_VARIABLES{}", 0, " ");
                for (stringpair pair : mappings) {
                    form.add(pair.second);
                }
                form.closeVector();
                form.add("");
                
                // Output DATA_INPUT_VARIABLES
                form.openVector("DATA_INPUT_VARIABLES {}", 1, "");
                form.addMany(data_input_vars);
                form.closeVector();
                form.append(" # end DATA_INPUT_VARIABLES");
                form.add("");
                
                // Generate SOURCE
                form.indentAdd("SOURCE {");
                ExternalFile *file = dataset->getExternal();
                form.add("# Name: " + file->getOid());
                form.add("# Type: " + file->getFormat());
                form.add("# Delimiter: \"" + file->getDelimiter() + "\"");
                form.openVector("srcfile : {}", 1, ", ");
                form.add("file = \"" + file->getPath() + "\"");
                form.add("inputFormat = nonmemFormat");
                form.closeVector();
                form.outdentAdd("} # end SOURCE");
            } else {
                form.add("# Lack of external dataset!");
            }
        } else {
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
}
