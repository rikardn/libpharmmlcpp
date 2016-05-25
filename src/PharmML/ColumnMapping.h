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
#include <xml/xml.h>
#include <PharmML/PharmMLContext.h>
#include <visitors/PharmMLVisitor.h>
#include <visitors/XMLAstVisitor.h>
#include <visitors/AstAnalyzer.h>
#include <symbols/Symbol.h>

namespace PharmML
{
    struct MapType {
        std::string dataSymbol;
        std::string modelSymbol;
        std::string admNumber;
    };

    // TODO: Move elsewhere (Dataset.h?)
    class TargetMapping
    {
        public:
            TargetMapping(PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);

            std::string getBlkIdRef();
            std::vector<MapType> getMaps();

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
            std::unordered_map<int, std::string> getAdministrationMap();

            void gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap);
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
