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

#include "StructuralModel.h"

namespace PharmML
{
    StructuralModel::StructuralModel(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void StructuralModel::parse(xml::Node node) {
        this->blkId = node.getAttribute("blkId").getValue();
        std::vector<xml::Node> array = this->context->getElements(node, ".//ct:Variable");
        for (xml::Node n : array) {
            PharmML::Variable *var = new PharmML::Variable(this->context, n);
            this->variables.push_back(var);
        }
        std::vector<xml::Node> derivs = this->context->getElements(node, ".//ct:DerivativeVariable");
        for (xml::Node n : derivs) {
            PharmML::DerivativeVariable *var = new PharmML::DerivativeVariable(this->context, n);
            this->variables.push_back(var);
        }
    }

    std::vector<PharmML::CommonVariable *> StructuralModel::getVariables() {
        return this->variables;
    }

    // Return all derivative variables
    std::vector<PharmML::CommonVariable *> StructuralModel::getDerivatives() {
        std::vector<PharmML::CommonVariable *> derivs;
        for (CommonVariable *var : this->variables) {
            if (var->isDerivative()) {
                derivs.push_back(var);
            }
        }

        return derivs;
    }

    // Get all needed prerequisites of a list of variables
    std::vector<PharmML::CommonVariable *> StructuralModel::getPrerequisiteVariables(std::vector<PharmML::CommonVariable *> list) {
        bool added = true;
        while (added) {
            added = false;            
        }

        return list;
    }
}
