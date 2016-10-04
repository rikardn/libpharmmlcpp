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

#ifndef PHARMMLCPP_PHARMMLCONTEXT_H_
#define PHARMMLCPP_PHARMMLCONTEXT_H_

#include <unordered_map>
#include <xml/xml.h>
#include <AST/symbols.h>
#include <helpers/Logger.h>
#include <PharmML/PharmMLReader.h>

namespace pharmmlcpp
{
    class PharmML;
    class PharmMLContext
    {
        public:
            xml::Document doc;
            Logger logger;
            void validateDocument();
            xml::Node getSingleElement(xml::Node, const char *xpath);
            std::vector<xml::Node> getElements(xml::Node node, const char *xpath);

            PharmMLContext(PharmMLReader &reader);
            PharmMLContext(std::string filename);

        private:
            xml::XPathContext xpathContext;
    };
}

#endif
