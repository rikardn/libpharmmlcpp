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

#ifndef PHARMMLCPP_COLUMNMAPPING_H_
#define PHARMMLCPP_COLUMNMAPPING_H_

#include <unordered_map>
#include <helpers/StringTools.h>
#include <helpers/Logger.h>
#include <xml/xml.h>
#include <PharmML/PharmMLSection.h>
#include <visitors/PharmMLVisitor.h>
#include <visitors/XMLAstVisitor.h>
#include <visitors/AstAnalyzer.h>
#include <symbols/Symbol.h>
#include <PharmML/PKMacro.h>
#include <symbols/SymbolGathering.h>
#include <symbols/MacroGathering.h>

namespace pharmmlcpp
{
    class MacroGathering; // FIXME: Inclusion problems!
    struct MapType {
        // Raw strings
        std::string dataSymbol;
        std::string modelSymbol;
        std::string admNumber;

        // Filled in by setupTargetMapping
        Symbol *symbol = nullptr;
        PKMacro *macro = nullptr;
    };

    // TODO: Move elsewhere (Dataset.h?)
    class TargetMapping : public PharmMLSection
    {
        public:
            TargetMapping(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);

            std::string getBlkIdRef();
            std::vector<MapType> getMaps();

            std::unordered_set<std::string> getSymbolStrings();
            std::unordered_set<int> getAdmNumbers();
            std::unordered_map<std::string, Symbol *> getDataSymbolMap();
            std::unordered_map<std::string, PKMacro *> getDataMacroMap();

            void setupSymbolRefs(SymbolGathering &gathering);
            void setupMacroRefs(MacroGathering &gathering);
            void accept(PharmMLVisitor *visitor);

        private:
            std::string blkIdRef;
            std::vector<MapType> maps;
    };

    // TODO: Move elsewhere (Dataset.h?)
    class CategoryMapping : public PharmMLSection
    {
        public:
            CategoryMapping(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);

            std::vector<std::string> getCategories();
            std::unordered_map<std::string, std::string> getMap();

        private:
            std::vector<MapType> maps;
    };

    class ColumnMapping : public Referer
    {
        public:
            ColumnMapping(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            xml::Node xml();

            bool isCategorical() { return this->category_map != nullptr; };
            CategoryMapping *getCategoryMapping() { return this->category_map; };
            std::shared_ptr<AstNode> getAssignment();
            std::shared_ptr<AstNode> getPiecewise();
            std::string getColumnIdRef();

            Symbol *getMappedSymbol();
            PKMacro *getMappedMacro();

            TargetMapping *getTargetMapping();

            void setupSymbRefs(SymbolGathering &gathering, std::string blkId) override;
            void accept(PharmMLVisitor *visitor);

        private:
            std::string columnIdRef;
            std::shared_ptr<AstNode> assignment;
            std::shared_ptr<AstNode> piecewise;
            SymbRef *symbRef = nullptr;
            Symbol *mappedSymbol = nullptr;
            TargetMapping *target_map = nullptr;
            CategoryMapping *category_map = nullptr;
    };
}

#endif
