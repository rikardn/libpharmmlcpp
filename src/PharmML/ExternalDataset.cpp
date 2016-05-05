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

namespace PharmML
{
    ExternalDataset::ExternalDataset(PharmML::PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void ExternalDataset::parse(xml::Node node) {
        this->oid = node.getAttribute("oid").getValue();
        std::vector<xml::Node> array = this->context->getElements(node, "./design:ColumnMapping");
        for (xml::Node n : array) {
            PharmML::ColumnMapping *col = new PharmML::ColumnMapping(this->context, n);
            this->ColumnMappings.push_back(col);
        }
        // TODO: Support ColumnTransformation
        // TODO: Support MultipleDVMapping
        xml::Node ds_node = this->context->getSingleElement(node, "./ds:DataSet");
        if (ds_node.exists()) {
            this->dataset = new PharmML::Dataset(this->context, ds_node);
        } else {
            // TODO: Support CodeInjection
        }
        this->toolName = node.getAttribute("toolName").getValue();
    }
    
    std::string ExternalDataset::getOid() {
        return this->oid;
    }

    std::vector<PharmML::ColumnMapping *> ExternalDataset::getColumnMappings() {
        return this->ColumnMappings;
    }
    
    Dataset *ExternalDataset::getDataset() {
        return this->dataset;
    }
    
    std::string ExternalDataset::getToolName() {
        return this->toolName;
    }
    
    void ExternalDataset::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
