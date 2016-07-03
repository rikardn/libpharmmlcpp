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
    TargetTool::TargetTool(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void TargetTool::parse(xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        // Get target tool name
        xml::Node name_node = this->context->getSingleElement(node, "./msteps:TargetToolName");
        this->name = name_node.getText();

        // Get column mappings
        std::vector<xml::Node> col_maps = this->context->getElements(node, "./ds:ColumnMapping");
        for (xml::Node col_map : col_maps) {
            pharmmlcpp::ColumnMapping *col = new pharmmlcpp::ColumnMapping(this->context, col_map);
            this->columnMappings.push_back(col);
        }

        // Get target tool data
        std::vector<xml::Node> tool_data_nodes = this->context->getElements(node, "./ds:TargetToolData");
        //for (xml::Node tool_data_node : tool_data_nodes) {
            // TODO: Support this
        //}

        // TODO: Support CodeInjection
    }

    VariableAssignment::VariableAssignment(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void VariableAssignment::parse(xml::Node node) {
        // Get SymbRef
        xml::Node ref_node = this->context->getSingleElement(node, "./ct:SymbRef");
        this->symbRef = new SymbRef(ref_node);

        // Get the assignment of that SymbRef
        xml::Node assign = this->context->getSingleElement(node, "./ct:Assign");
        xml::Node tree = assign.getChild();
        this->assignment = this->context->factory.create(tree);
    }

    SymbRef *VariableAssignment::getSymbRef() {
        return this->symbRef;
    }

    AstNode *VariableAssignment::getAssignment() {
        return this->assignment;
    }

    CommonStepType::CommonStepType(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void CommonStepType::parse(xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        // Get software settings
        std::vector<xml::Node> sw_set_nodes = this->context->getElements(node, "./msteps:SoftwareSettings");
        for (xml::Node sw_set_node : sw_set_nodes) {
            xml::Node ext_file_node = this->context->getSingleElement(sw_set_node, "./ds:File");
            pharmmlcpp::ExternalFile *ext_file = new ExternalFile(this->context, ext_file_node);
            this->softwareSettings.push_back(ext_file);
        }

        // Get output files
        std::vector<xml::Node> out_nodes = this->context->getElements(node, "./ds:OutputFiles");
        for (xml::Node out_node : out_nodes) {
            pharmmlcpp::ExternalFile *ext_file = new ExternalFile(this->context, out_node);
            this->outputFiles.push_back(ext_file);
        }

        // Get target tool reference
        xml::Node tool_ref_node = this->context->getSingleElement(node, "./msteps:TargetToolReference");
        if (tool_ref_node.exists()) {
            this->targetToolRef = tool_ref_node.getChild().getAttribute("oidRef").getValue();
        }

        // Get external dataset reference
        xml::Node ds_ref_node = this->context->getSingleElement(node, "./msteps:ExternalDataSetReference/ct:OidRef");
        if (ds_ref_node.exists()) {
            this->extDatasetRef = ds_ref_node.getAttribute("oidRef").getValue();
        }

        // Get interventions reference
        xml::Node int_ref_node = this->context->getSingleElement(node, "./msteps:InterventionReference");
        if (int_ref_node.exists()) {
            std::vector<xml::Node> ref_nodes = this->context->getElements(int_ref_node, "./ct:OidRef");
            for (xml::Node ref_node : ref_nodes) {
                std::string ref = ref_node.getAttribute("oidRef").getValue();
                this->interventionsRefs.push_back(ref);
            }
        }

        // Get observations reference
        xml::Node obs_ref_node = this->context->getSingleElement(node, "./msteps:ObservationsReference");
        if (obs_ref_node.exists()) {
            std::vector<xml::Node> ref_nodes = this->context->getElements(int_ref_node, "./ct:OidRef");
            for (xml::Node ref_node : ref_nodes) {
                std::string ref = ref_node.getAttribute("oidRef").getValue();
                this->observationsRefs.push_back(ref);
            }
        }

        // Get variable assignments
        std::vector<xml::Node> assign_nodes = this->context->getElements(node, "./ct:VariableAssignment");
        for (xml::Node assign_node : assign_nodes) {
            pharmmlcpp::VariableAssignment *var_assign = new VariableAssignment(this->context, assign_node);
            this->varAssignments.push_back(var_assign);
        }
    }

    std::string CommonStepType::getOid() {
        return this->oid;
    }

    std::vector<pharmmlcpp::ExternalFile *> CommonStepType::getSoftwareSettingsFiles() {
        return this->softwareSettings;
    }

    std::vector<pharmmlcpp::ExternalFile *> CommonStepType::getOutputFiles() {
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

    std::vector<pharmmlcpp::VariableAssignment *> CommonStepType::getVariableAssignments() {
        return this->varAssignments;
    }

    ParameterEstimation::ParameterEstimation(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void ParameterEstimation::parse(xml::Node node) {
        // Get SymbRef (parameter to estimate)
        xml::Node ref_node = this->context->getSingleElement(node, "./ct:SymbRef");
        this->symbRef = new SymbRef(ref_node);

        // Get initial estimate
        xml::Node init_node = this->context->getSingleElement(node, "./msteps:InitialEstimate");
        if (init_node.exists()) {
            this->fixed = (init_node.getAttribute("fixed").getValue() == "true") ? true : false;
            xml::Node tree = init_node.getChild();
            this->init = this->context->factory.create(tree);
        }

        // Get lower bound
        xml::Node lbnd_node = this->context->getSingleElement(node, "./msteps:LowerBound");
        if (lbnd_node.exists()) {
            xml::Node tree = lbnd_node.getChild();
            this->loBound = this->context->factory.create(tree);
        }

        // Get upper bound
        xml::Node ubnd_node = this->context->getSingleElement(node, "./msteps:UpperBound");
        if (ubnd_node.exists()) {
            xml::Node tree = ubnd_node.getChild();
            this->hiBound = this->context->factory.create(tree);
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

    AstNode *ParameterEstimation::getInitValue() {
        return this->init;
    }

    AstNode *ParameterEstimation::getLoBound() {
        return this->loBound;
    }

    AstNode *ParameterEstimation::getHiBound() {
        return this->hiBound;
    }

    void ParameterEstimation::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    OperationProperty::OperationProperty(PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
    }

    void OperationProperty::parse(xml::Node node) {
        this->name = node.getAttribute("name").getValue();
        xml::Node assign_node = this->context->getSingleElement(node, "./ct:Assign");
        this->assignment = this->context->factory.create(assign_node.getChild());
    }

    std::string OperationProperty::getName() {
        return this->name;
    }

    AstNode *OperationProperty::getAssignment() {
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

    Algorithm::Algorithm(PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
    }

    void Algorithm::parse(xml::Node node) {
        this->definition = node.getAttribute("definition").getValue();
        xml::Node name_node = this->context->getSingleElement(node, "./ct:Name");
        if (name_node.exists()) {
            this->name = name_node.getText();
        }
        std::vector<xml::Node> prop_nodes = this->context->getElements(node, "./msteps:Property");
        for (xml::Node prop_node : prop_nodes) {
            OperationProperty *prop = new OperationProperty(this->context, prop_node);
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

    Operation::Operation(PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
    }

    void Operation::parse(xml::Node node) {
        // Get attributes (opType restrictions: only differences between est, sim and opt)
        this->order = std::stoi(node.getAttribute("order").getValue());
        this->type = node.getAttribute("opType").getValue();

        // Get name, properties and algorithm
        xml::Node name_node = this->context->getSingleElement(node, "./ct:Name");
        if (name_node.exists()) {
            this->name = name_node.getText();
        }
        std::vector<xml::Node> prop_nodes = this->context->getElements(node, "./msteps:Property");
        for (xml::Node prop_node : prop_nodes) {
            OperationProperty *prop = new OperationProperty(this->context, prop_node);
            this->properties.push_back(prop);
        }
        xml::Node algo_node = this->context->getSingleElement(node, "./msteps:Algorithm");
        if (algo_node.exists()) {
            this->algorithm = new Algorithm(this->context, algo_node);
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

    EstimationStep::EstimationStep(PharmMLContext *context, xml::Node node) : CommonStepType(context, node) {
        this->context = context;
        this->parse(node);
    }

    void EstimationStep::parse(xml::Node node) {
        // Get parameter estimation settings
        std::vector<xml::Node> param_nodes = this->context->getElements(node, "./msteps:ParametersToEstimate/msteps:ParameterEstimation");
        for (xml::Node param_node : param_nodes) {
            ParameterEstimation *param = new ParameterEstimation(this->context, param_node);
            this->parameterEstimations.push_back(param);
        }

        // Get Operation's
        std::vector<xml::Node> op_nodes = this->context->getElements(node, "./msteps:Operation");
        for (xml::Node op_node : op_nodes) {
            Operation *operation = new Operation(this->context, op_node);
            this->operations.push_back(operation);
        }
    }

    std::vector<ParameterEstimation *> EstimationStep::getParameters() {
        return this->parameterEstimations;
    }

    std::vector<Operation *> EstimationStep::getOperations() {
        return this->operations;
    }

    SimulationStep::SimulationStep(PharmMLContext *context, xml::Node node) : CommonStepType(context, node) {
        this->context = context;
        this->parse(node);
    }

    void SimulationStep::parse(xml::Node node) {
        // TODO: Simulation support!

        // Get Operation's
        std::vector<xml::Node> op_nodes = this->context->getElements(node, "./msteps:Operation");
        for (xml::Node op_node : op_nodes) {
            Operation *operation = new Operation(this->context, op_node);
            this->operations.push_back(operation);
        }
    }

    std::vector<Operation *> SimulationStep::getOperations() {
        return this->operations;
    }

    OptimiseOn::OptimiseOn(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void OptimiseOn::parse(xml::Node node) {
        // Get boolean options
        if (this->context->getSingleElement(node, "./msteps:ArmSize").exists()) {
            armSize = false;
        }
        if (this->context->getSingleElement(node, "./msteps:DoseAmount").exists()) {
            doseAmount = false;
        }
        if (this->context->getSingleElement(node, "./msteps:DosingTimes").exists()) {
            dosingTimes = false;
        }
        if (this->context->getSingleElement(node, "./msteps:Duration").exists()) {
            duration = false;
        }
        if (this->context->getSingleElement(node, "./msteps:NumberArms").exists()) {
            numberArms = false;
        }
        if (this->context->getSingleElement(node, "./msteps:NumberSamples").exists()) {
            numberSamples = false;
        }
        if (this->context->getSingleElement(node, "./msteps:NumberTimes").exists()) {
            numberTimes = false;
        }
        if (this->context->getSingleElement(node, "./msteps:ObservationTimes").exists()) {
            observationTimes = true;
        }

        // Get symbol references
        std::vector<xml::Node> symb_nodes = this->context->getElements(node, "./ct:SymbRef");
        for (xml::Node symb_node : symb_nodes) {
            pharmmlcpp::SymbRef *symbRef = new pharmmlcpp::SymbRef(symb_node);
            this->symbols.push_back(symbRef);
        }

    }

    OptimalDesignStep::OptimalDesignStep(PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
    }

    void OptimalDesignStep::parse(xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        // Get what to optimize on
        xml::Node opt_on_node = this->context->getSingleElement(node, "./msteps:OptimiseOn");
        if (opt_on_node.exists()) {
            this->optOn = new OptimiseOn(this->context, opt_on_node);
        }

        // Get parameters to estimate
        std::vector<xml::Node> param_nodes = this->context->getElements(node, "./msteps:ParametersToEstimate/msteps:ParameterEstimation");
        for (xml::Node param_node : param_nodes) {
            ParameterEstimation *param = new ParameterEstimation(this->context, param_node);
            this->parameterEstimations.push_back(param);
        }

        // Get Operation's
        std::vector<xml::Node> op_nodes = this->context->getElements(node, "./msteps:Operation");
        for (xml::Node op_node : op_nodes) {
            Operation *operation = new Operation(this->context, op_node);
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

    ModellingSteps::ModellingSteps(PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->xml_node = node;
        this->parse(node);
    }

    void ModellingSteps::parse(xml::Node node) {
        // Get target tools
        std::vector<xml::Node> tool_nodes = this->context->getElements(node, "./msteps:TargetTool");
        for (xml::Node tool_node : tool_nodes) {
            TargetTool *tool = new TargetTool(this->context, tool_node);
            this->tools.push_back(tool);
        }

        // Get modelling steps (estimation and simulation)
        std::vector<xml::Node> estep_nodes = this->context->getElements(node, "./msteps:EstimationStep");
        std::vector<xml::Node> sstep_nodes = this->context->getElements(node, "./msteps:SimulationStep");
        for (xml::Node estep_node : estep_nodes) {
            EstimationStep *estep = new EstimationStep(this->context, estep_node);
            this->estSteps.push_back(estep);
        }
        for (xml::Node sstep_node : sstep_nodes) {
            SimulationStep *sstep = new SimulationStep(this->context, sstep_node);
            this->simSteps.push_back(sstep);
        }

        // Get optimal design steps
        std::vector<xml::Node> ostep_nodes = this->context->getElements(node, "./msteps:OptimalDesignStep");
        for (xml::Node ostep_node : ostep_nodes) {
            OptimalDesignStep *ostep = new OptimalDesignStep(this->context, ostep_node);
            this->optSteps.push_back(ostep);
        }

        // Get step dependencies
        xml::Node step_dep_node = this->context->getSingleElement(node, "./msteps:StepDependencies");
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
