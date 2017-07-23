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

namespace pharmmlcpp
{
    CovariateModel::CovariateModel(PharmMLReader &reader, xml::Node node) {
        this->Block::parse(node);
        this->parse(reader, node);
    }

    void CovariateModel::parse(PharmMLReader &reader, xml::Node node) {
        std::vector<xml::Node> cov_nodes = reader.getElements(node, "./mdef:Covariate");
        for (xml::Node cov_node : cov_nodes) {
            Covariate *cov = new Covariate(reader, cov_node);
            this->covariates.push_back(cov);
        }
    }

    xml::Node CovariateModel::xml(PharmMLWriter &writer) {
        xml::Node covmod("CovariateModel");
        this->Block::xml(writer, covmod);

        return covmod;
    }

    std::vector<pharmmlcpp::Covariate *> CovariateModel::getCovariates() {
        return this->covariates;
    }

    void CovariateModel::gatherSymbols(SymbolGathering &gathering) {
        gathering.newBlock(this);
        for (Covariate *cov : this->covariates) {
            gathering.addSymbol(cov);
            for (Covariate *transformation : cov->getTransformations()) {
                gathering.addSymbol(transformation);
            }
        }
    }
}
