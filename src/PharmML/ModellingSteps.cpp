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

#include <PharmML/PharmMLContext.h>
#include <PharmML/ModellingSteps.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <xml/xml.h>

namespace pharmmlcpp
{
    TargetTool::TargetTool(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void TargetTool::parse(PharmMLReader &reader, xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        // Get target tool name
        xml::Node name_node = reader.getSingleElement(node, "./msteps:TargetToolName");
        this->name = name_node.getText();

        // Get column mappings
        std::vector<xml::Node> col_maps = reader.getElements(node, "./ds:ColumnMapping");
        for (xml::Node col_map : col_maps) {
            pharmmlcpp::ColumnMapping *col = new ColumnMapping(reader, col_map);
            this->columnMappings.push_back(col);
        }

        // Get target tool data
        std::vector<xml::Node> tool_data_nodes = reader.getElements(node, "./ds:TargetToolData");
        //for (xml::Node tool_data_node : tool_data_nodes) {
            // TODO: Support this
        //}

        // TODO: Support CodeInjection
    }

    VariableAssignment::VariableAssignment(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void VariableAssignment::parse(PharmMLReader &reader, xml::Node node) {
        // Get SymbRef
        xml::Node ref_node = reader.getSingleElement(node, "./ct:SymbRef");
        this->symbRef = new SymbRef(reader, ref_node);

        // Get the assignment of that SymbRef
        xml::Node assign = reader.getSingleElement(node, "./ct:Assign");
        xml::Node tree = assign.getChild();
        this->assignment = AstNode::create(reader, tree);
    }

    SymbRef *VariableAssignment::getSymbRef() {
        return this->symbRef;
    }

    std::shared_ptr<AstNode> VariableAssignment::getAssignment() {
        return this->assignment;
    }

    CommonStepType::CommonStepType(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void CommonStepType::parse(PharmMLReader &reader, xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        // Get software settings
        std::vector<xml::Node> sw_set_nodes = reader.getElements(node, "./msteps:SoftwareSettings");
        for (xml::Node sw_set_node : sw_set_nodes) {
            xml::Node ext_file_node = reader.getSingleElement(sw_set_node, "./ds:File");
            pharmmlcpp::ExternalFile *ext_file = new ExternalFile(reader, ext_file_node);
            this->softwareSettings.push_back(ext_file);
        }

        // Get output files
        std::vector<xml::Node> out_nodes = reader.getElements(node, "./ds:OutputFiles");
        for (xml::Node out_node : out_nodes) {
            pharmmlcpp::ExternalFile *ext_file = new ExternalFile(reader, out_node);
            this->outputFiles.push_back(ext_file);
        }

        // Get target tool reference
        xml::Node tool_ref_node = reader.getSingleElement(node, "./msteps:TargetToolReference");
        if (tool_ref_node.exists()) {
            this->targetToolRef = tool_ref_node.getChild().getAttribute("oidRef").getValue();
        }

        // Get external dataset reference
        xml::Node ds_ref_node = reader.getSingleElement(node, "./msteps:ExternalDataSetReference/ct:OidRef");
        if (ds_ref_node.exists()) {
            this->extDatasetRef = ds_ref_node.getAttribute("oidRef").getValue();
        }

        // Get interventions reference
        xml::Node int_ref_node = reader.getSingleElement(node, "./msteps:InterventionReference");
        if (int_ref_node.exists()) {
            std::vector<xml::Node> ref_nodes = reader.getElements(int_ref_node, "./ct:OidRef");
            for (xml::Node ref_node : ref_nodes) {
                std::string ref = ref_node.getAttribute("oidRef").getValue();
                this->interventionsRefs.push_back(ref);
            }
        }

        // Get observations reference
        xml::Node obs_ref_node = reader.getSingleElement(node, "./msteps:ObservationsReference");
        if (obs_ref_node.exists()) {
            std::vector<xml::Node> ref_nodes = reader.getElements(int_ref_node, "./ct:OidRef");
            for (xml::Node ref_node : ref_nodes) {
                std::string ref = ref_node.getAttribute("oidRef").getValue();
                this->observationsRefs.push_back(ref);
            }
        }

        // Get variable assignments
        std::vector<xml::Node> assign_nodes = reader.getElements(node, "./ct:VariableAssignment");
        for (xml::Node assign_node : assign_nodes) {
            pharmmlcpp::VariableAssignment *var_assign = new VariableAssignment(reader, assign_node);
            this->varAssignments.push_back(var_assign);
        }
    }

    std::string CommonStepType::getOid() {
        return this->oid;
    }

    std::vector<ExternalFile *> CommonStepType::getSoftwareSettingsFiles() {
        return this->softwareSettings;
    }

    std::vector<ExternalFile *> CommonStepType::getOutputFiles() {
        return this->outputFiles;
    }

    std::string CommonStepType::getTargetToolRef() {
        return this->targetToolRef;
    }

    std::string CommonStepType::getExternalDatasetRef() {
        return this->extDatasetRef;
    }

    std::vector<std::string> CommonStepType::getInterventionsRefs() {
        return this->interventionsRefs;
    }

    std::vector<std::string> CommonStepType::getObservationsRefs() {
        return this->observationsRefs;
    }

    std::vector<VariableAssignment *> CommonStepType::getVariableAssignments() {
        return this->varAssignments;
    }

    ParameterEstimation::ParameterEstimation(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void ParameterEstimation::parse(PharmMLReader &reader, xml::Node node) {
        // Get SymbRef (parameter to estimate)
        xml::Node ref_node = reader.getSingleElement(node, "./ct:SymbRef");
        this->symbRef = new SymbRef(reader, ref_node);

        // Get initial estimate
        xml::Node init_node = reader.getSingleElement(node, "./msteps:InitialEstimate");
        if (init_node.exists()) {
            this->fixed = (init_node.getAttribute("fixed").getValue() == "true") ? true : false;
            xml::Node tree = init_node.getChild();
            this->init = AstNode::create(reader, tree);
        }

        // Get lower bound
        xml::Node lbnd_node = reader.getSingleElement(node, "./msteps:LowerBound");
        if (lbnd_node.exists()) {
            xml::Node tree = lbnd_node.getChild();
            this->loBound = AstNode::create(reader, tree);
        }

        // Get upper bound
        xml::Node ubnd_node = reader.getSingleElement(node, "./msteps:UpperBound");
        if (ubnd_node.exists()) {
            xml::Node tree = ubnd_node.getChild();
            this->hiBound = AstNode::create(reader, tree);
        }
    }

    SymbRef *ParameterEstimation::getSymbRef() {
        return this->symbRef;
    }

    bool ParameterEstimation::isFixed() {
        return this->fixed;
    }

    bool ParameterEstimation::hasInitValue() {
        return (this->init != nullptr);
    }

    bool ParameterEstimation::hasLoBound() {
        return (this->loBound != nullptr);
    }

    bool ParameterEstimation::hasHiBound() {
        return (this->hiBound != nullptr);
    }

    std::shared_ptr<AstNode> ParameterEstimation::getInitValue() {
        return this->init;
    }

    std::shared_ptr<AstNode> ParameterEstimation::getLoBound() {
        return this->loBound;
    }

    std::shared_ptr<AstNode> ParameterEstimation::getHiBound() {
        return this->hiBound;
    }

    void ParameterEstimation::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    OperationProperty::OperationProperty(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->parse(reader, node);
    }

    void OperationProperty::parse(PharmMLReader &reader, xml::Node node) {
        this->name = node.getAttribute("name").getValue();
        xml::Node assign_node = reader.getSingleElement(node, "./ct:Assign");
        this->assignment = AstNode::create(reader, assign_node.getChild());
    }

    std::string OperationProperty::getName() {
        return this->name;
    }

    std::shared_ptr<AstNode> OperationProperty::getAssignment() {
        return this->assignment;
    }

    bool OperationProperty::isNamed(std::string case_insensitive_name) {
        return StringTools::iequals(this->name, case_insensitive_name);
    }

    // Convenience functions for simply accessing simple property values
    // FIXME: Below is properly setup by a postParse method (instead of AstAnalyzer usage each method)!
    bool OperationProperty::isInt() {
        pharmmlcpp::AstAnalyzer aa;
        this->assignment->accept(&aa);
        if (aa.getPureScalarInt()) {
            this->int_val = new int(aa.getPureScalarInt()->toInt());
            return true;
        } else {
            return false;
        }
    }

    bool OperationProperty::isReal() {
        pharmmlcpp::AstAnalyzer aa;
        this->assignment->accept(&aa);
        if (aa.getPureScalarReal()) {
            this->real_val = new double(aa.getPureScalarReal()->toDouble());
            return true;
        } else {
            return false;
        }
    }

    bool OperationProperty::isBool() {
        pharmmlcpp::AstAnalyzer aa;
        this->assignment->accept(&aa);
        if (aa.getPureScalarBool()) {
            this->bool_val = new bool(aa.getPureScalarBool()->toBool());
            return true;
        } else {
            return false;
        }
    }

    bool OperationProperty::isString() {
        pharmmlcpp::AstAnalyzer aa;
        this->assignment->accept(&aa);
        if (aa.getPureScalarString()) {
            this->string_val = new std::string(aa.getPureScalarString()->toString());
            return true;
        } else {
            return false;
        }
    }

    int OperationProperty::getInt() {
        if (this->int_val) {
            return *(this->int_val);
        } else {
            pharmmlcpp::AstAnalyzer aa;
            this->assignment->accept(&aa);
            return aa.getPureScalarInt()->toInt();
        }
    }

    double OperationProperty::getReal() {
        if (this->real_val) {
            return *(this->real_val);
        } else if (this->int_val) { // Seems reasonable to cast integer properties as doubles if requested
            return *(this->int_val);
        } else {
            pharmmlcpp::AstAnalyzer aa;
            this->assignment->accept(&aa);
            return aa.getPureScalarReal()->toDouble();
        }
    }

    bool OperationProperty::getBool() {
        if (this->bool_val) {
            return *(this->bool_val);
        } else {
            pharmmlcpp::AstAnalyzer aa;
            this->assignment->accept(&aa);
            return aa.getPureScalarBool()->toBool();
        }
    }

    std::string OperationProperty::getString() {
        if (this->string_val) {
            return *(this->string_val);
        } else {
            pharmmlcpp::AstAnalyzer aa;
            this->assignment->accept(&aa);
            return aa.getPureScalarString()->toString();
        }
    }

    bool OperationProperty::isFoldedCaseString(std::string case_insensitive) {
         return StringTools::iequals(this->getString(), case_insensitive);
    }

    Algorithm::Algorithm(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->parse(reader, node);
    }

    void Algorithm::parse(PharmMLReader &reader, xml::Node node) {
        this->definition = node.getAttribute("definition").getValue();
        xml::Node name_node = reader.getSingleElement(node, "./ct:Name");
        if (name_node.exists()) {
            this->name = name_node.getText();
        }
        std::vector<xml::Node> prop_nodes = reader.getElements(node, "./msteps:Property");
        for (xml::Node prop_node : prop_nodes) {
            OperationProperty *prop = new OperationProperty(reader, prop_node);
            this->properties.push_back(prop);
        }
    }

    std::string Algorithm::getName() {
        return this->name;
    }

    std::string Algorithm::getDefinition() {
        return this->definition;
    }

    std::vector<OperationProperty *> Algorithm::getProperties() {
        return this->properties;
    }

    // Ask algorithm if (case-insensitively) named something
    bool Algorithm::isNamed(std::string case_insensitive_name) {
        return StringTools::iequals(this->name, case_insensitive_name);
    }

    // Ask algorithm if (case-insensitively) defined something
    bool Algorithm::isDefinedAs(std::string case_insensitive_def) {
        return StringTools::iequals(this->definition, case_insensitive_def);
    }

    Operation::Operation(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->parse(reader, node);
    }

    void Operation::parse(PharmMLReader &reader, xml::Node node) {
        // Get attributes (opType restrictions: only differences between est, sim and opt)
        this->order = std::stoi(node.getAttribute("order").getValue());
        this->type = node.getAttribute("opType").getValue();

        // Get name, properties and algorithm
        xml::Node name_node = reader.getSingleElement(node, "./ct:Name");
        if (name_node.exists()) {
            this->name = name_node.getText();
        }
        std::vector<xml::Node> prop_nodes = reader.getElements(node, "./msteps:Property");
        for (xml::Node prop_node : prop_nodes) {
            OperationProperty *prop = new OperationProperty(reader, prop_node);
            this->properties.push_back(prop);
        }
        xml::Node algo_node = reader.getSingleElement(node, "./msteps:Algorithm");
        if (algo_node.exists()) {
            this->algorithm = new Algorithm(reader, algo_node);
        }
    }

    int Operation::getOrder() {
        return this->order;
    }

    std::string Operation::getType() {
        return this->type;
    }

    std::string Operation::getName() {
        return this->name;
    }

    std::vector<OperationProperty *> Operation::getProperties() {
        return this->properties;
    }

    Algorithm *Operation::getAlgorithm() {
        return this->algorithm;
    }

    EstimationStep::EstimationStep(PharmMLReader &reader, xml::Node node) : CommonStepType(reader, node) {
        this->setXMLNode(node);
        this->parse(reader, node);
    }

    void EstimationStep::parse(PharmMLReader &reader, xml::Node node) {
        // Get parameter estimation settings
        std::vector<xml::Node> param_nodes = reader.getElements(node, "./msteps:ParametersToEstimate/msteps:ParameterEstimation");
        for (xml::Node param_node : param_nodes) {
            ParameterEstimation *param = new ParameterEstimation(reader, param_node);
            this->parameterEstimations.push_back(param);
        }

        // Get Operation's
        std::vector<xml::Node> op_nodes = reader.getElements(node, "./msteps:Operation");
        for (xml::Node op_node : op_nodes) {
            Operation *operation = new Operation(reader, op_node);
            this->operations.push_back(operation);
        }
    }

    std::vector<ParameterEstimation *> EstimationStep::getParameters() {
        return this->parameterEstimations;
    }

    std::vector<Operation *> EstimationStep::getOperations() {
        return this->operations;
    }

    // TODO: Move to CommonStepType (with more general name)? How are initials encoded in SimulationStep?
    ParameterEstimation *EstimationStep::getParameterEstimation(Symbol *symbol) {
        for (ParameterEstimation *par_est : this->parameterEstimations) {
            bool depends_on_symbol = par_est->referencedSymbols.hasSymbol(symbol);
            if (depends_on_symbol) {
                return par_est;
            }
        }
        return nullptr;
    }

    SimulationStep::SimulationStep(PharmMLReader &reader, xml::Node node) : CommonStepType(reader, node) {
        this->parse(reader, node);
    }

    void SimulationStep::parse(PharmMLReader &reader, xml::Node node) {
        // TODO: Simulation support!

        // Get Operation's
        std::vector<xml::Node> op_nodes = reader.getElements(node, "./msteps:Operation");
        for (xml::Node op_node : op_nodes) {
            Operation *operation = new Operation(reader, op_node);
            this->operations.push_back(operation);
        }
    }

    std::vector<Operation *> SimulationStep::getOperations() {
        return this->operations;
    }

    OptimiseOn::OptimiseOn(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void OptimiseOn::parse(PharmMLReader &reader, xml::Node node) {
        // Get boolean options
        if (reader.getSingleElement(node, "./msteps:ArmSize").exists()) {
            armSize = false;
        }
        if (reader.getSingleElement(node, "./msteps:DoseAmount").exists()) {
            doseAmount = false;
        }
        if (reader.getSingleElement(node, "./msteps:DosingTimes").exists()) {
            dosingTimes = false;
        }
        if (reader.getSingleElement(node, "./msteps:Duration").exists()) {
            duration = false;
        }
        if (reader.getSingleElement(node, "./msteps:NumberArms").exists()) {
            numberArms = false;
        }
        if (reader.getSingleElement(node, "./msteps:NumberSamples").exists()) {
            numberSamples = false;
        }
        if (reader.getSingleElement(node, "./msteps:NumberTimes").exists()) {
            numberTimes = false;
        }
        if (reader.getSingleElement(node, "./msteps:ObservationTimes").exists()) {
            observationTimes = true;
        }

        // Get symbol references
        std::vector<xml::Node> symb_nodes = reader.getElements(node, "./ct:SymbRef");
        for (xml::Node symb_node : symb_nodes) {
            SymbRef *symbRef = new SymbRef(reader, symb_node);
            this->symbols.push_back(symbRef);
        }
    }

    OptimalDesignStep::OptimalDesignStep(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->parse(reader, node);
    }

    void OptimalDesignStep::parse(PharmMLReader &reader, xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        // Get what to optimize on
        xml::Node opt_on_node = reader.getSingleElement(node, "./msteps:OptimiseOn");
        if (opt_on_node.exists()) {
            this->optOn = new OptimiseOn(reader, opt_on_node);
        }

        // Get parameters to estimate
        std::vector<xml::Node> param_nodes = reader.getElements(node, "./msteps:ParametersToEstimate/msteps:ParameterEstimation");
        for (xml::Node param_node : param_nodes) {
            ParameterEstimation *param = new ParameterEstimation(reader, param_node);
            this->parameterEstimations.push_back(param);
        }

        // Get Operation's
        std::vector<xml::Node> op_nodes = reader.getElements(node, "./msteps:Operation");
        for (xml::Node op_node : op_nodes) {
            Operation *operation = new Operation(reader, op_node);
            this->operations.push_back(operation);
        }

        // TODO: Get more optimal design stuff
    }

    std::vector<ParameterEstimation *> OptimalDesignStep::getParameters() {
        return this->parameterEstimations;
    }

    std::vector<Operation *> OptimalDesignStep::getOperations() {
        return this->operations;
    }

    ParameterEstimation *OptimalDesignStep::getParameterEstimation(Symbol *symbol) {
        for (ParameterEstimation *par_est : this->parameterEstimations) {
            bool depends_on_symbol = par_est->referencedSymbols.hasSymbol(symbol);
            if (depends_on_symbol) {
                return par_est;
            }
        }
        return nullptr;
    }

    ModellingSteps::ModellingSteps(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->xml_node = node;
        this->parse(reader, node);
    }

    void ModellingSteps::parse(PharmMLReader &reader, xml::Node node) {
        // Get target tools
        std::vector<xml::Node> tool_nodes = reader.getElements(node, "./msteps:TargetTool");
        for (xml::Node tool_node : tool_nodes) {
            TargetTool *tool = new TargetTool(reader, tool_node);
            this->tools.push_back(tool);
        }

        // Get modelling steps (estimation and simulation)
        std::vector<xml::Node> estep_nodes = reader.getElements(node, "./msteps:EstimationStep");
        std::vector<xml::Node> sstep_nodes = reader.getElements(node, "./msteps:SimulationStep");
        for (xml::Node estep_node : estep_nodes) {
            EstimationStep *estep = new EstimationStep(reader, estep_node);
            this->estSteps.push_back(estep);
        }
        for (xml::Node sstep_node : sstep_nodes) {
            SimulationStep *sstep = new SimulationStep(reader, sstep_node);
            this->simSteps.push_back(sstep);
        }

        // Get optimal design steps
        std::vector<xml::Node> ostep_nodes = reader.getElements(node, "./msteps:OptimalDesignStep");
        for (xml::Node ostep_node : ostep_nodes) {
            OptimalDesignStep *ostep = new OptimalDesignStep(reader, ostep_node);
            this->optSteps.push_back(ostep);
        }

        // Get step dependencies
        xml::Node step_dep_node = reader.getSingleElement(node, "./msteps:StepDependencies");
        if (step_dep_node.exists()) {
            // TODO: Support this
        }
    }

    void ModellingSteps::setupRefererSymbRefs(SymbolGathering &gathering) {
        for (pharmmlcpp::EstimationStep *est_step : this->getEstimationSteps()) {
            std::vector<ParameterEstimation *> est_params = est_step->getParameters();
            for (pharmmlcpp::ParameterEstimation *est_param : est_params) {
                Symbol *found_symbol = gathering.getSymbol(est_param->getSymbRef()->getBlkIdRef(), est_param->getSymbRef()->getSymbIdRef());
                est_param->addReference(found_symbol);
                est_param->getSymbRef()->setSymbol(found_symbol);
            }
        }
        //for (pharmmlcpp::SimulationStep *sim_step : this->getSimulationSteps()) {
            // TODO: Fill SimulationSteps
        //}
        for (pharmmlcpp::OptimalDesignStep *opt_step : this->getOptimalDesignSteps()) {
            std::vector<ParameterEstimation *> opt_params = opt_step->getParameters();
            for (pharmmlcpp::ParameterEstimation *opt_param : opt_params) {
                Symbol *found_symbol = gathering.getSymbol(opt_param->getSymbRef()->getBlkIdRef(), opt_param->getSymbRef()->getSymbIdRef());
                opt_param->addReference(found_symbol);
                opt_param->getSymbRef()->setSymbol(found_symbol);
            }
        }
    }

    std::vector<EstimationStep *> ModellingSteps::getEstimationSteps() {
        return this->estSteps;
    }

    std::vector<SimulationStep *> ModellingSteps::getSimulationSteps() {
        return this->simSteps;
    }

    std::vector<OptimalDesignStep *> ModellingSteps::getOptimalDesignSteps() {
        return this->optSteps;
    }

    void ModellingSteps::update() {
        xml::Node ms("ModellingSteps");
        ms.setAttribute("xmlns", "http://www.pharmml.org/pharmml/0.8/ModellingSteps");

        this->xml_node.replaceNode(ms);
    }
}
