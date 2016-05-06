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

#ifndef PHARMML_VARIABILITYLEVEL_H_
#define PHARMML_VARIABILITYLEVEL_H_

#include <PharmML/PharmMLContext.h>
#include <visitors/PharmMLVisitor.h>

namespace PharmML
{
    class VariabilityLevel : public Symbol
    {
        PharmML::PharmMLContext *context;
        std::string name;
        bool referenceLevel;
        AstNode *parentLevelRef = nullptr;

        public:
        VariabilityLevel(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        std::string getName();
        bool isReferenceLevel();
        AstNode *getParentReference();
        void accept(PharmMLVisitor *visitor);
        void accept(SymbolVisitor *visitor);
    };
    
    class VariabilityReference
    {
        PharmML::PharmMLContext *context;
        AstNode *variabilityLevelReference;
        AstNode *randomEffectsMapping = nullptr;
        
        public:
        VariabilityReference(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
        AstNode *getLevelReference();
        AstNode *getRandomEffectsMapping();
        //~ void accept(AstNodeVisitor *visitor);
    };
}

#endif
