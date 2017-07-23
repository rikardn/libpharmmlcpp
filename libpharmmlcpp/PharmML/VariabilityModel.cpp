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

#include <vector>
#include <iostream>
#include <cstdlib> // FIXME: Remove when logger implemented (see comment in getVariabilityLevelHieararchy)

#include "VariabilityModel.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <symbols/SymbolGathering.h>

namespace pharmmlcpp
{
    VariabilityModel::VariabilityModel(PharmMLReader &reader, xml::Node node) {
        this->Block::parse(node);
        this->parse(reader, node);
    }

    void VariabilityModel::parse(PharmMLReader &reader, xml::Node node) {
        // Get type and name
        this->type = node.getAttribute("type").getValue(); // TODO: Validate to either "residualError" or "parameterVariability"
        xml::Node name_node = reader.getSingleElement(node, "./ct:Name");
        if (name_node.exists()) {
            this->name = reader.getSingleElement(node, "./ct:Name").getText();
        }

        // Get variability levels
        std::vector<xml::Node> var_level_nodes = reader.getElements(node, "./mdef:Level");
        for (xml::Node var_level_node : var_level_nodes) {
            VariabilityLevel *var_level = new VariabilityLevel(reader, var_level_node);
            this->variabilityLevels.push_back(var_level);
        }
    }

    xml::Node VariabilityModel::xml(PharmMLWriter &writer) {
        xml::Node vmod("VariabilityModel");
        this->Block::xml(writer, vmod);
        vmod.setAttribute("type", this->type);

        for (const auto &level : this->variabilityLevels) {
            vmod.addChild(level->xml(writer));
        }

        return vmod;
    }

    std::string VariabilityModel::getName() {
        return this->name;
    }

    bool VariabilityModel::onResidualError() {
        return (this->type == "residualError");
    }

    bool VariabilityModel::onParameter() {
        return (this->type == "parameterVariability");
    }

    std::vector<pharmmlcpp::VariabilityLevel *> VariabilityModel::getVariabilityLevels() {
        return this->variabilityLevels;
    }

    // Get VariabilityLevel's in dependency order (parents first, children last)
    // TODO: Consider if cross-reference between VariabilityModel's could be a problem?
    std::vector<VariabilityLevel *> VariabilityModel::getVariabilityLevelHierarchy() {
        // Build associative set of level->parent_level_symbol
        std::unordered_map<pharmmlcpp::VariabilityLevel *, pharmmlcpp::Symbol *> parent;
        for (pharmmlcpp::VariabilityLevel *level : this->variabilityLevels) {
            pharmmlcpp::SymbRef *parent_ref = level->getParentReference();
            if (parent_ref) {
                parent[level] = parent_ref->getSymbol();
            }
        }

        // Build dependency chain via adding and depleting list of levels
        std::vector<VariabilityLevel *> chain;
        std::unordered_map<pharmmlcpp::Symbol *, bool> added;
        size_t last_num_added = added.size();
        do {
            for (VariabilityLevel *level : this->variabilityLevels) {
                // Don't add duplicates
                if (!added[level]) {
                    // If level has no parent, it is top-level and should be added first
                    auto got = parent.find(level);
                    if (got == parent.end()) {
                        chain.push_back(level);
                        added[level] = true;
                    // If level has parent but it has already been added, add this child level
                    } else if (added[parent[level]]) {
                        chain.push_back(level);
                        added[level] = true;
                    }
                }
            }
            // Check if depletion has stagnated -> non-resolvable dependencies
            if (added.size() == last_num_added) {
                for (VariabilityLevel *level : this->variabilityLevels) {
                    if (!added[level]) {
                        // FIXME: How access to a logger?
                        // this->logger("VariabilityLevel could not be included in hiearchy due to parent not existing", level);
                        abort(); // FIXME: Remove when logger is implemented
                    }
                }
                break;
            } else {
                last_num_added = added.size();
            }
        } while (chain.size() < this->variabilityLevels.size());
        return chain;
    }

    VariabilityLevel *VariabilityModel::getReferenceLevel() {
        for (VariabilityLevel *level : this->variabilityLevels) {
             if (level->isReferenceLevel()) {
                 return level;
             }
        }
        return nullptr;
    }

    void VariabilityModel::gatherSymbols(SymbolGathering &gathering) {
        gathering.newBlock(this);
        for (VariabilityLevel *varlev : this->variabilityLevels) {
            gathering.addSymbol(varlev);
        }
    }
}
