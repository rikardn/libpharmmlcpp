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

#include "ExternalDataset.h"

namespace pharmmlcpp
{
    ExternalDataset::ExternalDataset(pharmmlcpp::PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
    }

    void ExternalDataset::parse(xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        std::vector<xml::Node> array = this->context->getElements(node, "./design:ColumnMapping");
        for (xml::Node n : array) {
            pharmmlcpp::ColumnMapping *col = new pharmmlcpp::ColumnMapping(this->context, n);
            this->col_maps.push_back(col);
        }
        // TODO: Support ColumnTransformation
        // TODO: Support MultipleDVMapping
        xml::Node ds_node = this->context->getSingleElement(node, "./ds:DataSet");
        if (ds_node.exists()) {
            this->dataset = new pharmmlcpp::Dataset(this->context, ds_node);
        } else {
            // TODO: Support CodeInjection
        }
        this->toolName = node.getAttribute("toolName").getValue();
    }

    std::string ExternalDataset::getOid() {
        return this->oid;
    }

    std::vector<pharmmlcpp::ColumnMapping *> ExternalDataset::getColumnMappings() {
        return this->col_maps;
    }

    Dataset *ExternalDataset::getDataset() {
        return this->dataset;
    }

    std::string ExternalDataset::getToolName() {
        return this->toolName;
    }

    void ExternalDataset::setupRefererSymbRefs(SymbolGathering &gathering) {
        for (pharmmlcpp::ColumnMapping *col_map : this->col_maps) {
            col_map->setupSymbRefs(gathering, "");
        }
    }

    void ExternalDataset::setupTargetMappings(SymbolGathering &gathering) {
        for (pharmmlcpp::ColumnMapping *col_map : this->col_maps) {
            pharmmlcpp::TargetMapping *target_map = col_map->getTargetMapping();
            if (target_map) {
                target_map->setupSymbolRefs(gathering);
            }
        }
    }

    void ExternalDataset::setupTargetMappings(MacroGathering &gathering) {
        for (pharmmlcpp::ColumnMapping *col_map : this->col_maps) {
            pharmmlcpp::TargetMapping *target_map = col_map->getTargetMapping();
            if (target_map) {
                target_map->setupMacroRefs(gathering);
            }
        }
    }

    void ExternalDataset::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    // POST PARSE/CONSOLIDATION
    
}
