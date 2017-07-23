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

#include "Covariates.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace pharmmlcpp
{
    Covariates::Covariates(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
    }

    void Covariates::parse(PharmMLReader &reader, xml::Node node) {
        /*std::vector<xml::Node> ind_covs = reader.getElements(node, "./design:IndividualCovariates");
        for (xml::Node ind_cov : ind_covs) {
            IndividualCovariate *cov = new IndividualCovariate(reader, ind_cov);
            this->individualCovariates.push_back(cov);
        }*/
        std::vector<xml::Node> cov_models = reader.getElements(node, "./design:CovariateModel");
        for (xml::Node cov_model : cov_models) {
            TDCovariateModel *cov = new TDCovariateModel(reader, cov_model);
            this->covariateModels.push_back(cov);
        }
    }

    std::vector<TDCovariateModel *> Covariates::getCovariateModels() {
        return this->covariateModels;
    }

/*    void Covariates::gatherSymbols(SymbolGathering &gathering) {
        gathering.newBlock(this);
        for (Covariate *cov : this->covariates) {
            gathering.addSymbol(cov);
            for (Covariate *transformation : cov->getTransformations()) {
                gathering.addSymbol(transformation);
            }
        }
    }*/
}
