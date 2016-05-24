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

#include <consolidators/VariabilityModels.h>

namespace CPharmML
{
    // Add a VariabilityModel (only one of each type)
    void VariabilityModels::addVariabilityModel(PharmML::VariabilityModel *variabilityModel) {
        this->variabilityModels.push_back(variabilityModel);
        // Sort the levels into bins for dependency sorting
        std::vector<PharmML::VariabilityLevel *> levels = variabilityModel->getVariabilityLevels();
        for (PharmML::VariabilityLevel *level : levels) {
            // Split parameter/residual error levels
            if (variabilityModel->onParameter()) {
                this->parameterLevels.insert(level);
            }
            else if (variabilityModel->onResidualError()) {
                this->residualErrorLevels.insert(level);
            }

            // Reference levels
            if (level->isReferenceLevel()) {
                this->referenceLevels.insert(level);
            }

            // Split children (associative array children->parent) and orphans (top-level parents)
            if (level->getParentReference()) {
                PharmML::Symbol *parent = level->getParentReference()->getSymbol();
                this->parent[level] = parent;
            } else {
                this->orphans.insert(level);
            }
        }
    }

    // Add a RandomVariable
    void VariabilityModels::addRandomVariable(PharmML::RandomVariable *randomVariable) {
        std::vector<PharmML::VariabilityReference *> var_refs = randomVariable->getVariabilityReferences();
        for (PharmML::VariabilityReference *var_ref : var_refs) {
            PharmML::Symbol *level = var_ref->getLevelReference()->getSymbol();
            if (this->randomVariablesOnLevel.count(level) == 0) {
                // Need to initialize vector first (insert and std::make_pair if not C++11)
                this->randomVariablesOnLevel.emplace(level, std::vector<PharmML::RandomVariable *>());
            }
            this->randomVariablesOnLevel[level].push_back(randomVariable);
        }
    }

    // Add a Correlation
    void VariabilityModels::addCorrelation(PharmML::Correlation *correlation) {
        PharmML::VariabilityReference *var_ref = correlation->getVariabilityReference();
        PharmML::Symbol *level = var_ref->getLevelReference()->getSymbol();
        if (this->correlationsOnLevel.count(level) == 0) {
            // Need to initialize vector first (insert and std::make_pair if not C++11)
            this->correlationsOnLevel.emplace(level, std::vector<PharmML::Correlation *>());
        }
        this->correlationsOnLevel[level].push_back(correlation);
}

    std::vector<PharmML::VariabilityLevel *> VariabilityModels::getParameterLevelChain() {
        return this->buildDependencyChain(this->parameterLevels);
    }

    std::vector<PharmML::VariabilityLevel *> VariabilityModels::getResidualErrorLevelChain() {
        return this->buildDependencyChain(this->residualErrorLevels);
    }

    std::vector<PharmML::RandomVariable *> VariabilityModels::getRandomVariablesOnLevel(PharmML::Symbol *level) {
        return this->randomVariablesOnLevel[level];
    }

    std::vector<PharmML::Correlation *> VariabilityModels::getCorrelationsOnLevel(PharmML::Symbol *level) {
        return this->correlationsOnLevel[level];
    }

    std::vector<PharmML::VariabilityLevel *> VariabilityModels::buildDependencyChain(std::unordered_set<PharmML::VariabilityLevel *> levelSet) {
        std::vector<PharmML::VariabilityLevel *> chain;
        std::unordered_map<PharmML::Symbol *, bool> added;
        do {
            for (PharmML::VariabilityLevel *level : levelSet) {
                auto got = this->orphans.find(level);
                if (got != this->orphans.end() && !added[level]) {
                    chain.push_back(level);
                    added[level] = true;
                }
                if (added[this->parent[level]] && !added[level]) {
                    chain.push_back(level);
                    added[level] = true;
                }
            }
        } while (chain.size() < levelSet.size());
        return chain;
    }
}
