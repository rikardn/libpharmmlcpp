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
#include <AST/AstTransformation.h>

namespace pharmmlcpp
{
    // Observation class
    Observation::Observation(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->parse(reader, node);
    }

    void Observation::parse(PharmMLReader &reader, xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();

        // Get (oid) observation references (to observations already defined)
        xml::Node reference = reader.getSingleElement(node, "./design:ObservationRef");
        if (reference.exists()) {
            this->oidRef = std::make_shared<ObjectRef>(reference);
        }

        // Get NumberTimes (what is this even?)
        xml::Node number = reader.getSingleElement(node, "./design:NumberTimes");
        if (number.exists()) {
            xml::Node assign = reader.getSingleElement(number, "./ct:Assign");
            xml::Node tree = assign.getChild();
            this->numberTimes = reader.factory.create(tree);
        }

        // Get observation times
        xml::Node times = reader.getSingleElement(node, "./design:ObservationTimes");
        if (times.exists()) {
            xml::Node assign = reader.getSingleElement(times, "./ct:Assign");
            xml::Node tree = assign.getChild();
            this->observationTimes = reader.factory.create(tree);
        }

        // Get continuous and discrete variable output(s)
        xml::Node continuous = reader.getSingleElement(node, "./design:Continuous");
        xml::Node discrete = reader.getSingleElement(node, "./design:Discrete");
        if (continuous.exists()) {
            std::vector<xml::Node> variables = reader.getElements(continuous, "./ct:SymbRef");
            for (xml::Node variable : variables) {
                this->continuousVariables.push_back(std::make_shared<SymbRef>(variable));
            }
        }
        if (discrete.exists()) {
            std::vector<xml::Node> variables = reader.getElements(discrete, "./ct:SymbRef");
            for (xml::Node variable : variables) {
                this->discreteVariables.push_back(std::make_shared<SymbRef>(variable));
            }
        }
    }

    void Observation::gatherObjectRefs(std::unordered_map<std::string, Object *> &oidMap) {
        if (this->oidRef) {
            this->oidRef->setObject(oidMap[this->oidRef->getOidRef()]);
        }
    }

    xml::Node Observation::xml() {
        // FIXME: Under construction
        xml::Node observation("Observation");
        observation.setAttribute("oid", this->oid);
        //    arm.addChild(seq->xml());
        return observation;
    }

    std::shared_ptr<ObjectRef> Observation::getOidRef() {
        return this->oidRef;
    }

    std::shared_ptr<AstNode> Observation::getNumberTimes() {
        return this->numberTimes;
    }

    void Observation::setNumberTimes(std::shared_ptr<AstNode> numberTimes) {
        // FIXME: Here we could immediately validate that numberTimes is a scalar integer
        this->numberTimes = numberTimes;
    }

    std::shared_ptr<AstNode> Observation::getObservationTimes() {
        return this->observationTimes;
    }

    void Observation::setObservationTimes(std::shared_ptr<AstNode> observationTimes) {
        this->observationTimes = observationTimes;
    }

    std::vector<std::shared_ptr<AstNode>> Observation::getObservationTimesAsVector() {
        return AstTransformation::toVector(this->observationTimes);
    }

    std::vector<std::shared_ptr<SymbRef>>& Observation::getContinuousVariables() {
        return this->continuousVariables;
    }

    std::vector<std::shared_ptr<SymbRef>>& Observation::getDiscreteVariables() {
        return this->discreteVariables;
    }

    void Observation::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void Observation::accept(ObjectVisitor *visitor) {
        visitor->visit(this);
    }

    // IndividualObservations class
    IndividualObservations::IndividualObservations(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->parse(reader, node);
    }

    void IndividualObservations::parse(PharmMLReader &reader, xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        // Get column mappings
        std::vector<xml::Node> mapping_nodes = reader.getElements(node, "./design:ColumnMapping");
        for (xml::Node mapping_node : mapping_nodes) {
            ColumnMapping *map = new ColumnMapping(reader, mapping_node);
            this->columnMappings.push_back(map);
        }

        // Get dataset
        xml::Node ds_node = reader.getSingleElement(node, "./ds:DataSet");
        pharmmlcpp::Dataset *ds = new Dataset(reader, ds_node);
        this->dataset = ds;

        // Check that the individual observation has an independent variable
        pharmmlcpp::DataColumn *col = ds->getIdvColumn();
        if (!col) {     // No idv column was found
            reader.logger.error("Missing idv column in IndividualObservations", this);
            // FIXME: What to do here?
        }
        // FIXME: Need to check ColumnMapping and IndependentVariables also
        // FIXME: What happens if there is no Column definitions and/or ColumnMapping. Error checking is hard!
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
    ObservationCombination::ObservationCombination(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void ObservationCombination::parse(PharmMLReader &reader, xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        // Get all observation combinations
        std::vector<xml::Node> single_observations = reader.getElements(node, "./design:Observations");
        for (xml::Node obs : single_observations) {
            std::vector<xml::Node> observation_refs = reader.getElements(obs, "./design:ObservationRef");
            for (xml::Node ref : observation_refs) {
                std::string oidRef = ref.getAttribute("oidRef").getValue();
                this->oidRefs.push_back(oidRef);
            }
        }

        // Get relative
        xml::Node relative = reader.getSingleElement(node, "./design:Relative");
        if (relative.exists()) {
            xml::Node assign = reader.getSingleElement(relative, "./ct:Assign");
            xml::Node tree = assign.getChild();
            this->relative = reader.factory.create(tree);
        }
    }

    std::string ObservationCombination::getOid() {
        return this->oid;
    }

    std::vector<std::string> ObservationCombination::getOidRefs() {
        return this->oidRefs;
    }

    std::shared_ptr<AstNode> ObservationCombination::getRelative() {
        return this->relative;
    }

    void ObservationCombination::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    // Observations class (contains objects of classes above)
    Observations::Observations(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void Observations::parse(PharmMLReader &reader, xml::Node node) {
        // Get design parameters
        // (mdef:DesignParameterType extends mdef:CommonParameterType which is close enough to class Variable for now)
        std::vector<xml::Node> design_parameters = reader.getElements(node, "./mdef:DesignParameter");
        for (xml::Node node : design_parameters) {
            Variable *parameter = new Variable(reader, node);
            this->designParameters.push_back(parameter);
        }

        // Get samplings to be generated by the simulation
        std::vector<xml::Node> sim_obs = reader.getElements(node, "./design:Observation");
        for (xml::Node node : sim_obs) {
            Observation *obs = new Observation(reader, node);
            this->simulationObservations.push_back(obs);
        }

        // Get samplings to use in dataset
        std::vector<xml::Node> data_obs = reader.getElements(node, "./design:IndividualObservations");
        for (xml::Node node : data_obs) {
            IndividualObservations *obs = new IndividualObservations(reader, node);
            this->datasetObservations.push_back(obs);
        }

        // TODO: Implement support for LookupTable according to schema
        std::vector<xml::Node> lookup_tables = reader.getElements(node, "./design:LookupTable");

        // Get observation combinations
        std::vector<xml::Node> obs_combinations = reader.getElements(node, "./design:ObservationsCombination");
        for (xml::Node node : obs_combinations) {
            ObservationCombination *combination = new ObservationCombination(reader, node);
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
