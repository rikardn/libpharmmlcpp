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

#ifndef PHARMMLCPP_OBSERVATIONS_H_
#define PHARMMLCPP_OBSERVATIONS_H_

#include <xml/xml.h>
#include <PharmML/PharmMLContext.h>
#include <AST/AstNode.h>
#include <symbols/Variable.h>
#include <AST/symbols.h>
#include <PharmML/ColumnMapping.h>
#include <PharmML/Dataset.h>
#include <objects/Object.h>
#include <PharmML/PharmMLSection.h>
#include <visitors/ObjectVisitor.h>
//~ #include "ExternalDataset.h" // Check comments in this file

namespace pharmmlcpp
{
    /**
     * The Observation class defines one or multiple observations or samples
     * ObservationTimes is a vector (or scalar in case of one) of real or integer for the time points of the observations
     * NumberTimes is an integer scalar that gives a number of observation if the ObservationTimes themeselves are omitted
     *      as of PharmML 0.8.1 the mutual exclusive nature of ObservationTimes and NumberTimes is not in the schema
     * Continuous and Discrete are lists of variables to sample at the given time points
     */
    class Observation : public Object, public PharmMLSection
    {
        public:
            Observation(PharmMLReader &reader, xml::Node node);
            Observation() {};
            void gatherObjectRefs(std::unordered_map<std::string, Object *> &oidMap);
            xml::Node xml();

            std::shared_ptr<ObjectRef> getOidRef();
            std::shared_ptr<AstNode> getNumberTimes();
            void setNumberTimes(std::shared_ptr<AstNode> numberTimes);
            std::shared_ptr<AstNode> getObservationTimes();
            void setObservationTimes(std::shared_ptr<AstNode> observationTimes);
            std::vector<std::shared_ptr<SymbRef>>& getContinuousVariables();
            std::vector<std::shared_ptr<SymbRef>>& getDiscreteVariables();
 
            std::vector<AstNode *> getObservationTimesAsVector();
            void accept(PharmMLVisitor *visitor);
            void accept(ObjectVisitor *visitor);

        private:
            void parse(PharmMLReader &reader, xml::Node node);
            std::shared_ptr<ObjectRef> oidRef;
            std::shared_ptr<AstNode> numberTimes;
            std::shared_ptr<AstNode> observationTimes;
            std::vector<std::shared_ptr<SymbRef>> continuousVariables;
            std::vector<std::shared_ptr<SymbRef>> discreteVariables;
    };

    // IndividualObservations class
    class IndividualObservations : public Object, public PharmMLSection
    {
        public:
            IndividualObservations(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            void gatherObjectRefs(std::unordered_map<std::string, Object *> &oidMap) {};
            xml::Node xml();
            std::vector<ColumnMapping *> getColumnMappings();
            Dataset *getDataset();
            void accept(PharmMLVisitor *visitor);
            void accept(ObjectVisitor *visitor);

        private:
            std::vector<ColumnMapping *> columnMappings;
            Dataset *dataset;
    };

    // ObservationCombination class
    class ObservationCombination
    {
        public:
            ObservationCombination(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            std::string getOid();
            std::vector<std::string> getOidRefs();
            AstNode *getRelative();
            void accept(PharmMLVisitor *visitor);

        private:
            std::string oid;
            std::vector<std::string> oidRefs;
            AstNode *relative = nullptr;
    };

    // Observations class (contains objects of classes above)
    class Observations
    {
        public:
            Observations(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            xml::Node xml();
            std::vector<Variable *> getDesignParameters();
            std::vector<Observation *> getObservations(); // Or getSimulationObservations()?
            std::vector<IndividualObservations *> getIndividualObservations(); // Or getDatasetObservations()?
            std::vector<ObservationCombination *> getObservationCombinations();
            void accept(PharmMLVisitor *visitor);

        private:
            std::vector<Variable *> designParameters;
            std::vector<Observation *> simulationObservations;
            std::vector<IndividualObservations *> datasetObservations;
            // TODO: Implement support for LookupTable according to schema
            std::vector<ObservationCombination *> observationCombinations;
    };
}

#endif
