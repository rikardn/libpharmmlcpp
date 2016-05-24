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

#include "Observations.h"

namespace PharmML
{
    // Observation class
    Observation::Observation(PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
    }
    
    void Observation::parse(xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        
        // Get (oid) observation references (to observations already defined)
        xml::Node reference = this->context->getSingleElement(node, "./design:ObservationRef");
        if (reference.exists()) {
            this->oidRef = new ObjectRef(reference);
        }
        
        // Get NumberTimes (what is this even?)
        xml::Node number = this->context->getSingleElement(node, "./design:NumberTimes");
        if (number.exists()) {
            xml::Node assign = this->context->getSingleElement(number, "./ct:Assign");
            xml::Node tree = assign.getChild();
            this->number = this->context->factory.create(tree);
        }
        
        // Get observation times
        xml::Node times = this->context->getSingleElement(node, "./design:ObservationTimes");
        if (times.exists()) {
            xml::Node assign = this->context->getSingleElement(times, "./ct:Assign");
            xml::Node tree = assign.getChild();
            this->times = this->context->factory.create(tree);
        }
        
        // Get continuous and discrete variable output(s)
        xml::Node continuous = this->context->getSingleElement(node, "./design:Continuous");
        xml::Node discrete = this->context->getSingleElement(node, "./design:Discrete");
        if (continuous.exists()) {
            std::vector<xml::Node> variables = this->context->getElements(continuous, "./ct:SymbRef");
            for (xml::Node variable : variables) {
                SymbRef *var = new SymbRef(variable);
                this->continuousVariables.push_back(var);
            }
        }
        if (discrete.exists()) {
            std::vector<xml::Node> variables = this->context->getElements(discrete, "./ct:SymbRef");
            for (xml::Node variable : variables) {
                SymbRef *var = new SymbRef(variable);
                this->discreteVariables.push_back(var);
            }
        }
    }

    void Observation::gatherObjectRefs(std::unordered_map<std::string, Object *> &oidMap) {
        if (this->oidRef) {
            this->oidRef->setObject(oidMap[this->oidRef->getOidRef()]);
        }
    }

    ObjectRef* Observation::getOidRef() {
        return this->oidRef;
    }
    
    AstNode *Observation::getNumber() {
        return this->number;
    }
    
    AstNode *Observation::getTimes() {
        return this->times;
    }
    
    std::vector<PharmML::SymbRef *> Observation::getContinuousVariables() {
        return this->continuousVariables;
    }
    
    std::vector<PharmML::SymbRef *> Observation::getDiscreteVariables() {
        return this->discreteVariables;
    }
    
    void Observation::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
   
    void Observation::accept(ObjectVisitor *visitor) {
        visitor->visit(this);
    }

    // IndividualObservations class
    IndividualObservations::IndividualObservations(PharmML::PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
    }

    void IndividualObservations::parse(xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        // Get column mappings
        std::vector<xml::Node> mapping_nodes = this->context->getElements(node, "./design:ColumnMapping");
        for (xml::Node mapping_node : mapping_nodes) {
            ColumnMapping *map = new ColumnMapping(this->context, mapping_node);
            this->columnMappings.push_back(map);
        }
        
        // Get dataset
        xml::Node ds_node = this->context->getSingleElement(node, "./ds:DataSet");
        PharmML::Dataset *ds = new PharmML::Dataset(this->context, ds_node);
        this->dataset = ds;
    }

    xml::Node IndividualObservations::xml() {
        xml::Node io("IndividualObservations");
        io.setAttribute("oid", this->getOid());
        for (ColumnMapping *cm : this->getColumnMappings()) {
            io.addChild(cm->xml());
        }
        io.addChild(this->dataset->xml());
        return io;
    }
    
    std::vector<ColumnMapping *> IndividualObservations::getColumnMappings() {
        return this->columnMappings;
    }
    
    Dataset *IndividualObservations::getDataset() {
        return this->dataset;
    }
    
    void IndividualObservations::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void IndividualObservations::accept(ObjectVisitor *visitor) {
        visitor->visit(this);
    }

    // ObservationCombination class
    ObservationCombination::ObservationCombination(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }
    
    void ObservationCombination::parse(xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        // Get all observation combinations
        std::vector<xml::Node> single_observations = this->context->getElements(node, "./design:Observations");
        for (xml::Node obs : single_observations) {
            std::vector<xml::Node> observation_refs = this->context->getElements(obs, "./design:ObservationRef");
            for (xml::Node ref : observation_refs) {
                std::string oidRef = ref.getAttribute("oidRef").getValue();
                this->oidRefs.push_back(oidRef);
            }
        }
        
        // Get relative
        xml::Node relative = this->context->getSingleElement(node, "./design:Relative");
        if (relative.exists()) {
            xml::Node assign = this->context->getSingleElement(relative, "./ct:Assign");
            xml::Node tree = assign.getChild();
            this->relative = this->context->factory.create(tree);
        }
    }
    
    std::string ObservationCombination::getOid() {
        return this->oid;
    }
    
    std::vector<std::string> ObservationCombination::getOidRefs() {
        return this->oidRefs;
    }
    
    AstNode *ObservationCombination::getRelative() {
        return this->relative;
    }
    
    void ObservationCombination::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
    
    // Observations class (contains objects of classes above)
    Observations::Observations(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }
    
    void Observations::parse(xml::Node node) {
        // Get design parameters
        // (mdef:DesignParameterType extends mdef:CommonParameterType which is close enough to class Variable for now)
        std::vector<xml::Node> design_parameters = this->context->getElements(node, "./mdef:DesignParameter");
        for (xml::Node node : design_parameters) {
            Variable *parameter = new Variable(this->context, node);
            this->designParameters.push_back(parameter);
        }
        
        // Get samplings to be generated by the simulation
        std::vector<xml::Node> sim_obs = this->context->getElements(node, "./design:Observation");
        for (xml::Node node : sim_obs) {
            Observation *obs = new Observation(this->context, node);
            this->simulationObservations.push_back(obs);
        }
        
        // Get samplings to use in dataset
        std::vector<xml::Node> data_obs = this->context->getElements(node, "./design:IndividualObservations");
        for (xml::Node node : data_obs) {
            IndividualObservations *obs = new IndividualObservations(this->context, node);
            this->datasetObservations.push_back(obs);
        }
        
        // TODO: Implement support for LookupTable according to schema
        std::vector<xml::Node> lookup_tables = this->context->getElements(node, "./design:LookupTable");
        
        // Get observation combinations
        std::vector<xml::Node> obs_combinations = this->context->getElements(node, "./design:ObservationsCombination");
        for (xml::Node node : obs_combinations) {
            ObservationCombination *combination = new ObservationCombination(this->context, node);
            this->observationCombinations.push_back(combination);
        }
    }
    
    xml::Node Observations::xml() {
        xml::Node obs("Observations");
        for (IndividualObservations *io : this->getIndividualObservations()) {
            obs.addChild(io->xml());
        }
        return obs;
    }

    std::vector<Variable *> Observations::getDesignParameters() {
        return this->designParameters;
    }
    
    std::vector<Observation *> Observations::getObservations() { // Or getSimulationObservations()?
        return this->simulationObservations;
    }
    
    std::vector<IndividualObservations *> Observations::getIndividualObservations() { // Or getDatasetObservations()?
        return this->datasetObservations;
    }
    
    std::vector<ObservationCombination *> Observations::getObservationCombinations() {
        return this->observationCombinations;
    }
    
    void Observations::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
