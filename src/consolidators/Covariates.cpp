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
    // Construct with nothing as base
    Covariate::Covariate() {
        
    }
    
    // Add PharmML objects for consolidation
    void Covariate::addCovariate(PharmML::Covariate *covariate) {
        this->covariate = covariate;
        this->name = covariate->getTransformedName();
        this->definition = covariate->getAssignment();
    }
    
    void Covariate::addColumnDefinition(PharmML::ColumnDefinition *covariateColumnDef) {
        this->columnDef = covariateColumnDef;
        this->derived = true;
        this->name = covariateColumnDef->getId();
    }
    
    // Get attributes
    std::string Covariate::getName() {
        return this->name;
    }
    
    PharmML::AstNode *Covariate::getDefinition() {
        return this->definition;
    }
    
    bool Covariate::isDerived() {
        return this->derived;
    }
    
    // COVARIATE CONSOLIDATOR
    // Construct with nothing as base
    Covariates::Covariates() {
        
    }
    
    // Add PharmML objects for consolidation
    void Covariates::addCovariate(PharmML::Covariate *covariate) {
        std::string name = covariate->getTransformedName();
        Covariate *foundCov = this->getCovariateByName(name);
        if (foundCov) {
            foundCov->addCovariate(covariate);
        } else {
            Covariate *newCov = new Covariate();
            newCov->addCovariate(covariate);
            this->covariates.insert(newCov);
        }
    }
    
    void Covariates::addColumnDefinition(PharmML::ColumnDefinition *covariateColumnDef) {
        std::string name = covariateColumnDef->getId();
        Covariate *foundCov = this->getCovariateByName(name);
        if (foundCov) {
            foundCov->addColumnDefinition(covariateColumnDef);
        } else {
            Covariate *newCov = new Covariate();
            newCov->addColumnDefinition(covariateColumnDef);
            this->covariates.insert(newCov);
        }
    }
    
    void Covariates::addColumnMapping(PharmML::ColumnMapping *columnMapping) {
        
    }
    
    // Match an already added covariate by name
    Covariate *Covariates::getCovariateByName(std::string name) {
        for (Covariate *cov : this->covariates) {
            if (cov->getName() == name) {
                return cov;
            }
        }
        return nullptr;
    }
}
