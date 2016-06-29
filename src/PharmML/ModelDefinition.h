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

#ifndef PHARMMLCPP_MODELDEFINITION_H_
#define PHARMMLCPP_MODELDEFINITION_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <PharmML/PharmMLContext.h>
#include <PharmML/VariabilityModel.h>
#include <PharmML/CovariateModel.h>
#include <PharmML/ParameterModel.h>
#include <PharmML/StructuralModel.h>
#include <symbols/ObservationModel.h>
#include <PharmML/ModellingSteps.h>
#include <symbols/Symbol.h>

namespace pharmmlcpp
{
    class ModelDefinition
    {
        public:
            ModelDefinition(PharmMLReader &reader, xml::Node node);
            std::vector<pharmmlcpp::VariabilityModel *> getVariabilityModels();
            pharmmlcpp::CovariateModel *getCovariateModel();
            pharmmlcpp::ParameterModel *getParameterModel();
            pharmmlcpp::StructuralModel *getStructuralModel();
            pharmmlcpp::ObservationModel *getObservationModel();
            void parse(PharmMLReader &reader, xml::Node node);
            void setupRefererSymbRefs(SymbolGathering &gathering);

        private:
            pharmmlcpp::PharmMLContext *context;
            std::vector<pharmmlcpp::VariabilityModel *> VariabilityModels;
            // TODO: Why are these all singular? It might be wise to support multiple models while it's still straightforward.
            pharmmlcpp::CovariateModel *covariateModel = nullptr;
            pharmmlcpp::ParameterModel *parameterModel = nullptr;
            pharmmlcpp::StructuralModel *structuralModel = nullptr;
            pharmmlcpp::ObservationModel *observationModel = nullptr;

    };
}

#endif
