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

#ifndef CPHARMML_VARIABILITYMODELS_H_
#define CPHARMML_VARIABILITYMODELS_H_

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <AST/AstNode.h>
#include <symbols/Symbol.h>
#include <PharmML/VariabilityModel.h>
#include <symbols/VariabilityLevel.h>
#include <symbols/RandomVariable.h>
#include <PharmML/Correlation.h>

namespace CPharmML
{
    class VariabilityModels
    {
        public:
            // Add a VariabilityModel (only one of each type)
            void addVariabilityModel(pharmmlcpp::VariabilityModel *variabilityModel);

            // Add a RandomVariable or Correlation
            void addRandomVariable(pharmmlcpp::RandomVariable *randomVariable);
            void addCorrelation(pharmmlcpp::Correlation *correlation);

            std::vector<pharmmlcpp::VariabilityLevel *> getParameterLevelChain();
            std::vector<pharmmlcpp::VariabilityLevel *> getResidualErrorLevelChain();
            std::vector<pharmmlcpp::RandomVariable *> getRandomVariablesOnLevel(pharmmlcpp::Symbol *level);
            std::vector<pharmmlcpp::Correlation *> getCorrelationsOnLevel(pharmmlcpp::Symbol *level);

        private:
            // PharmML objects used to consolidate
            std::vector<pharmmlcpp::VariabilityModel *> variabilityModels;
            std::vector<pharmmlcpp::RandomVariable *> randomVariables;

            std::unordered_set<pharmmlcpp::VariabilityLevel *> parameterLevels;
            std::unordered_set<pharmmlcpp::VariabilityLevel *> residualErrorLevels;
            std::unordered_set<pharmmlcpp::VariabilityLevel *> referenceLevels;
            std::unordered_set<pharmmlcpp::VariabilityLevel *> orphans;
            std::unordered_map<pharmmlcpp::VariabilityLevel *, pharmmlcpp::Symbol *> parent;

            std::unordered_map<pharmmlcpp::Symbol *, std::vector<pharmmlcpp::RandomVariable *>> randomVariablesOnLevel;
            std::unordered_map<pharmmlcpp::Symbol *, std::vector<pharmmlcpp::Correlation *>> correlationsOnLevel;

            std::vector<pharmmlcpp::VariabilityLevel *> buildDependencyChain(std::unordered_set<pharmmlcpp::VariabilityLevel *> levelSet);
    };
}

#endif
