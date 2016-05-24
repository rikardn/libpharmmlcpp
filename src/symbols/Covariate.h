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

#ifndef PHARMML_COVARIATE_H_
#define PHARMML_COVARIATE_H_

#include <xml/xml.h>
#include <PharmML/PharmMLContext.h>
#include <AST/AstNode.h>
#include <symbols/Symbol.h>
#include <visitors/PharmMLVisitor.h>

namespace PharmML
{
    class Covariate : public Symbol
    {
        public:
            Covariate(PharmMLContext *context, xml::Node node);
            Covariate(PharmMLContext *context, xml::Node name_node, xml::Node assign_node);
            void parse(xml::Node node);

            bool isTransformed();
            bool isContinuous();
            std::string getType();
            PharmML::Distribution *getDistribution();
            std::vector<Covariate *> getTransformations();
            AstNode *getAssignment();

            void gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap);
            void accept(PharmMLVisitor *visitor);
            void accept(SymbolVisitor *visitor);

        private:
            PharmML::PharmMLContext *context;
            bool transformed = false;
            bool continuous;
            std::string type;
            PharmML::Distribution *distribution = nullptr;
            std::vector<Covariate *> transformations;
            AstNode *assignment = nullptr;
    };
}

#endif
