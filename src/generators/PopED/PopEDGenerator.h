/* libpharmml cpp - Library to handle PharmML
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

#ifndef PHARMMLCPP_POPEDGENERATOR_H_
#define PHARMMLCPP_POPEDGENERATOR_H_

#include <string>
#include <visitors/AstNodeVisitor.h>
#include <symbols/PopulationParameter.h>
#include <symbols/IndividualParameter.h>
#include <symbols/Variable.h>
#include <PharmML/PKMacro.h>
#include <generators/R/RAstGenerator.h>
#include <generators/R/RPharmMLGenerator.h>
#include <visitors/PharmMLVisitor.h>
#include <symbols/ObservationModel.h>
#include <PharmML/PharmML.h>
#include <generators/PopED/PopEDAstGenerator.h>
#include <generators/TextFormatter.h>
#include <generators/R/RSymbols.h>
#include <generators/PopED/PopEDPastDerivativesSymbols.h>
#include <generators/PopED/PopEDErrorAstGenerator.h>
#include <helpers/Logger.h>
#include <generators/PopED/PopEDObjects.h>
#include <generators/R/RSymbolNamer.h>

namespace pharmmlcpp
{
    class PopEDGenerator
    {
        public:
            std::string generateModel(PharmML *model);

        private:
            Logger logger;
            RSymbolNamer symbolNamer;
            RAstGenerator ast_gen;
            RSymbols r_symb;
            PopEDAstGenerator poped_astgen;
            RPharmMLGenerator r_gen;
            PopEDObjects td_visitor;
            int nArms;          // Number of arms

            PharmML *model;
            std::vector<Symbol *> derivs;       // Derivative symbols in (some) order
            std::vector<RandomVariable *> etas;
            std::vector<Symbol *> designParameters;     // Design parameter with connection to DesignSpaces

            std::string accept(AstNode *);
            std::string genParameterModel();
            std::string genODEFunc();
            std::string genStructuralModel();
            std::string genErrorFunction();
            std::string genDatabaseCall();
            std::string getDoseVariable();
            void collectTrialDesignInformation();
            Symbol *findSigmaSymbol();

            // Logger methods
            void warnOperationPropertyUnexpectedType(OperationProperty *prop, std::string exp_type);
            void warnOperationPropertyUnderflow(OperationProperty *prop, int min);
            void warnOperationPropertyUnexpectedValue(OperationProperty *prop, std::vector<std::string> exp_strings);
            void warnOperationPropertyUnknown(OperationProperty *prop);

            // Constants
            const std::string InfusionFunction = R"(inf_func <- function(start,rate,amount,Time) {
    duration = rate/amount
    if(Time >= start && Time < (start+duration)) {
        return(amount/duration)
    } else {
        return(0)
    }
})";
    };
}

#endif
