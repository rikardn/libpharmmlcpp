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
     * FIXME ???? NumberTimes is a vector of the same size as ObservationTimes with the number of replicates for each observation time point
     * Continuous and Discrete are lists of variables to sample at the given time points
     */
    class Observation : public Object, public PharmMLSection
    {
        public:
            Observation(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            void gatherObjectRefs(std::unordered_map<std::string, Object *> &oidMap);
            ObjectRef* getOidRef();
            AstNode *getNumber();
            AstNode *getTimes();
            std::vector<AstNode *> getTimesAsVector();
            std::vector<SymbRef *> getContinuousVariables();
            std::vector<SymbRef *> getDiscreteVariables();
            void accept(PharmMLVisitor *visitor);
            void accept(ObjectVisitor *visitor);

        private:
            ObjectRef *oidRef = nullptr;
            AstNode *number = nullptr;
            AstNode *times = nullptr;
            std::vector<SymbRef *> continuousVariables;
            std::vector<SymbRef *> discreteVariables;
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
