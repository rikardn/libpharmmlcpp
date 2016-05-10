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

#ifndef PHARMML_INDIVIDUALPARAMETER_H_
#define PHARMML_INDIVIDUALPARAMETER_H_

#include <PharmML/PharmMLContext.h>
#include <xml/xml.h>
#include <AST/AstNode.h>
#include <visitors/PharmMLVisitor.h>
#include <PharmML/PharmMLSection.h>
#include <PharmML/Dependencies.h>

namespace PharmML
{
    class IndividualParameter : public PharmMLSection, public Symbol
    {
        PharmMLContext *context;
        std::string transformation;
        bool is_structured;
        AstNode *PopulationValue;
        AstNode *RandomEffects;
        AstNode *FixedEffect = nullptr;
        AstNode *Covariate = nullptr;
        AstNode *assignment;
        Dependencies deps;

        public:
        IndividualParameter(PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getTransformation();
        AstNode *getPopulationValue();
        AstNode *getRandomEffects();
        AstNode *getFixedEffect();
        AstNode *getCovariate();
        AstNode *getAssignment();
        bool isStructured();
        Dependencies &getDependencies();
        void accept(PharmMLVisitor *visitor);
        void accept(SymbolVisitor *visitor);
    };
}

#endif
