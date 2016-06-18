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

#ifndef PHARMMLCPP_POPEDOBJECTS_H_
#define PHARMMLCPP_POPEDOBJECTS_H_

#include <visitors/ObjectVisitor.h>
#include <visitors/StringVisitor.h>
#include <helpers/Logger.h>
#include <generators/R/RAstGenerator.h>
#include <generators/TextFormatter.h>
#include <PharmML/Interventions.h>

namespace pharmmlcpp
{
    class PopEDObjects : public ObjectVisitor, public StringVisitor
    {
        public:
            PopEDObjects();
            TextFormatter& getDatabaseXT();
            TextFormatter& getDatabaseA();
            void setIndividualAdministrations(std::vector<IndividualAdministration *> individualAdministrations);
            std::vector<std::string> getDoseNames();
            std::vector<std::string> getTimeNames();
            bool hasInfusions();
            bool hasBoluses();

            // FIXME: Rushed methods for combining stuff. Should probably be rethinked.
            AstNode *getCombinationStart();
            AstNode *getInterventionStart();
            std::vector<std::string> getInfFuncCalls();
            std::unordered_map<Symbol *, std::vector<std::string>> getInfusionMap();
            std::vector<AstNode *> getDoseTimes();
            int getNumObservations();
            std::unordered_map<Symbol *, std::vector<AstNode *>> getBolusAmounts();
            std::unordered_map<Symbol *, std::vector<AstNode *>> getBolusTimes();

            void visit(Arm *object) override;
            void visit(Administration *object) override;
            void visit(InterventionsCombination *object) override;
            void visit(Observation *object) override;
            void visit(IndividualObservations *object) override;

        private:
            RAstGenerator rast;

            TextFormatter xt_formatter;
            TextFormatter a_formatter;
            std::vector<std::string> doseNames;
            std::vector<std::string> timeNames;
            bool has_infusions = false;
            bool has_boluses = false;
            std::vector<IndividualAdministration *> individualAdministrations;
            std::string generateIndividualAdministration(IndividualAdministration *individualAdministration);
            std::string generateAdministration(Administration *administration);
            AstNode *combination_start;
            AstNode *intseq_start;
            std::vector<std::string> infFuncCalls;
            std::unordered_map<Symbol *, std::vector<std::string>> infusionMap;
            std::vector<AstNode *> doseTimes;
            int numObservations = 0;
            std::unordered_map<Symbol *, std::vector<AstNode *>> bolusAmounts;
            std::unordered_map<Symbol *, std::vector<AstNode *>> bolusTimes;
    };
}

#endif
