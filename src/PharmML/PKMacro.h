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

#ifndef PHARMMLCPP_PKMACRO_H_
#define PHARMMLCPP_PKMACRO_H_

#include <vector>
#include <PharmML/PharmMLReader.h>
#include <PharmML/PharmMLSection.h>
#include <visitors/PharmMLVisitor.h>
#include <visitors/AstAnalyzer.h>
#include <symbols/MacroGathering.h>
#include <symbols/SymbolSet.h>
#include <symbols/Symbol.h>
#include <AST/AstNode.h>

namespace pharmmlcpp
{
    typedef std::pair<std::string, std::shared_ptr<AstNode>> MacroValue;
    enum class MacroType {Compartment, Peripheral, Effect, Depot, IV, Absorption, Oral, Elimination, Transfer};
    class PKMacro : public PharmMLSection, public Referer
    {
        public:
            PKMacro(PharmMLReader &reader, xml::Node node);

            std::string getType();
            bool hasAttribute(std::string attribute);
            std::vector<MacroValue> getValues();
            std::shared_ptr<AstNode> getAssignment(std::string attribute);

            void setupSymbRefs(SymbolGathering &gathering, std::string blkId) override;
            void accept(PharmMLVisitor *visitor);

            // POST PARSE/CONSOLIDATION
            std::string generateName();
            void setName(std::string name);
            std::string getName();
            bool isCompartment();
            bool isAdministration();
            bool isMassTransfer();
            MacroType getSubType();

            int getCmtNum();
            int getAdmNum();

            int getSourceNum();
            SymbRef *getSourceSymbRef();

            int getTargetNum();
            SymbRef *getTargetSymbRef();

        private:
            void parse(PharmMLReader &reader, xml::Node node);

            std::string type;
            std::vector<MacroValue> values;

            // POST PARSE/CONSOLIDATION
            void postParse();
            std::string name;
            bool is_comp = false;
            bool is_abs = false;
            bool is_trans = false;
            MacroType sub_type;

            int cmt_num = 0;
            int adm_num = 0;

            int source_cmt_num = 0;
            std::unique_ptr<SymbRef> source_symbref;

            int target_cmt_num = 0;
            std::unique_ptr<SymbRef> target_symbref;
    };

    class PKMacros : public PharmMLSection
    {
        public:
            PKMacros(PharmMLReader &reader, xml::Node node);

            std::vector<PKMacro *> getMacros();

            void setupRefererSymbRefs(SymbolGathering &gathering, std::string blkId);

            // POST PARSE/CONSOLIDATION
            void postParse(); // FIXME: See comment in Model.cpp!!!
            std::vector<PKMacro *> getAdministrations();
            std::vector<PKMacro *> getCompartments();
            std::vector<PKMacro *> getTransfers();
            PKMacro *getAdministration(int adm_num);
            PKMacro *getCompartment(int cmt_num);
            PKMacro *getElimination(int cmt_num);
            PKMacro *getAbsorption(int cmt_num);

        private:
            std::vector<PKMacro *> macros;

            void parse(PharmMLReader &reader, xml::Node node);

            // POST PARSE/CONSOLIDATION
            //~ void postParse(); // FIXME: See comment in Model.cpp!!!
            void validate();
    };
}

#endif
