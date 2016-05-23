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

#ifndef CPHARMML_CONSOLIDATOR_H_
#define CPHARMML_CONSOLIDATOR_H_

#include <memory>
#include <vector>
#include <string>
#include <unordered_set>
#include <PharmML/PharmMLContext.h>
#include <AST/AstNode.h>
#include <AST/symbols.h>
#include <consolidators/PopulationParameters.h>
#include <consolidators/Covariates.h>
#include <consolidators/VariabilityModels.h>
#include <consolidators/Functions.h>
#include <consolidators/PKMacros.h>
#include <symbols/SymbolSet.h>
#include <symbols/Symbol.h>
#include <objects/Object.h>
#include <helpers/Logger.h>
#include <PharmML/PharmMLSection.h>

namespace CPharmML
{
    class Consolidator
    {
        public:
            Consolidator(PharmML::PharmMLContext *context, PharmML::Model *model);
            CPharmML::PopulationParameters *getPopulationParameters();
            std::vector<CPharmML::Covariate *> getCovariates();
            CPharmML::VariabilityModels *getVariabilityModels();
            CPharmML::Functions *getFunctions();
            CPharmML::PKMacros *getPKMacros();

        private:
            std::shared_ptr<PharmML::Logger> logger;
            PharmML::PharmMLContext *context;
            
            PharmML::SymbolSet allSymbols;
            std::unordered_set<PharmML::Object *> allObjects;

            std::vector<CPharmML::PopulationParameters *> populationParameters; // Vector with one wrapper object per parameter model
            std::vector<CPharmML::Covariate *> covariates;
            CPharmML::VariabilityModels *variabilityModels;
            CPharmML::Functions *functions;
            CPharmML::PKMacros *pk_macros;
            
            void consolidateSymbols(PharmML::Model *model);
            void consolidateObjects(PharmML::Model *model);
            
            void consolidatePopulationParameters(PharmML::Model *model);
            void consolidateCovariates(PharmML::Model *model);
            void consolidateVariabilityModels(PharmML::Model *model);
            void consolidateFunctions(PharmML::Model *model);
            void consolidatePKMacros(PharmML::Model *model);
            void consolidateTrialDesign(PharmML::Model *model);
            
            void duplicateOidError(const std::string&, PharmML::PharmMLSection *arm);
    };
}

#endif
