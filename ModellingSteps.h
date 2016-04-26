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
#include "PharmMLContext.h"
#include "ColumnMapping.h"
#include "symbols.h"

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
        PharmML::PharmMLContext *context;
        std::string oid;
        OptimiseOn *optOn = nullptr;
        
        public:
        OptimalDesignStep(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
    };
    
    class ModellingSteps
    {
        PharmML::PharmMLContext *context;
        xml::Node xml_node;
        std::vector<TargetTool *> tools;
        
        public:
        ModellingSteps(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        void update();
    };
}

#endif
