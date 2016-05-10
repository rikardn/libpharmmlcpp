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

#include "MDLPharmMLConsolidator.h"
#include "MDLGenerator.h"

namespace PharmML
{
    // Parameter object consolidator
    void MDLPharmMLConsolidator::addPopulationParameter(PopulationParameter *param, MDLGenerator &gen) {
        param->accept(&gen);
        std::string symbId = gen.getValue();
        this->populationParameters.push_back(symbId);
    }
    
    void MDLPharmMLConsolidator::addIndividualParameter(IndividualParameter *param) {
        
    }
    
    void MDLPharmMLConsolidator::addRandomVariable(RandomVariable *var) {
        
    }
    
    void MDLPharmMLConsolidator::combine() {

    }

    std::string MDLPharmMLConsolidator::genStructuralBlock() {
        // Generate MDL STRUCTURAL block
        RFormatter form;
        
        form.indentAdd("STRUCTURAL {");
        form.addMany(this->populationParameters);
        form.outdentAdd("}");
        
        return form.createString(); 
    }
}
