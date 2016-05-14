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

#ifndef CPHARMML_COVARIATES_H_
#define CPHARMML_COVARIATES_H_

#include <string>
#include <unordered_set>
#include <AST/AstNode.h>
#include <symbols/Symbol.h>
#include <symbols/Covariate.h>
#include <PharmML/Dataset.h>

namespace CPharmML
{
    class Covariate
    {
        public:
            // Construct with PharmML::Covariate as base
            Covariate(PharmML::Covariate *covariate);
            
            // Add PharmML objects for consolidation (in this order)
            void addColumnMapping(PharmML::ColumnMapping *columnMapping);
            void addColumnDefinition(PharmML::ColumnDefinition *covariateColumnDef);
            
            // Get attributes
            PharmML::Covariate *getCovariate();
            PharmML::ColumnMapping *getColumnMapping();
            PharmML::ColumnDefinition *getColumnDefinition();
            
            std::string getName();
            bool isDerived();
            bool isContinuous();
            std::string getType();
            PharmML::Distribution *getDistribution();
            PharmML::AstNode *getDefinition();
            std::string getColumnId();
        
        private:
            // PharmML objects used to consolidate
            PharmML::Covariate *covariate;
            PharmML::ColumnMapping *columnMapping = nullptr;
            PharmML::ColumnDefinition *columnDef = nullptr;
            
            std::string name;
            bool derived;
            bool continuous;
            std::string type;
            PharmML::Distribution *distribution;
            PharmML::AstNode *definition;
            std::string columnId;
    };
}

#endif
