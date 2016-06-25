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

#include <iostream>
#include <PharmML/PharmML.h>
#include <PharmML/PharmMLContext.h>
#include <xml/xml.h>
#include <PharmML/TrialDesign.h>

namespace pharmmlcpp
{
    PharmML::PharmML(const char *filename) {
        PharmMLReader reader(filename);
        this->parse(reader, reader.doc.getRoot());
    }

    void PharmML::parse(PharmMLReader &reader, xml::Node node) {
        this->context = new PharmMLContext(reader);
        xml::Node iv = reader.getSingleElement(node, "/x:PharmML/x:IndependentVariable");
        if (iv.exists()) {
            this->independentVariable = new IndependentVariable(this->context, iv);
        }

        xml::Node mdef_node = reader.getSingleElement(node, "/x:PharmML/mdef:ModelDefinition");
        if (mdef_node.exists()) {
            this->modelDefinition = new ModelDefinition(this->context, mdef_node);
        }

        std::vector<xml::Node> function_nodes = reader.getElements(node, "/x:PharmML/ct:FunctionDefinition");
        for (xml::Node n : function_nodes) {
            this->functionDefinitions.push_back(new FunctionDefinition(this->context, n));
        }

        xml::Node design_node = reader.getSingleElement(node, "/x:PharmML/design:TrialDesign");
        if (design_node.exists()) {
            this->trialDesign = new TrialDesign(this->context, design_node);
        }

        xml::Node msteps_node = reader.getSingleElement(node, "/x:PharmML/msteps:ModellingSteps");
        if (msteps_node.exists()) {
            this->modellingSteps = new ModellingSteps(this->context, msteps_node);
        }

        // TODO: This an be moved into postParse when the consolidator call below is removed
        this->setupSymbols();

        // Build consolidator object
        this->consolidator = new CPharmML::Consolidator(this->context, this);

        this->postParse();
    }

    void PharmML::postParse() {
        this->setupObjects();

        /* FIXME: Dirty hack to avoid crash because PKMacros::postParse() invokes name-generation of macros
         * which in turn uses names of refered Symbol's. Guess who sets SymbRef's? The consolidator, after XML
         * read and object construction... I.e., postParse() CANNOT be called in constructor of PKMacros.
         * This needs a good solution. Quickly. */
        PKMacros *pk_macros = this->modelDefinition->getStructuralModel()->getPKMacros();
        if (pk_macros) {
            pk_macros->postParse();
            this->setupPKMacros();
        }
    }

    PharmML::~PharmML() {
        delete context;
    }

    void PharmML::write(std::string filename) {
        this->context->doc.write(filename);
    }

    pharmmlcpp::IndependentVariable *PharmML::getIndependentVariable() {
        return this->independentVariable;
    }

    std::vector<pharmmlcpp::FunctionDefinition *> PharmML::getFunctionDefinitions() {
        return this->functionDefinitions;
    }

    pharmmlcpp::FunctionDefinition *PharmML::resolveFunctionCall(pharmmlcpp::FunctionCall *functionCall) {
        for (FunctionDefinition *FunctionDefinition : this->functionDefinitions) {
            if (functionCall->getFunction()->getSymbol() == FunctionDefinition) {
                return FunctionDefinition;
            }
        }
        return nullptr;
    }

    ModelDefinition *PharmML::getModelDefinition() {
        return this->modelDefinition;
    }

    TrialDesign *PharmML::getTrialDesign() {
        return this->trialDesign;
    }

    ModellingSteps *PharmML::getModellingSteps() {
        return this->modellingSteps;
    }

    CPharmML::Consolidator *PharmML::getConsolidator() {
        return this->consolidator;
    }

    // Check if oid already exists
    void PharmML::checkAndAddOid(std::unordered_set<std::string> &allOids, Object *object, PharmMLSection *section) {
        if (allOids.count(object->getOid()) == 1) {
            this->context->logger.error("Duplicate oid '" + object->getOid() + "'", section);
        }
        allOids.insert(object->getOid());
        this->allObjects.insert(object);
    }

    // Gater all Symbols and setup SymbolRefs and referencedSymbols
    void PharmML::setupSymbols() {
        SymbolGathering gathering;
        pharmmlcpp::ModelDefinition *mdef = this->getModelDefinition();

        // Gather all symbols
        if (mdef->getParameterModel()) {
            mdef->getParameterModel()->gatherSymbols(gathering);
        }
        mdef->getStructuralModel()->gatherSymbols(gathering);
        for (VariabilityModel *vmod : mdef->getVariabilityModels()) {
            vmod->gatherSymbols(gathering);
        }
        if (mdef->getCovariateModel()) {
            mdef->getCovariateModel()->gatherSymbols(gathering);
        }
        mdef->getObservationModel()->gatherSymbols(gathering);
        gathering.globalBlock();
        if (this->independentVariable) {
            gathering.addSymbol(this->independentVariable);
        }
        for (FunctionDefinition *fdef : this->getFunctionDefinitions()) {
            gathering.addSymbol(fdef);
            for (FunctionArgumentDefinition *farg : fdef->getArguments()) {
                gathering.addSymbol(farg);
            }
        }
        if (this->getTrialDesign()) {       // Assume global namespace for DesignParameters
            this->getTrialDesign()->gatherSymbols(gathering);
        }

        this->allSymbols = gathering.getAllSymbols();

        gathering.setupAllSymbRefs();

        // Handle referrers that are not symbols
        this->setupRefererSymbRefs(gathering);

        // Setup TargetMapping's (refers to Symbol's via strings)
        this->setupTargetMappings(gathering);
    }

    // Gather all Objects and setup ObjectRefs 
    void PharmML::setupObjects() {
        std::unordered_set<std::string> allOids;

        pharmmlcpp::TrialDesign *td = this->getTrialDesign();

        if (td) {
            Arms *arms = td->getArms();
            if (arms) {
                for (Arm *arm : arms->getArms()) {
                    this->checkAndAddOid(allOids, arm, arm);
                }
            }
            Observations *observations = td->getObservations();
            if (observations) {
                for (Observation *observation : observations->getObservations()) {
                    this->checkAndAddOid(allOids, observation, observation);
                }
                for (IndividualObservations *observation : observations->getIndividualObservations()) {
                    this->checkAndAddOid(allOids, observation, observation);
                }
            }
            Interventions *interventions = td->getInterventions();
            if (interventions) {
                for (Administration *admin : interventions->getAdministrations()) {
                    this->checkAndAddOid(allOids, admin, admin);
                }
                for (InterventionsCombination *comb : interventions->getInterventionsCombinations()) {
                    this->checkAndAddOid(allOids, comb, comb);
                }
            }
        }

        // Obtain a map from all oids to Objects. Will be used to populate ObjectRefs
        std::unordered_map<std::string, pharmmlcpp::Object *> oidMap;
        for (Object *object : this->allObjects) {
            oidMap[object->getOid()] = object;
        }

        for (Object *object : this->allObjects) {
            object->gatherObjectRefs(oidMap);
        }

        // Populate ObjectReferer ObjectRefs
        if (td) {
            Interventions *interventions = td->getInterventions();
            if (interventions) {
                for (IndividualAdministration *ia : interventions->getIndividualAdministrations()) {
                    ia->gatherObjectRefs(oidMap);
                }
            }
        }
    }

    // Gather all referable PK macros and setup TargetMapping's mapping these
    void PharmML::setupPKMacros() {
        // Gather all macros
        MacroGathering gathering;
        pharmmlcpp::StructuralModel *sm = this->getModelDefinition()->getStructuralModel();
        if (sm) {
            sm->gatherMacros(gathering);
        }

        // Setup TargetMapping's (refers to PKMacro's via strings)
        this->setupTargetMappings(gathering);
    }

    void PharmML::setupRefererSymbRefs(SymbolGathering &gathering) {
        if (this->modellingSteps) {
            this->modellingSteps->setupRefererSymbRefs(gathering);
        }
        this->modelDefinition->setupRefererSymbRefs(gathering);
        if (this->trialDesign) {
            this->trialDesign->setupRefererSymbRefs(gathering);
        }
    }

    void PharmML::setupTargetMappings(SymbolGathering &gathering) {
        if (this->trialDesign) {
            this->trialDesign->setupTargetMappings(gathering);
        }
    }

    void PharmML::setupTargetMappings(MacroGathering &gathering) {
        if (this->trialDesign) {
            this->trialDesign->setupTargetMappings(gathering);
        }
    }

    void PharmML::setSymbolNamer(SymbolNamer *namer) {
        for (Symbol *symbol : this->allSymbols) {
            symbol->setSymbolNamer(namer);
        }
    }
            
    // FIXME: Only here to be able to create classes (that demands a context) in unit tests!
    PharmMLContext *PharmML::getContext() {
        return this->context;
    }
}
