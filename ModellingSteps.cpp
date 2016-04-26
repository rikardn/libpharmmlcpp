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

#include "PharmMLContext.h"
#include "ModellingSteps.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "xml.h"

namespace PharmML
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
            PharmML::ColumnMapping *col = new PharmML::ColumnMapping(this->context, col_map);
            this->columnMappings.push_back(col);
        }
        
        // Get target tool data
        std::vector<xml::Node> tool_data_nodes = this->context->getElements(node, "./ds:TargetToolData");
        for (xml::Node tool_data_node : tool_data_nodes) {
            // TODO: Support this
        }
        
        // TODO: Support CodeInjection
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
            PharmML::SymbRef *symbRef = new PharmML::SymbRef(symb_node);
            this->symbols.push_back(symbRef);
        }
        
    }
    
    OptimalDesignStep::OptimalDesignStep(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void OptimalDesignStep::parse(xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        // Get what to optimize on
        xml::Node opt_on_node = this->context->getSingleElement(node, "./msteps:OptimiseOn");
        if (opt_on_node.exists()) {
            this->optOn = new PharmML::OptimiseOn(this->context, opt_on_node);
        }
        
        // TODO: Get a lot of stuff here
    } 
    
    ModellingSteps::ModellingSteps(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->xml_node = node;
        this->parse(node);
    }

    void ModellingSteps::parse(xml::Node node) {
        // Get target tools
        std::vector<xml::Node> tool_nodes = this->context->getElements(node, "./msteps:TargetTool");
        for (xml::Node tool_node : tool_nodes) {
            TargetTool *tool = new PharmML::TargetTool(this->context, tool_node);
            this->tools.push_back(tool);
        }
        
        // Get common modelling steps
        std::vector<xml::Node> cstep_nodes = this->context->getElements(node, "./msteps:CommonModellingStep");
        for (xml::Node cstep_node : cstep_nodes) {
            // TODO: Support this
        }
        
        // Get optimal design steps
        std::vector<xml::Node> ostep_nodes = this->context->getElements(node, "./msteps:OptimalDesignStep");
        for (xml::Node ostep_node : ostep_nodes) {
            // TODO: Support this
        }
        
        // Get step dependencies
        xml::Node step_dep_node = this->context->getSingleElement(node, "./msteps:StepDependencies");
        if (step_dep_node.exists()) {
            // TODO: Support this
        }
    }

    void ModellingSteps::update() {
        xml::Node ms("ModellingSteps");
        ms.setAttribute("xmlns", "http://www.pharmml.org/pharmml/0.8/ModellingSteps");
        
        this->xml_node.replaceNode(ms);
    }
}
