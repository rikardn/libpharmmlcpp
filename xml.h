#ifndef PHARMML_XML_H_
#define PHARMML_XML_H_

#include <vector>
#include <string>
#include <libxml/tree.h>
#include <libxml/xpathInternals.h>
#include <libxml/parser.h>

namespace xml
{
    class Attribute
    {
        bool does_exist;
        std::string name;
        std::string value;

        public:
        bool exists();
        void setExists(bool exists);
        std::string getValue();
        std::string getName();
        void setValue(std::string value);
        void setName(std::string name);
    };

    class Node
    {
        xmlNode *node;

        public:
        Node(xmlNode *node);
        bool exists();
        Attribute getAttribute(const char *name);
        std::string getName();
        xml::Node getChild();
        xml::Node getLastChild();
        std::vector<xml::Node> getChildren();
        std::string getText();
        Node getSingleElement(xmlXPathContext *xpath_context, const char *xpath);
        std::vector<xml::Node> getElements(xmlXPathContext *xpath_context, const char *xpath);
        xml::Node createChild();
    };
}

#endif
