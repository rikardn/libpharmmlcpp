/* libpharmmlcpp - Library to handle PharmML * Copyright (C) 2016 Rikard Nordgren and Gunnar Yngman
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

#ifndef PHARMMLCPP_MDLSYMBOLNAMER_H_
#define PHARMMLCPP_MDLSYMBOLNAMER_H_

#include <helpers/SymbolNamer.h>

// MDL RESERVED WORDS (IN MACRO FORM FOR CONVENIENCE)
// object names and their blocks (full chapter 1)
#define MDL_OBJECT_NAMES "mdlObj", "dataObj", "priorObj", "taskObj", "designObj", "parObj", "mogObj"
#define MDL_MDLOBJ_BLOCKS "COVARIATES", "FUNCTIONS", "GROUP_VARIABLES", "IDV", "INDIVIDUAL_VARIABLES", "MODEL_PREDICTION", "OBSERVATION", "POPULATION_PARAMETERS", "RANDOM_VARIABLE_DEFINITION", "STRUCTURAL_PARAMETERS", "VARIABILITY_LEVELS", "VARIABILITY_PARAMETERS"
#define MDL_DATAOBJ_BLOCKS "DATA_DERIVED_VARIABLES", "DATA_INPUT_VARIABLES", "DECLARED_VARIABLES", "FUNCTIONS", "SOURCE"
#define MDL_PRIOROBJ_BLOCKS "NON_CANONICAL_DISTRIBUTION", "PRIOR_PARAMETERS", "PRIOR_VARIABLE_DEFINITION"
#define MDL_TASKOBJ_BLOCKS "ESTIMATE", "EVALUATE", "OPTIMISE", "SIMULATE"
#define MDL_DESIGNOBJ_BLOCKS "DECLARED_VARIABLES", "DESIGN_PARAMETERS", "DESIGN_SPACES", "INTERVENTION", "POPULATION", "SAMPLING", "STUDY_DESIGN"
#define MDL_PAROBJ_BLOCKS "STRUCTURAL", "VARIABILITY"
#define MDL_MOGOBJ_BLOCKS "INFO", "OBJECTS"
// list definitions (from index of chapter 3; might not be sufficient wrt e.g. attributes)
#define MDL_LIST_DEFINITIONS "AdminBolusList", "AdminComplexList", "AdminInfusionList", "AdminResetAllList", "AdminResetList", "AdminSuperList", "Amt", "BlqList", "CatCovariate", "CatObs", "CensCol", "Cmt", "CmtDirect", "Compartment", "ContinuousObsList", "CorrelationList", "CountObs", "CovarianceList", "Covariate", "DDCatCovariate", "DDCovariate", "DDVariable", "DataColumn", "DataVarLevel", "DataVariable", "Depot", "DerivList", "DerivedSamplingList", "DesignSpaceArmSize", "DesignSpaceBolusAmt", "DesignSpaceCov", "DesignSpaceDoseTime", "DesignSpaceDuration", "DesignSpaceInfAmt", "DesignSpaceNumArms", "DesignSpaceNumTimes", "DesignSpaceParam", "DesignSpaceSampleTimes", "DiscreteObs", "Distribution", "DoseInterval", "DoseTime", "Dv", "Dvid", "Effect", "Elimination", "Evid", "Id", "Idv", "IndivAbstractList", "IndivParamGeneral", "IndivParamLinear", "IndivRvList", "IndivUserDefined", "MdlObjInMog", "Mdv", "PopnCategoricalList", "PopnContinuousList", "PopulationTemplate", "PriorMatrixInput", "PriorSourceCsv", "PriorTabularSource", "PriorVectorInput", "SamplingSuper", "SimpleSamplingList", "Source", "StructuralEstimateMatrix", "StructuralEstimateReal", "StructuralEstimateVector", "StudyDesign", "Transfer", "TteObs", "UserDefinedObsList", "VarEstimateMatrix", "VarEstimateReal", "VarEstimateVector", "additiveErrorList", "addl", "blkDefn", "combinedErrorList", "combinedErrorList", "dosingTarget", "dosingVar", "ignore", "ii", "observation", "proportionalErrorList", "rate", "ss", "varLevel"
// sublist definitions (from index of chapter 4; might not be sufficient wrt e.g. attributes)
#define MDL_SUBLIST_DEFINITIONS "fixEffAtts", "priorFormat", "ResetSublist", "intSeqAtts", "sampSeqAtts", "OccSeqSubList", "PopTmpltCovSublist"
// function definitions (from index of chapter 5; might not be sufficient wrt e.g. arguments)
#define MDL_FUNCTIONS "Bernoulli", "Beta", "Binomial", "CategoricalNonordered", "CategoricalOrdered", "Empirical", "Gamma", "Gamma", "InverseGamma", "InverseWishart", "LogNormal", "LogNormal", "LogNormal", "LogNormal", "LogNormal", "MixtureDistribution", "MultiEmpirical", "MultiNonParametric", "MultivariateNormal", "MultivariateNormal", "MultivariateStudentT", "MultivariateStudentT", "NegativeBinomial", "NonParametric", "Normal", "Normal", "Normal", "Normal", "Poisson", "StandardNormal", "SublistDefinitions", "fixEffAtts", "priorFormat", "ResetSublist", "intSeqAtts", "sampSeqAtts", "OccSeqSubList", "PopTmpltCovSublist", "StandardUniform", "StudentT", "StudentT", "Uniform", "Wishart", "Wishart", "ZeroInflatedPoisson", "abs", "acos", "acosh", "asVector", "asin", "asinh", "atan", "atanh", "ceiling", "chol", "constInterp", "cos", "cosh", "cubicInterp", "det", "diagonal", "dseq", "eigen", "exp", "factorial", "floor", "gInv", "invLogit", "invProbit", "inverse", "lastValueInterp", "linearInterp", "ln", "lnFactorial", "log", "log", "log", "logit", "matrix", "max", "mean", "median", "min", "nearestInterp", "pchipInterp", "probit", "rep", "seq", "seqby", "sin", "sinh", "splineInterp", "sqrt", "sum", "tan", "tanh", "toInt", "toMatrixByCol", "toMatrixByRow", "transpose", "triangle"
// standard types/mapping types (full chapter 6/7)
#define MDL_STANDARD_TYPES_NAMES "Mapping", "boolean", "deriv", "function", "genericCategoryValue", "genericEnum", "int", "matrix", "pdf", "pmf", "randomVariable", "real", "reference", "string", "undefined", "vector"
#define MDL_STANDARD_TYPES_CLASSES "_mapping", "_bool", "_deriv", "_function", "_categoryValue", "_category", "_int", "_matrix", "_pdf", "_pmf", "_rv", "_real", "_reference", "_str", "_undef", "_vector"
#define MDL_MAPPING_TYPES_NAMES "DoseMapping", "ObsMapping"
// builtin enumeration types and their enumerations (full chapter 8)
#define MDL_ENUMERATION_TYPES "AdminTypeType", "CorrelationEnumType", "IndivTypeType", "OptAlgoType", "PopnType", "PopnTypeType", "blkMethodType", "blkType", "cmtType", "covArgEnumType", "ddvUse", "divUse", "estAlgo", "estimateType", "input", "objType", "obstype", "priorElementType", "priorInput", "sampleElement", "sampletype", "solverType", "transType", "varLvlType", "varType"
#define MDL_ENUMERATION_TYPES_ENUMERATIONS "bolus", "infusion", "combi", "resetAll", "reset", "covariance", "correlation", "linear", "general", "rv", "simplex", "fw", "template", "continuous", "categorical", "m1", "m2", "m3", "m4", "m1", "m2", "m3", "m4", "depot", "compartment", "elimination", "transfer", "distribution", "direct", "effect", "constant", "idvDependent", "doseTime", "doseInterval", "covariate", "catCov", "variable", "dvid", "covariate", "variable", "amt", "dv", "dvid", "cmt", "mdv", "idv", "id", "rate", "ignore", "varLevel", "catCov", "ss", "ii", "addl", "evid", "cens", "saem", "foce", "fo", "focei", "mcmc", "scalar", "vector", "matrix", "nonmemFormat", "mdlObj", "dataObj", "parObj", "taskObj", "designObj", "priorObj", "combinedError1", "combinedError2", "additiveError", "proportionalError", "categorical", "count", "discrete", "tte", "continuous", "userDefined", "matrix", "vector", "csv", "bolusAmt", "infAmt", "duration", "sampleTime", "numberTimes", "covariate", "numberArms", "armSize", "parameter", "doseTime", "simple", "combi", "stiff", "nonStiff", "none", "ln", "logit", "probit", "parameter", "observation", "cov", "corr", "sd", "var"

namespace pharmmlcpp
{
    // Superset of all MDL reserved words
    const std::unordered_set<std::string> MDL_RESERVED_WORDS = {
        MDL_OBJECT_NAMES, MDL_MDLOBJ_BLOCKS, MDL_DATAOBJ_BLOCKS, MDL_PRIOROBJ_BLOCKS, MDL_TASKOBJ_BLOCKS, MDL_DESIGNOBJ_BLOCKS, MDL_PAROBJ_BLOCKS, MDL_MOGOBJ_BLOCKS
        MDL_LIST_DEFINITIONS,
        MDL_SUBLIST_DEFINITIONS,
        MDL_FUNCTIONS,
        MDL_STANDARD_TYPES_NAMES, MDL_STANDARD_TYPES_CLASSES, MDL_MAPPING_TYPES_NAMES,
        MDL_ENUMERATION_TYPES, MDL_ENUMERATION_TYPES_ENUMERATIONS
    };

    class MDLSymbolNamer : public SymbolNamer
    {
        public:
            MDLSymbolNamer() : SymbolNamer(MDL_RESERVED_WORDS) {
                addCharSet(std::unordered_set<char32_t>(LatinChars::ALL_EN.begin(), LatinChars::ALL_EN.end()));
                subInitialCharSet(std::unordered_set<char32_t>(LatinChars::DIGITS.begin(), LatinChars::DIGITS.end()));
            };
    };
}

#endif
