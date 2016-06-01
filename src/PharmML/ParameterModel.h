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

#ifndef PHARMML_PARAMETERMODEL_H_
#define PHARMML_PARAMETERMODEL_H_

#include <symbols/PopulationParameter.h>
#include <symbols/IndividualParameter.h>
#include <symbols/RandomVariable.h>
#include <PharmML/Correlation.h>
#include <xml/xml.h>
#include <symbols/Symbol.h>
#include <PharmML/ModellingSteps.h>

namespace PharmML
{
    class ParameterModel
    {
        public:
            std::vector<Parameter *> getParameters();
            std::vector<PopulationParameter *> getPopulationParameters();
            std::vector<IndividualParameter *> getIndividualParameters();
            std::vector<RandomVariable *> getRandomVariables();
            std::vector<Correlation *> getCorrelations();
            ParameterModel(PharmML::PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            void gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap);
            std::string getBlkId();

            AstNode *initialCovariance(RandomVariable *var1, RandomVariable *var2, std::vector<ParameterEstimation *> parameterEstimations);

        private:
            PharmML::PharmMLContext *context;
            std::string blkId;
            std::vector<Parameter *> parameters;
            std::vector<PopulationParameter *> populationParameters;
            std::vector<IndividualParameter *> individualParameters;
            std::vector<RandomVariable *> randomVariables;
            std::vector<Correlation *> correlations;
    };
}

#endif
