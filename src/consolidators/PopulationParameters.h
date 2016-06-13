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

#ifndef CPHARMMLCPP_POPULATIONPARAMETERS_H_
#define CPHARMMLCPP_POPULATIONPARAMETERS_H_

#include <string>
#include <unordered_set>
#include <vector>
#include <symbols/PopulationParameter.h>
#include <symbols/RandomVariable.h>
#include <symbols/IndividualParameter.h>
#include <PharmML/ModellingSteps.h>
#include <PharmML/Correlation.h>

namespace CPharmML
{
    class PopulationParameter
    {
        public:
            // Construct with PopulationParameter/Correlation as base
            PopulationParameter(pharmmlcpp::PopulationParameter *populationParameter);
            PopulationParameter(pharmmlcpp::Correlation *correlation);

            // Add PharmML objects for consolidation with PopulationParameter
            void addCorrelation(pharmmlcpp::Correlation *corr);
            void addRandomVariable(pharmmlcpp::RandomVariable *randomVariable);
            void addIndividualParameter(pharmmlcpp::IndividualParameter *individualParameter);
            void addParameterEstimation(pharmmlcpp::ParameterEstimation *parameterEstimation);

            // Get PharmML objects used to consolidate
            pharmmlcpp::PopulationParameter *getPopulationParameter();
            std::vector<pharmmlcpp::RandomVariable *> getRandomVariables();
            std::vector<pharmmlcpp::IndividualParameter *> getIndividualParameters();
            pharmmlcpp::ParameterEstimation *getParameterEstimation();
            pharmmlcpp::Correlation *getCorrelation();

            // Add attributes
            void addDistributionName(std::string name);
            void addDistributionParameterType(std::string name);
            void setName(std::string name);

            // Get attributes
            bool isVariabilityParameter();
            bool isCorrelation();
            std::string getDistributionName();
            std::string getDistributionParameterType();
            bool inDifferentParameterizations();
            std::string getName();

        private:
            // PharmML objects used to consolidate
            pharmmlcpp::PopulationParameter *populationParameter = nullptr;
            pharmmlcpp::Correlation *correlation = nullptr;
            std::vector<pharmmlcpp::RandomVariable *> randomVariables;
            std::vector<pharmmlcpp::IndividualParameter *> individualParameters;
            pharmmlcpp::ParameterEstimation *parameterEstimation = nullptr;

            bool variabilityParameter = false;
            std::unordered_set<std::string> distNames;
            std::unordered_set<std::string> distParTypes;
            std::string name; // For Correlations lacking PharmML naming
    };

    class PopulationParameters
    {
        public:
            PopulationParameters(std::vector<pharmmlcpp::PopulationParameter *> populationParameters, std::vector<pharmmlcpp::Correlation *> correlations);
            void addRandomVariables(std::vector<pharmmlcpp::RandomVariable *> randomVariables);
            void addIndividualParameters(std::vector<pharmmlcpp::IndividualParameter *> individualParameters);
            void addEstimationStep(pharmmlcpp::EstimationStep *estimationStep);
            void addOptimalDesignStep(pharmmlcpp::OptimalDesignStep *optimalDesignStep);

            std::vector<PopulationParameter *> getPopulationParameters();
            std::vector<PopulationParameter *> getPopulationParameters(std::vector<pharmmlcpp::Correlation *> correlations);

        private:
            std::vector<PopulationParameter *> populationParameters;
            void addParameterEstimation(std::vector<pharmmlcpp::ParameterEstimation *> params_est);
    };
}

#endif
