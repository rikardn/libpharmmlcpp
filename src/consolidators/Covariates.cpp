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

#include <consolidators/Covariates.h>

namespace CPharmML
{
    // INDIVIDUAL COVARIATES
    // Construct with PharmML::Covariate as base
    Covariate::Covariate(PharmML::Covariate *covariate) {
        this->covariate = covariate;
        
        this->name = covariate->getSymbId();
        this->transformedName = covariate->getTransformedName();
        this->definition = covariate->getAssignment();
    }
    
    // Add PharmML objects for consolidation (in this order)
    void Covariate::addColumnMapping(PharmML::ColumnMapping *columnMapping) {
        this->columnMapping = columnMapping;
        
        this->columnId = columnMapping->getColumnIdRef();
    }
    
    void Covariate::addColumnDefinition(PharmML::ColumnDefinition *covariateColumnDef) {
        this->columnDef = covariateColumnDef;
    }
    
    // Get attributes
    PharmML::Covariate *Covariate::getCovariate() {
        return this->covariate;
    }
    
    std::string Covariate::getName() {
        return this->name;
    }
    
    std::string Covariate::getColumnId() {
        return this->columnId;
    }
    
    PharmML::AstNode *Covariate::getDefinition() {
        return this->definition;
    }
    
    bool Covariate::isDerived() {
        return this->derived;
    }
    
    // COVARIATE CONSOLIDATOR
    // Add PharmML objects for consolidation (in this order)
    void Covariates::addCovariate(PharmML::Covariate *covariate) {
        Covariate *newCov = new Covariate(covariate);
        this->covariates.insert(newCov);
    }
    
    void Covariates::addColumnMapping(PharmML::ColumnMapping *columnMapping) {
        for (Covariate *cov : this->covariates) {
            PharmML::Symbol *cov_symbol = cov->getCovariate();
            if (columnMapping->referencedSymbols.hasSymbol(cov_symbol)) {
                cov->addColumnMapping(columnMapping);
            }
        }
    }
    
    void Covariates::addColumnDefinition(PharmML::ColumnDefinition *covariateColumnDef) {
        for (Covariate *cov : this->covariates) {
            std::string id = cov->getColumnId();
            if (covariateColumnDef->getId() == id) {
                cov->addColumnDefinition(covariateColumnDef);
            }
        }
    }
}
