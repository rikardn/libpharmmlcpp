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

#ifndef PHARMML_MODEL_H_
#define PHARMML_MODEL_H_

#ifndef PHARMML_PHARMMLCONTEXT_H_
#include <PharmML/PharmMLContext.h>
#endif

#include <symbols/IndependentVariable.h>
#include <symbols/FunctionDefinition.h>
#include <PharmML/ModelDefinition.h>
#include <PharmML/TrialDesign.h>
#include <xml/xml.h>
#include <consolidators/Consolidator.h>
#include <symbols/SymbolGathering.h>

namespace PharmML
{
    class Model
    {
        public:
            Model(const char *filename);
            ~Model();
            void write(const char *filename);
            PharmML::IndependentVariable *getIndependentVariable();
            std::vector<PharmML::FunctionDefinition *> getFunctionDefinitions();
            PharmML::ModelDefinition *getModelDefinition();
            PharmML::TrialDesign *getTrialDesign();
            PharmML::ModellingSteps *getModellingSteps();
            CPharmML::Consolidator *getConsolidator();

        private:
            PharmMLContext *context;
            PharmML::IndependentVariable *IndependentVariable;
            std::vector<PharmML::FunctionDefinition *> FunctionDefinitions;
            PharmML::ModelDefinition *ModelDefinition;
            PharmML::TrialDesign *TrialDesign;
            PharmML::ModellingSteps *ModellingSteps = nullptr;
            CPharmML::Consolidator *consolidator;
            PharmML::SymbolSet allSymbols;
            std::unordered_set<PharmML::Object *> allObjects;
            void parse(xml::Node node);
            void postParse();
            void setupSymbols();
            void setupObjects();
            void checkAndAddOid(std::unordered_set<std::string> &allOids, Object *object, PharmMLSection *section);
            void setupRefererSymbRefs(SymbolGathering &gathering);
    };
}

#endif
