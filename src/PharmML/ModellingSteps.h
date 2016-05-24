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

#ifndef PHARMML_MODELLINGSTEPS_H_
#define PHARMML_MODELLINGSTEPS_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <PharmML/PharmMLContext.h>
#include <PharmML/ColumnMapping.h>
#include <AST/symbols.h>
#include <PharmML/Dataset.h>
#include <symbols/Symbol.h>

namespace PharmML
{
    class TargetTool
    {
        PharmML::PharmMLContext *context;
        std::string oid;
        std::string name;
        std::vector<ColumnMapping *> columnMappings;
        
        public:
        TargetTool(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
    };
    
    class VariableAssignment
    {
        PharmML::PharmMLContext *context;
        SymbRef *symbRef;
        AstNode *assignment;
        
        public:
        VariableAssignment(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        SymbRef *getSymbRef();
        AstNode *getAssignment();
    };
    
    class CommonStepType
    {
        PharmML::PharmMLContext *context;
        
        protected:
        std::string oid;
        std::vector<PharmML::ExternalFile *> softwareSettings;
        std::vector<PharmML::ExternalFile *> outputFiles;
        std::string targetToolRef;
        std::string extDatasetRef;
        std::vector<std::string> interventionsRefs;
        std::vector<std::string> observationsRefs;
        std::vector<PharmML::VariableAssignment *> varAssignments;
        
        public:
        CommonStepType(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getOid();
        std::vector<PharmML::ExternalFile *> getSoftwareSettingsFiles();
        std::vector<PharmML::ExternalFile *> getOutputFiles();
        std::string getTargetToolRef();
        std::string getExternalDatasetRef();
        std::vector<std::string> getInterventionsRefs();
        std::vector<std::string> getObservationsRefs();
        std::vector<PharmML::VariableAssignment *> getVariableAssignments();
    };
    
    class ParameterEstimation : public Referer
    {
        PharmML::PharmMLContext *context;
        SymbRef *symbRef;
        bool fixed = false;
        AstNode *init = nullptr;
        AstNode *loBound = nullptr;
        AstNode *hiBound = nullptr;
        
        public:
        ParameterEstimation(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        SymbRef *getSymbRef();
        bool isFixed();
        bool hasInitValue();
        bool hasLoBound();
        bool hasHiBound();
        AstNode *getInitValue();
        AstNode *getLoBound();
        AstNode *getHiBound();
        void accept(PharmMLVisitor *visitor);
    };
    
    class EstimationStep : CommonStepType
    {
        PharmML::PharmMLContext *context;
        std::vector<ParameterEstimation *> parameterEstimations;
        // TODO: Add Operation support! SAEM etc. Forgot that one.
        
        public:
        EstimationStep(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::vector<ParameterEstimation *> getParameters();
    };
    
    class SimulationStep : CommonStepType
    {
        PharmML::PharmMLContext *context;
        
        public:
        SimulationStep(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
    };
    
    class OptimiseOn
    {
        PharmML::PharmMLContext *context;
        bool armSize = false;
        bool doseAmount = false;
        bool dosingTimes = false;
        bool duration = false;
        bool numberArms = false;
        bool numberSamples = false;
        bool numberTimes = false;
        bool observationTimes = false;
        std::vector<SymbRef *> symbols;
        
        public:
        OptimiseOn(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
    };
    
    class OptimalDesignStep
    {
        public:
            OptimalDesignStep(PharmML::PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            std::vector<ParameterEstimation *> getParameters();

        private:
            PharmML::PharmMLContext *context;
            std::string oid;
            OptimiseOn *optOn = nullptr;
            std::vector<ParameterEstimation *> parameterEstimations;
    };
    
    class ModellingSteps
    {
        PharmML::PharmMLContext *context;
        xml::Node xml_node;
        std::vector<TargetTool *> tools;
        std::vector<EstimationStep *> estSteps;
        std::vector<SimulationStep *> simSteps;
        std::vector<OptimalDesignStep *> optSteps;
        
        public:
        ModellingSteps(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        void gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap);
        std::vector<EstimationStep *> getEstimationSteps();
        std::vector<SimulationStep *> getSimulationSteps();
        std::vector<OptimalDesignStep *> getOptimalDesignSteps();
        void update();
    };
}

#endif
