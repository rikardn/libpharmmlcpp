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

#ifndef PHARMMLCPP_MODELLINGSTEPS_H_
#define PHARMMLCPP_MODELLINGSTEPS_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <PharmML/PharmMLContext.h>
#include <PharmML/ColumnMapping.h>
#include <AST/symbols.h>
#include <PharmML/Dataset.h>
#include <symbols/Symbol.h>
#include <symbols/SymbolGathering.h>

#include <helpers/StringTools.h>
#include <visitors/AstAnalyzer.h>

namespace pharmmlcpp
{
    class TargetTool
    {
        public:
            TargetTool(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);

        private:
            std::string oid;
            std::string name;
            std::vector<ColumnMapping *> columnMappings;
    };

    class VariableAssignment
    {
        public:
            VariableAssignment(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            SymbRef *getSymbRef();
            std::shared_ptr<AstNode> getAssignment();

        private:
            SymbRef *symbRef;
            std::shared_ptr<AstNode> assignment;
    };

    class CommonStepType
    {
        public:
            CommonStepType(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            std::string getOid();
            std::vector<ExternalFile *> getSoftwareSettingsFiles();
            std::vector<ExternalFile *> getOutputFiles();
            std::string getTargetToolRef();
            std::string getExternalDatasetRef();
            std::vector<std::string> getInterventionsRefs();
            std::vector<std::string> getObservationsRefs();
            std::vector<VariableAssignment *> getVariableAssignments();

        protected:
            std::string oid;
            std::vector<ExternalFile *> softwareSettings;
            std::vector<ExternalFile *> outputFiles;
            std::string targetToolRef;
            std::string extDatasetRef;
            std::vector<std::string> interventionsRefs;
            std::vector<std::string> observationsRefs;
            std::vector<VariableAssignment *> varAssignments;
    };

    class ParameterEstimation : public Referer
    {
        public:
            ParameterEstimation(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            SymbRef *getSymbRef();
            bool isFixed();
            bool hasInitValue();
            bool hasLoBound();
            bool hasHiBound();
            std::shared_ptr<AstNode> getInitValue();
            std::shared_ptr<AstNode> getLoBound();
            std::shared_ptr<AstNode> getHiBound();
            void accept(PharmMLVisitor *visitor);
            void setupSymbRefs(SymbolGathering &gathering, std::string blkId) override {};

        private:
            SymbRef *symbRef;
            bool fixed = false;
            std::shared_ptr<AstNode> init;
            std::shared_ptr<AstNode> loBound;
            std::shared_ptr<AstNode> hiBound;
    };

    class OperationProperty : public PharmMLSection
    {
        public:
            OperationProperty(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            std::string getName();
            std::shared_ptr<AstNode> getAssignment();

            bool isNamed(std::string case_insensitive_name);
            // Convenience functions for simply accessing simple property values
            bool isInt();
            bool isReal();
            bool isBool();
            bool isString();

            int getInt();
            double getReal();
            bool getBool();
            std::string getString();
            bool isFoldedCaseString(std::string case_insensitive);

        private:
            std::string name;
            std::shared_ptr<AstNode> assignment;

            // Convenience storage of simple property values
            int *int_val = nullptr;
            double *real_val = nullptr;
            bool *bool_val = nullptr;
            std::string *string_val = nullptr;
            // FIXME: Above is properly setup by a postParse method!
    };

    class Algorithm : public PharmMLSection
    {
        public:
            Algorithm(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            std::string getName();
            std::string getDefinition();
            std::vector<OperationProperty *> getProperties();

            bool isNamed(std::string case_insensitive_name);
            bool isDefinedAs(std::string case_insensitive_def);

        private:
            std::string name;
            std::string definition;
            std::vector<OperationProperty *> properties;
    };

    class Operation : public PharmMLSection
    {
        public:
            Operation(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            int getOrder();
            std::string getType();
            std::string getName();
            std::vector<OperationProperty *> getProperties();
            Algorithm *getAlgorithm();

        private:
            std::string name;
            int order;
            std::string type;
            std::vector<OperationProperty *> properties;
            Algorithm *algorithm = nullptr;
    };

    class EstimationStep : public CommonStepType, public PharmMLSection
    {
        public:
            EstimationStep(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            std::vector<ParameterEstimation *> getParameters();
            std::vector<Operation *> getOperations();

            ParameterEstimation *getParameterEstimation(Symbol *symbol);

        private:
            std::vector<ParameterEstimation *> parameterEstimations;
            std::vector<Operation *> operations;

    };

    class SimulationStep : public CommonStepType
    {
        public:
            SimulationStep(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            std::vector<Operation *> getOperations();

        private:
            std::vector<Operation *> operations;
    };

    class OptimiseOn
    {
        public:
            OptimiseOn(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);

        private:
            bool armSize = false;
            bool doseAmount = false;
            bool dosingTimes = false;
            bool duration = false;
            bool numberArms = false;
            bool numberSamples = false;
            bool numberTimes = false;
            bool observationTimes = false;
            std::vector<SymbRef *> symbols;
    };

    class OptimalDesignStep : public PharmMLSection
    {
        public:
            OptimalDesignStep(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            std::vector<ParameterEstimation *> getParameters();
            std::vector<Operation *> getOperations();

        private:
            std::string oid;
            OptimiseOn *optOn = nullptr;
            std::vector<ParameterEstimation *> parameterEstimations;
            std::vector<Operation *> operations;
    };

    class ModellingSteps : public PharmMLSection
    {
        public:
            ModellingSteps(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            void setupRefererSymbRefs(SymbolGathering &gathering);
            std::vector<EstimationStep *> getEstimationSteps();
            std::vector<SimulationStep *> getSimulationSteps();
            std::vector<OptimalDesignStep *> getOptimalDesignSteps();
            void update();

        private:
            xml::Node xml_node;
            std::vector<TargetTool *> tools;
            std::vector<EstimationStep *> estSteps;
            std::vector<SimulationStep *> simSteps;
            std::vector<OptimalDesignStep *> optSteps;
    };
}

#endif
