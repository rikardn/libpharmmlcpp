#include "Observations.h"

namespace PharmML
{
    Observation::Observation(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }
    
    void Observation::parse(xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        
        // Get (oid) observation references (to observations already defined)
        xml::Node reference = this->context->getSingleElement(node, "./design:ObservationRef");
        if (reference.exists()) {
            this->oidRef = reference.getAttribute("oidRef").getValue();
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
    
    std::string Observation::getOid() {
        return this->oid;
    }
    
    std::string Observation::getOidRef() {
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
    
    void Observation::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
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
    
    void ObservationCombination::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
    
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
        
        // Get sampling
        std::vector<xml::Node> observations = this->context->getElements(node, "./design:Observation");
        for (xml::Node node : observations) {
            Observation *sampling = new Observation(this->context, node);
            this->observations.push_back(sampling);
        }
        
        // TODO: Implement support for IndividualObservations according to schema
        std::vector<xml::Node> data_observations = this->context->getElements(node, "./design:IndividualObservations");
        
        // TODO: Implement support for LookupTable according to schema
        std::vector<xml::Node> lookup_tables = this->context->getElements(node, "./design:LookupTable");
        
        // Get observation combinations
        std::vector<xml::Node> obs_combinations = this->context->getElements(node, "./design:ObservationsCombination");
        for (xml::Node node : obs_combinations) {
            ObservationCombination *combination = new ObservationCombination(this->context, node);
            this->observationCombinations.push_back(combination);
        }
    }
    
    std::vector<Variable *> Observations::getDesignParameters() {
        return this->designParameters;
    }
    
    std::vector<Observation *> Observations::getObservations() {
        return this->observations;
    }
    
    std::vector<ObservationCombination *> Observations::getObservationCombinations() {
        return this->observationCombinations;
    }
    
    void Observations::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
}
