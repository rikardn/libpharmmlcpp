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

#ifndef PHARMML_PHARMMLCONTEXT_H_
#define PHARMML_PHARMMLCONTEXT_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpathInternals.h>
#include <libxml/valid.h>
#include <libxml/xmlschemas.h>
#include <libxml/catalog.h>
#include <libxml/xpath.h>
#include <unordered_map>
#include <xml/xml.h>
#include <AST/AstNodeFactory.h>
#include <AST/symbols.h>

namespace PharmML
{
    class Model;
    class PharmMLContext
    {
        Model *model;
        xmlDoc *doc;
        xmlXPathContext *xpath_context;

        std::string buildNamespace(std::string name, std::string namespace_version);
        std::string getNamespaceVersion();

        public:
        std::unordered_map<std::string, Symbol *> symbols;
        std::vector<SymbRef *> symbRefs;
        Symbol *resolveSymbref(SymbRef *symbRef);
        AstNodeFactory factory;
        xmlDoc *getDocument();
        void validateDocument();
        xml::Node getRoot();
        xml::Node getSingleElement(xml::Node, const char *xpath);
        std::vector<xml::Node> getElements(xml::Node node, const char *xpath);

        PharmMLContext(const char *filename, Model *model);
        ~PharmMLContext();
        void write(const char *filename); 
        void fillSymbRefs();
    };
}

#endif
