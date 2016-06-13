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
#include <symbols/Symbol.h>
#include <AST/symbols.h>

namespace pharmmlcpp
{
    class VariabilityLevel : public Symbol
    {
        public:
            VariabilityLevel(pharmmlcpp::PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            std::string getName();
            bool isReferenceLevel();
            SymbRef *getParentReference();
            void setupSymbRefs(SymbolGathering &gathering, std::string blkId);
            void accept(PharmMLVisitor *visitor);
            void accept(SymbolVisitor *visitor);
        
        private:
            std::string name;
            bool referenceLevel;
            SymbRef *parentLevelRef = nullptr;
    };

    class VariabilityReference : public Referer
    {
        public:
            VariabilityReference(pharmmlcpp::PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            SymbRef *getLevelReference();
            void setupSymbRefs(SymbolGathering &gathering, std::string blkId);
            AstNode *getRandomEffectsMapping();
            //~ void accept(AstNodeVisitor *visitor);
        
        private:
            pharmmlcpp::PharmMLContext *context;
            SymbRef *levelReference;
            AstNode *randomEffectsMapping = nullptr;
    };
}

#endif
