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
#include <PharmML/Model.h>
#include <PharmML/PharmMLContext.h>
#include <xml/xml.h>
#include <PharmML/TrialDesign.h>

namespace PharmML
{
    void Model::parse(xml::Node node) {
        xml::Node iv = this->context->getSingleElement(node, "/x:PharmML/x:IndependentVariable");
        if (iv.exists()) {
            this->IndependentVariable = new PharmML::IndependentVariable(this->context, iv);
        }

        xml::Node mdef_node = this->context->getSingleElement(node, "/x:PharmML/mdef:ModelDefinition");
        if (mdef_node.exists()) {
            this->ModelDefinition = new PharmML::ModelDefinition(this->context, mdef_node);
        }

        std::vector<xml::Node> function_nodes = this->context->getElements(node, "/x:PharmML/ct:FunctionDefinition");
        for (xml::Node n : function_nodes) {
            this->FunctionDefinitions.push_back(new PharmML::FunctionDefinition(this->context, n));
        }

        xml::Node design_node = this->context->getSingleElement(node, "/x:PharmML/design:TrialDesign");
        if (design_node.exists()) {
            this->TrialDesign = new PharmML::TrialDesign(this->context, design_node);
        }

        xml::Node msteps_node = this->context->getSingleElement(node, "/x:PharmML/msteps:ModellingSteps");
        if (msteps_node.exists()) {
            this->ModellingSteps = new PharmML::ModellingSteps(this->context, msteps_node);
        }

        // Build consolidator object
        this->consolidator = new CPharmML::Consolidator(this->context, this);

        this->postParse();
    }

    void Model::postParse() {
        this->setupObjects();
    }

    Model::Model(const char *filename) {
        this->context = new PharmMLContext(filename, this);
        this->parse(this->context->getRoot());
        this->context->fillSymbRefs();
    }

    Model::~Model() {
        delete context;
    }

    void Model::write(const char *filename) {
        this->context->write(filename);
    }

    PharmML::IndependentVariable *Model::getIndependentVariable() {
        return this->IndependentVariable;
    }

    std::vector<PharmML::FunctionDefinition *> Model::getFunctionDefinitions() {
        return this->FunctionDefinitions;
    }

    PharmML::ModelDefinition *Model::getModelDefinition() {
        return this->ModelDefinition;
    }

    PharmML::TrialDesign *Model::getTrialDesign() {
        return this->TrialDesign;
    }

    PharmML::ModellingSteps *Model::getModellingSteps() {
        return this->ModellingSteps;
    }

    CPharmML::Consolidator *Model::getConsolidator() {
        return this->consolidator;
    }

    // Print an error for duplicate oid
    void Model::duplicateOidError(const std::string &oid, PharmML::PharmMLSection *section) {
        this->context->logger.error("Duplicate oid '" + oid + "'", section);
    }

    // Gather all Objects and setup ObjectRefs 
    void Model::setupObjects() {
        std::unordered_set<std::string> allOids;

        PharmML::TrialDesign *td = this->getTrialDesign();

        if (td) {
            Arms *arms = td->getArms();
            if (arms) {
                for (Arm *arm : arms->getArms()) {
                    // Check if oid already exists
                    if (allOids.count(arm->getOid()) == 1) {
                        this->duplicateOidError(arm->getOid(), arm);
                    }
                    allOids.insert(arm->getOid());
                    this->allObjects.insert(arm);
                }
            }
            Observations *observations = td->getObservations();
            if (observations) {
                for (Observation *observation : observations->getObservations()) {
                    if (allOids.count(observation->getOid()) == 1) {
                        this->duplicateOidError(observation->getOid(), observation);
                    }
                    allOids.insert(observation->getOid());
                    this->allObjects.insert(observation);
                }
                for (IndividualObservations *observation : observations->getIndividualObservations()) {
                    if (allOids.count(observation->getOid()) == 1) {
                        this->duplicateOidError(observation->getOid(), observation);
                    }
                    allOids.insert(observation->getOid());
                    this->allObjects.insert(observation);
                }
            }
            Interventions *interventions = td->getInterventions();
            if (interventions) {
                for (Administration *admin : interventions->getAdministrations()) {
                    if (allOids.count(admin->getOid()) == 1) {
                        this->duplicateOidError(admin->getOid(), admin);
                    }
                    allOids.insert(admin->getOid());
                    this->allObjects.insert(admin);
                }
            }
        }

        // Obtain a map from all oids to Objects. Will be used to populate ObjectRefs
        std::unordered_map<std::string, PharmML::Object *> oidMap;
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

    void Model::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        this->ModellingSteps->gatherSymbRefs(symbolMap);
        this->ModelDefinition->gatherSymbRefs(symbolMap);
        this->TrialDesign->gatherSymbRefs(symbolMap);
        for (PharmML::FunctionDefinition *fun : this->FunctionDefinitions) {
            fun->gatherSymbRefs(symbolMap);
        }
    }
}
