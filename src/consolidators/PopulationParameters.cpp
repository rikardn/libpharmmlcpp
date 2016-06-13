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

#include <consolidators/PopulationParameters.h>

namespace CPharmML
{
    // Construct with PopulationParameter/Correlation as base
    PopulationParameter::PopulationParameter(pharmmlcpp::PopulationParameter *populationParameter) {
        this->populationParameter = populationParameter;
    }

    PopulationParameter::PopulationParameter(pharmmlcpp::Correlation *correlation) {
        // Construct name for the (normally unnamed) correlation parameter
        this->correlation = correlation;
        this->name = "";
    }

    // Add PharmML objects for consolidation with PopulationParameter
    void PopulationParameter::addCorrelation(pharmmlcpp::Correlation *corr) {
        this->correlation = corr;
        this->name = this->populationParameter->getSymbId();
    }

    void PopulationParameter::addRandomVariable(pharmmlcpp::RandomVariable *randomVariable) {
        this->randomVariables.push_back(randomVariable);
        this->variabilityParameter = true; // Are we sure? What if used as 'mean' in 'Normal1' for example?
    }

    void PopulationParameter::addIndividualParameter(pharmmlcpp::IndividualParameter *individualParameter) {
        this->individualParameters.push_back(individualParameter);
    }

    void PopulationParameter::addParameterEstimation(pharmmlcpp::ParameterEstimation *parameterEstimation) {
        this->parameterEstimation = parameterEstimation;
    }

    // Get PharmML objects used to consolidate
    pharmmlcpp::PopulationParameter *PopulationParameter::getPopulationParameter() {
        return this->populationParameter;
    }

    std::vector<pharmmlcpp::RandomVariable *> PopulationParameter::getRandomVariables() {
        return this->randomVariables;
    }

    std::vector<pharmmlcpp::IndividualParameter *> PopulationParameter::getIndividualParameters() {
        return this->individualParameters;
    }

    pharmmlcpp::Correlation *PopulationParameter::getCorrelation() {
        return this->correlation;
    }

    std::string PopulationParameter::getName() {
        return this->name;
    }

    // Set attributes
    void PopulationParameter::addDistributionName(std::string name) {
        this->distNames.insert(name);
    }

    void PopulationParameter::addDistributionParameterType(std::string name) {
        this->distParTypes.insert(name);
    }

    void PopulationParameter::setName(std::string name) {
        this->name = name;
    }

    // Get attributes
    pharmmlcpp::ParameterEstimation *PopulationParameter::getParameterEstimation() {
        return this->parameterEstimation;
    }

    bool PopulationParameter::isVariabilityParameter() {
        return (this->variabilityParameter);
    }

    bool PopulationParameter::isCorrelation() {
        return (this->correlation != nullptr);
    }

    std::string PopulationParameter::getDistributionName() {
        std::vector<std::string> vector(this->distNames.begin(), this->distNames.end());
        if (vector.size() == 1) {
            return vector[0];
        } else {
            return std::string("");
        }
    }

    std::string PopulationParameter::getDistributionParameterType() {
        std::vector<std::string> vector(this->distParTypes.begin(), this->distParTypes.end());
        if (vector.size() == 1) {
            return vector[0];
        } else {
            return std::string("");
        }
    }

    bool PopulationParameter::inDifferentParameterizations() {
        if (this->distNames.size() > 1 || this->distParTypes.size() > 1) {
            return true;
        } else {
            return false;
        }
    }

    // Wrapper class containing all CPharmML::PopulationParameter for a parameter model
    PopulationParameters::PopulationParameters(std::vector<pharmmlcpp::PopulationParameter *> populationParameters, std::vector<pharmmlcpp::Correlation *> correlations) {
         // Consolidate PharmML PopulationParameter's (e.g. for init statement generation)
        for (pharmmlcpp::PopulationParameter *pop_param : populationParameters) {
            // Create new consolidated population parameter
            CPharmML::PopulationParameter *cpop_param = new PopulationParameter(pop_param);
            this->populationParameters.push_back(cpop_param);
        }

        // Consolidate PharmML Correlation's (not always an associated PopulationParameter)
        for (pharmmlcpp::Correlation *corr : correlations) {
            bool nameless = true;
            if (corr->isPairwise() && corr->hasPureSymbRefAssigment()) {
                // Correlation refers purely to PopulationParameter for its assignment, i.e. it's not nameless
                for (CPharmML::PopulationParameter *cpop_param : this->populationParameters) {
                    pharmmlcpp::PopulationParameter *pop_param = cpop_param->getPopulationParameter();
                    bool depends_on_pop = corr->referencedSymbols.hasSymbol(pop_param);
                    if (depends_on_pop) {
                        cpop_param->addCorrelation(corr);
                        nameless = false;
                    }
                }
            }
            if (nameless) {
                // Create new consolidated (nameless) population parameter
                CPharmML::PopulationParameter *cpop_param = new PopulationParameter(corr);
                this->populationParameters.push_back(cpop_param);
            }
        }
    }

    void PopulationParameters::addRandomVariables(std::vector<pharmmlcpp::RandomVariable *> randomVariables) {
        for (CPharmML::PopulationParameter *cpop_param : this->populationParameters) {
            if (!cpop_param->isCorrelation()) {
                pharmmlcpp::PopulationParameter *pop_param = cpop_param->getPopulationParameter();
                // Find RandomVariable's referencing this PopulationParameter
                for (pharmmlcpp::RandomVariable *random_var : randomVariables) {
                    bool depends_on_pop = random_var->referencedSymbols.hasSymbol(pop_param);
                    if (depends_on_pop) {
                        cpop_param->addRandomVariable(random_var);

                        // Get distribution for variable
                        pharmmlcpp::Distribution *dist = random_var->getDistribution();
                        cpop_param->addDistributionName(dist->getName());

                        // Find DistributionParameter's referencing this PopulationParameter
                        for (pharmmlcpp::DistributionParameter *dist_param: dist->getDistributionParameters()) {
                            bool depends_on_pop = dist_param->refersIndirectlyTo(pop_param); // Try out the new Referer system
                            if (depends_on_pop) {
                                cpop_param->addDistributionParameterType(dist_param->getName());
                                // TODO: Transformation support (AstNode containing SymbRef to PopulationParameter should at least know if it's simple)
                            }
                        }
                    }
                }
            } else if (cpop_param->getName() == "") { // If nameless correlation, give it a name
                pharmmlcpp::Correlation *corr = cpop_param->getCorrelation();
                // Find RandomVariable's referenced by this Correlation
                std::vector<std::string> names;
                for (pharmmlcpp::RandomVariable *random_var : randomVariables) {
                    if (corr->referencedSymbols.hasSymbol(random_var)) {
                        cpop_param->addRandomVariable(random_var);
                        names.push_back(random_var->getSymbId());
                    }
                }

                // Try to find common prefix on random variable names
                bool unique = false;
                size_t prefix_len = 0;
                while (!unique) {
                    prefix_len++;
                    std::string prefix = names[0].substr(0, prefix_len);
                    for (auto it = names.begin()+1; it != names.end(); ++it) {
                        if ((*it).find(prefix, 0) == std::string::npos) {
                            unique = true;
                        }
                    }
                }

                // Set name
                std::string corr_name = "CORR";
                for (std::string name : names) {
                    name.replace(0, prefix_len-1, "");
                    corr_name += "_" + name;
                }
                cpop_param->setName(corr_name);
            }
        }
    }

    void PopulationParameters::addIndividualParameters(std::vector<pharmmlcpp::IndividualParameter *> individualParameters) {
        for (CPharmML::PopulationParameter *cpop_param : this->populationParameters) {
            pharmmlcpp::PopulationParameter *pop_param = cpop_param->getPopulationParameter();
            // Find IndividualParameter's refering this PopulationParameter
            for (pharmmlcpp::IndividualParameter *indiv_param : individualParameters) {
                bool depends_on_pop = indiv_param->referencedSymbols.hasSymbol(pop_param);
                if (depends_on_pop) {
                    cpop_param->addIndividualParameter(indiv_param);
                }
            }
        }
    }

    void PopulationParameters::addParameterEstimation(std::vector<pharmmlcpp::ParameterEstimation *> params_est) {
        for (CPharmML::PopulationParameter *cpop_param : this->populationParameters) {
            pharmmlcpp::PopulationParameter *pop_param = cpop_param->getPopulationParameter();
            // Find ParameterEstimation for this PopulationParameter
            for (pharmmlcpp::ParameterEstimation *est_param : params_est) {
                bool depends_on_pop = est_param->refersIndirectlyTo(pop_param);
                if (depends_on_pop) {
                    cpop_param->addParameterEstimation(est_param);
                    break;
                }
            }
        }
    }

    void PopulationParameters::addOptimalDesignStep(pharmmlcpp::OptimalDesignStep *optimalDesignStep) {
        this->addParameterEstimation(optimalDesignStep->getParameters());
    }

    void PopulationParameters::addEstimationStep(pharmmlcpp::EstimationStep *estimationStep) {
        this->addParameterEstimation(estimationStep->getParameters());
    }

    // Get all consolidated population parameter objects
    std::vector<PopulationParameter *> PopulationParameters::getPopulationParameters() {
        return this->populationParameters;
    }

    // Only get those that correspond to the correlations supplied (used for variability level sorting in MDL; don't know if optimal)
    std::vector<PopulationParameter *> PopulationParameters::getPopulationParameters(std::vector<pharmmlcpp::Correlation *> correlations) {
        std::vector<CPharmML::PopulationParameter *> result;
        for (pharmmlcpp::Correlation *corr : correlations) {
            for (PopulationParameter *cpop_param : this->populationParameters) {
                if (cpop_param->isCorrelation()) {
                    if (cpop_param->getCorrelation() == corr) {
                        result.push_back(cpop_param);
                    }
                }
            }
        }
        return result;
    }
}
