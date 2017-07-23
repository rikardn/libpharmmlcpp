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

#ifndef CPHARMMLCPP_CONSOLIDATOR_H_
#define CPHARMMLCPP_CONSOLIDATOR_H_

#include <memory>
#include <vector>
#include <string>
#include <unordered_set>
#include <AST/AstNode.h>
#include <AST/symbols.h>
#include <consolidators/PopulationParameters.h>
#include <consolidators/Covariates.h>
#include <consolidators/VariabilityModels.h>
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
            Consolidator(pharmmlcpp::PharmML *model);
            CPharmML::PopulationParameters *getPopulationParameters();
            std::vector<CPharmML::Covariate *> getCovariates();
            CPharmML::VariabilityModels *getVariabilityModels();

        private:
            std::shared_ptr<pharmmlcpp::Logger> logger;

            std::vector<CPharmML::PopulationParameters *> populationParameters; // Vector with one wrapper object per parameter model
            std::vector<CPharmML::Covariate *> covariates;
            CPharmML::VariabilityModels *variabilityModels;

            void consolidatePopulationParameters(pharmmlcpp::PharmML *model);
            void consolidateCovariates(pharmmlcpp::PharmML *model);
            void consolidateVariabilityModels(pharmmlcpp::PharmML *model);
    };
}

#endif
