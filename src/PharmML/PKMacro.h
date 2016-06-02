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

#ifndef PHARMML_PKMACRO_H_
#define PHARMML_PKMACRO_H_

#include <vector>
#include <PharmML/PharmMLContext.h>
#include <PharmML/PharmMLSection.h>
#include <visitors/PharmMLVisitor.h>
#include <visitors/AstAnalyzer.h>
#include <symbols/SymbolSet.h>
#include <symbols/Symbol.h>
#include <AST/AstNode.h>

namespace PharmML
{
    typedef std::pair<std::string, AstNode *> MacroValue;
    class PKMacro : public PharmMLSection, public Referer
    {
        public:
            PKMacro(PharmMLContext *context, xml::Node node);

            std::string getType();
            bool hasAttribute(std::string attribute);
            std::vector<MacroValue> getValues();
            AstNode *getAssignment(std::string attribute);

            void gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap);
            void accept(PharmMLVisitor *visitor);

            // POST PARSE/CONSOLIDATION
            std::string generateName();
            void setName(std::string name);
            

        private:
            PharmML::PharmMLContext *context;
            void parse(xml::Node node);

            std::string type;
            std::vector<MacroValue> values;

            // POST PARSE/CONSOLIDATION
            std::string name;
    };

    class PKMacros : public PharmMLSection
    {
        public:
            PKMacros(PharmMLContext *context, xml::Node node);

            std::vector<PKMacro *> getAllMacros();

            void gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap);

            // POST PARSE/CONSOLIDATION

        private:
            PharmML::PharmMLContext *context;

            std::vector<PKMacro *> macros;
            
            void parse(xml::Node node);
            
            // POST PARSE/CONSOLIDATION
            void postParse();
    };
}

#endif
