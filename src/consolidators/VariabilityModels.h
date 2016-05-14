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

namespace CPharmML
{
    class VariabilityModels
    {
        public:
            // Add a VariabilityModel (only one of each type)
            void addVariabilityModel(PharmML::VariabilityModel *variabilityModel);
            
            std::vector<PharmML::VariabilityLevel *> getParameterLevelChain();
            std::vector<PharmML::VariabilityLevel *> getResidualErrorLevelChain();
        
        private:
            // PharmML objects used to consolidate
            std::vector<PharmML::VariabilityModel *> variabilityModels;
            
            std::unordered_set<PharmML::VariabilityLevel *> parameterLevels;
            std::unordered_set<PharmML::VariabilityLevel *> residualErrorLevels;
            std::unordered_set<PharmML::VariabilityLevel *> referenceLevels;
            std::unordered_set<PharmML::VariabilityLevel *> orphans;
            std::unordered_map<PharmML::VariabilityLevel *, PharmML::Symbol *> parent;
            
            std::vector<PharmML::VariabilityLevel *> buildDependencyChain(std::unordered_set<PharmML::VariabilityLevel *> levelSet);
    };
}

#endif
