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

#ifndef PHARMML_POPEDOBJECTS_H_
#define PHARMML_POPEDOBJECTS_H_

#include <visitors/ObjectVisitor.h>
#include <visitors/StringVisitor.h>
#include <helpers/Logger.h>
#include <generators/R/RAstGenerator.h>
#include <generators/TextFormatter.h>
#include <PharmML/Interventions.h>

namespace PharmML
{
    class PopEDObjects : public ObjectVisitor, public StringVisitor
    {
        private:
            RAstGenerator rast;

            TextFormatter xt_formatter;
            TextFormatter a_formatter;
            std::vector<std::string> doseNames;
            std::vector<std::string> timeNames;
            std::vector<IndividualAdministration *> individualAdministrations;
            std::string generateIndividualAdministration(IndividualAdministration *individualAdministration);

        public:
            PopEDObjects();
            std::string getDatabaseAdditions();
            void setIndividualAdministrations(std::vector<IndividualAdministration *> individualAdministrations);

            void visit(Arm *object) override;
            void visit(Administration *object) override;
            //void visit(Observation *object) override;
            void visit(IndividualObservations *object) override;
    };
}

#endif
