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

#ifndef PHARMMLCPP_MULTIPLEDVMAPPING_H_
#define PHARMMLCPP_MULTIPLEDVMAPPING_H_

#include <unordered_map>
#include <helpers/StringTools.h>
#include <helpers/Logger.h>
#include <xml/xml.h>
#include <PharmML/PharmMLSection.h>
#include <PharmML/PharmMLReader.h>
#include <visitors/PharmMLVisitor.h>
#include <visitors/XMLAstVisitor.h>
#include <visitors/AstAnalyzer.h>
#include <symbols/Symbol.h>
#include <symbols/SymbolGathering.h>

namespace pharmmlcpp
{
    class MultipleDVMapping : public Referer, public PharmMLSection
    {
        public:
            MultipleDVMapping(PharmMLReader &reader, xml::Node node);
            void parse(PharmMLReader &reader, xml::Node node);
            xml::Node xml();

            AstNode *getPiecewise();
            std::string getMappedColumn() { return this->column_id; };

            std::vector<SymbRef *> getAllObservationSymbRefs();
            std::unordered_set<std::string> getAllMappingColumns();
            std::vector<int> getCodesInColumn(std::string col_id);
            SymbRef *getObservationSymbRef(int code, std::string col_id);

            void setupSymbRefs(SymbolGathering &gathering, std::string blkId) override;
            void accept(PharmMLVisitor *visitor);

        private:
            std::string column_id;
            std::unique_ptr<AstNode> piecewise;

            std::vector<std::unique_ptr<SymbRef>> observation_model_symbrefs;
            std::unordered_set<std::string> dvid_columns;
            std::unordered_map<std::string, std::vector<int>> dvid_column_codes;
            std::unordered_map<std::string, std::unordered_map<int, SymbRef *>> symbref_by_dvid_and_code;

            void parsePiecewiseNode(PharmMLReader &reader, xml::Node node);
    };
}

#endif
