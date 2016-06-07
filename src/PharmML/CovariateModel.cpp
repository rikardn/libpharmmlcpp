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

#include "CovariateModel.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace PharmML
{
    CovariateModel::CovariateModel(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->Block::parse(node);
        this->parse(node);
    }

    void CovariateModel::parse(xml::Node node) {
        std::vector<xml::Node> cov_nodes = this->context->getElements(node, "./mdef:Covariate");
        for (xml::Node cov_node : cov_nodes) {
            PharmML::Covariate *cov = new PharmML::Covariate(this->context, cov_node);
            this->covariates.push_back(cov);
        }
    }

    std::vector<PharmML::Covariate *> CovariateModel::getCovariates() {
        return this->covariates;
    }

}
