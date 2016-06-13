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

#ifndef PHARMMLCPP_MODEL_H_
#define PHARMMLCPP_MODEL_H_

#ifndef PHARMMLCPP_PHARMMLCONTEXT_H_
#include <PharmML/PharmMLContext.h>
#endif

#include <symbols/IndependentVariable.h>
#include <symbols/FunctionDefinition.h>
#include <PharmML/ModelDefinition.h>
#include <PharmML/TrialDesign.h>
#include <xml/xml.h>
#include <consolidators/Consolidator.h>
#include <symbols/SymbolGathering.h>
#include <symbols/MacroGathering.h>
#include <helpers/SymbolNamer.h>

namespace pharmmlcpp
{
    class PharmML
    {
        public:
            PharmML(const char *filename);
            ~PharmML();
            void write(std::string filename);
            IndependentVariable *getIndependentVariable();
            std::vector<pharmmlcpp::FunctionDefinition *> getFunctionDefinitions();
            FunctionDefinition *resolveFunctionCall(FunctionCall *functionCall);
            ModelDefinition *getModelDefinition();
            TrialDesign *getTrialDesign();
            ModellingSteps *getModellingSteps();
            CPharmML::Consolidator *getConsolidator();
            void setSymbolNamer(SymbolNamer *namer);
            SymbolNamer *getSymbolNamer();

            PharmMLContext *getContext(); // FIXME: Only here to be able to create classes (that demands a context) in unit tests!

        private:
            PharmMLContext *context;
            pharmmlcpp::IndependentVariable *independentVariable = nullptr;
            std::vector<FunctionDefinition *> functionDefinitions;
            ModelDefinition *modelDefinition;
            TrialDesign *trialDesign = nullptr;
            ModellingSteps *modellingSteps = nullptr;
            CPharmML::Consolidator *consolidator;
            SymbolSet allSymbols;
            std::unordered_set<pharmmlcpp::Object *> allObjects;
            SymbolNamer *symbolNamer;
            void parse(xml::Node node);
            void postParse();
            void setupSymbols();
            void setupObjects();
            void setupPKMacros();
            void checkAndAddOid(std::unordered_set<std::string> &allOids, Object *object, PharmMLSection *section);
            void setupRefererSymbRefs(SymbolGathering &gathering);
            void setupTargetMappings(SymbolGathering &gathering);
            void setupTargetMappings(MacroGathering &gathering);
    };
}

#endif
