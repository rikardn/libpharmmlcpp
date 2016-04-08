#ifndef PHARMML_PHARMMLCONTEXT_H_
#define PHARMML_PHARMMLCONTEXT_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpathInternals.h>
#include "xml.h"
#include "AstNodeFactory.h"

namespace PharmML
{
    class PharmMLContext
    {
        xmlDoc *doc;
        xmlXPathContext *xpath_context;

        std::string buildNamespace(std::string name, std::string namespace_version);
        std::string getNamespaceVersion();

        public:
        AstNodeFactory factory;
        xmlDoc *getDocument();
        xml::Node getRoot();
        xml::Node getSingleElement(xml::Node, const char *xpath);
        std::vector<xml::Node> getElements(xml::Node node, const char *xpath);

        PharmMLContext(const char *filename);
        ~PharmMLContext();
    };
}

#endif
