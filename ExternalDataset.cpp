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
        std::vector<xml::Node> array = this->context->getElements(node, ".//design:ColumnMapping");
        for (xml::Node n : array) {
            PharmML::ColumnMapping *col = new PharmML::ColumnMapping(this->context, n);
            this->ColumnMappings.push_back(col);
        }
    }

    std::vector<PharmML::ColumnMapping *> ExternalDataset::getColumnMappings() {
        return this->ColumnMappings;
    }
}
