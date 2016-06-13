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

#include <unordered_map>
#include <xml/xml.h>
#include <AST/AstNodeFactory.h>
#include <AST/symbols.h>
#include <helpers/Logger.h>

namespace pharmmlcpp
{
    class Model;
    class PharmMLContext
    {
        public:
            xml::Document doc;
            Logger logger;
            AstNodeFactory factory;
            Model *model;
            void validateDocument();
            xml::Node getSingleElement(xml::Node, const char *xpath);
            std::vector<xml::Node> getElements(xml::Node node, const char *xpath);

            PharmMLContext(std::string filename, Model *model);
            ~PharmMLContext();

        private:
            xmlXPathContext *xpath_context;

            std::string buildNamespace(std::string name, std::string namespace_version);
            std::string getNamespaceVersion();
    };
}

#endif
