#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include "xml.h"
#include "PharmMLContext.h"

namespace PharmML
{
    PharmMLContext::PharmMLContext(const char *filename) {
        this->doc = xmlReadFile(filename, NULL, 0);
        this->xpath_context = xmlXPathNewContext(this->doc);
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "x", BAD_CAST "http://www.pharmml.org/pharmml/0.8/PharmML");
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "math", BAD_CAST "http://www.pharmml.org/pharmml/0.8/Maths");
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "ct", BAD_CAST "http://www.pharmml.org/pharmml/0.8/CommonTypes");
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "ds", BAD_CAST "http://www.pharmml.org/pharmml/0.8/Dataset");
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "mdef", BAD_CAST "http://www.pharmml.org/pharmml/0.8/ModelDefinition");
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "mstep", BAD_CAST "http://www.pharmml.org/pharmml/0.8/ModellingSteps");
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "design", BAD_CAST "http://www.pharmml.org/pharmml/0.8/TrialDesign");
    }

    xmlDoc *PharmMLContext::getDocument() {
        return this->doc;
    }

    xml::Node PharmMLContext::getRoot() {
        xmlNode *root = xmlDocGetRootElement(this->doc);
        xml::Node node(root);
        return node;
    }

    xml::Node PharmMLContext::getSingleElement(xml::Node node, const char *xpath) {
        return node.getSingleElement(this->xpath_context, xpath);
    }

    std::vector<xml::Node> PharmMLContext::getElements(xml::Node node, const char *xpath) {
        return node.getElements(this->xpath_context, xpath);
    }

    PharmMLContext::~PharmMLContext() {
        if (this->xpath_context) {
            xmlXPathFreeContext(xpath_context);
        }
        if (this->doc) {
            xmlFreeDoc(this->doc);
        }
    }
}
