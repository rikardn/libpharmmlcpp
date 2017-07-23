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

#ifndef PHARMMLCPP_PHARMMLREADER_H_
#define PHARMMLCPP_PHARMMLREADER_H_

#include <unordered_map>
#include <xml/xml.h>
#include <AST/symbols.h>
#include <helpers/Logger.h>
#include <memory>

namespace pharmmlcpp
{
    class PharmML;
    class PharmMLReader
    {
        public:
            static PharmMLReader createTestReader(std::string xml);
            xml::Node getRoot();
            std::shared_ptr<xml::Document> doc;
            Logger logger;
            xml::Node getSingleElement(xml::Node, const char *xpath);
            std::vector<xml::Node> getElements(xml::Node node, const char *xpath);

            PharmMLReader(std::string filename);
            PharmMLReader(std::shared_ptr<xml::Document> doc);

    //    private:
            std::unique_ptr<xml::XPathContext> xpathContext;
    };
}

#endif
