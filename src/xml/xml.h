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

#ifndef PHARMMLCPP_XML_H_
#define PHARMMLCPP_XML_H_

#include <stdexcept>
#include <vector>
#include <string>
#include <stdexcept>
#include <libxml/tree.h>
#include <libxml/xpathInternals.h>
#include <libxml/parser.h>
#include <libxml/valid.h>
#include <libxml/xmlschemas.h>
#include <libxml/catalog.h>
#include <libxml/xpath.h>
#include <xml/Document.h>
#include <xml/XPathContext.h>

namespace xml
{
    enum class Namespace { ct, ds, math };
    std::string nsToPrefix(Namespace ns);
    std::string buildNamespace(std::string name, std::string namespace_version);

    class XPathContext;     // FIXME: Why is this needed?

    class Attribute
    {
        public:
            bool exists();
            void setExists(bool exists);
            std::string getValue();
            std::string getName();
            void setValue(std::string value);
            void setName(std::string name);

        private:
            bool does_exist;
            std::string name;
            std::string value;
    };

    class Node
    {
        friend class Document;

        public:
            Node();
            Node(xmlNode *node);
            Node(std::string name);
            Node(std::string name, Namespace ns);
            Node(std::string name, std::string content);
            bool exists();
            Attribute getAttribute(const char *name);
            void setAttribute(std::string name, std::string value);
            std::string getName();
            xml::Node getChild();
            xml::Node getLastChild();
            std::vector<xml::Node> getChildren();
            std::string getText();
            Node getSingleElement(XPathContext &xpath_context, const char *xpath);
            std::vector<xml::Node> getElements(XPathContext &xpath_context, const char *xpath);
            void replaceNode(xml::Node new_node);
            xml::Node createChild(std::string name);
            void addChild(xml::Node child);
            void setContent(std::string content);
            long getLineNo();   // Get the line number in the xml file for a node
        
        private:
            xmlNode *node;
    };

    // This is expected to be used for testing
    Node nodeFromString(std::string xml_string);
    bool validateNCName(std::string ncname);
}

#endif
