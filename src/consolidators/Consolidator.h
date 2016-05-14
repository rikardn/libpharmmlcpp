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

#include <vector>
#include <string>
#include <unordered_set>
#include <PharmML/PharmMLContext.h>
#include <AST/AstNode.h>
#include <AST/symbols.h>
#include <consolidators/PopulationParameter.h>
#include <consolidators/Covariates.h>
#include <consolidators/VariabilityModels.h>
#include <symbols/SymbolSet.h>
#include <symbols/Symbol.h>

namespace CPharmML
{
    class Consolidator
    {
        public:
            Consolidator(PharmML::PharmMLContext *context, PharmML::Model *model);
            std::vector<CPharmML::PopulationParameter *> getPopulationParameters();
            std::vector<CPharmML::Covariate *> getCovariates();
            CPharmML::VariabilityModels *getVariabilityModels();

        private:
            PharmML::PharmMLContext *context;
            
            PharmML::SymbolSet allSymbols;
            
            std::vector<CPharmML::PopulationParameter *> populationParameters;
            std::vector<CPharmML::Covariate *> covariates;
            CPharmML::VariabilityModels *variabilityModels;
            
            void consolidateSymbols(PharmML::Model *model);
            
            void consolidatePopulationParameters(PharmML::Model *model);
            void consolidateCovariates(PharmML::Model *model);
            void consolidateVariabilityModels(PharmML::Model *model);
    };
}

#endif
