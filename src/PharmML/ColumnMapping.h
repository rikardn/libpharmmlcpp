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

#ifndef PHARMML_COLUMNMAPPING_H_
#define PHARMML_COLUMNMAPPING_H_

#include <unordered_map>
#include <helpers/StringTools.h>
#include <helpers/Logger.h>
#include <xml/xml.h>
#include <PharmML/PharmMLSection.h>
#include <PharmML/PharmMLContext.h>
#include <visitors/PharmMLVisitor.h>
#include <visitors/XMLAstVisitor.h>
#include <visitors/AstAnalyzer.h>
#include <symbols/Symbol.h>
#include <PharmML/PKMacro.h>
#include <symbols/SymbolGathering.h>
#include <symbols/MacroGathering.h>

namespace PharmML
{
    class MacroGathering; // FIXME: Inclusion problems!
    struct MapType {
        // Raw strings
        std::string dataSymbol;
        std::string modelSymbol;
        std::string admNumber;

        // Filled in by setupTargetMapping
        Symbol *symbol;
        PKMacro *macro;
    };

    // TODO: Move elsewhere (Dataset.h?)
    class TargetMapping : public PharmMLSection
    {
        public:
            TargetMapping(PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);

            std::string getBlkIdRef();
            std::vector<MapType> getMaps();

            std::unordered_set<std::string> getSymbolStrings();
            std::unordered_set<int> getAdmNumbers();
            std::unordered_map<std::string, PharmML::Symbol *> getDataSymbolMap();
            std::unordered_map<std::string, PharmML::PKMacro *> getDataMacroMap();

            void setupSymbolRefs(PharmML::SymbolGathering &gathering);
            void setupMacroRefs(PharmML::MacroGathering &gathering);
            void accept(PharmMLVisitor *visitor);

        private:
            PharmML::PharmMLContext *context;
            std::string blkIdRef;
            std::vector<MapType> maps;
    };

    class ColumnMapping : public Referer
    {
        public:
            ColumnMapping(PharmML::PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            xml::Node xml();
            
            AstNode *getAssignment();
            std::string getColumnIdRef();

            Symbol *getMappedSymbol();

            TargetMapping *getTargetMapping();

            void setupSymbRefs(SymbolGathering &gathering, std::string blkId) override;
            void accept(PharmMLVisitor *visitor);

        private:
            PharmML::PharmMLContext *context;

            std::string columnIdRef;
            AstNode *assignment = nullptr;
            SymbRef *symbRef = nullptr;
            Symbol *mappedSymbol = nullptr;
            TargetMapping *target_map = nullptr;
    };
}

#endif
