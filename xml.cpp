#include <string>
#include "xml.h"

namespace xml
{
    bool Attribute::exists() {
        return does_exist;
    }

    void Attribute::setExists(bool exists) {
        this->does_exist = exists;
    }

    std::string Attribute::getValue() {
        return this->value;
    }

    std::string Attribute::getName() {
        return this->name;
    }

    void Attribute::setValue(std::string value) {
        this->value = value;
    }

    void Attribute::setName(std::string name) {
        this->name = name;
    }

    Node::Node(xmlNode *node) {
        this->node = node;
    }

    bool Node::exists() {
        return (bool) this->node;
    }

    Attribute Node::getAttribute(const char *name) {
        Attribute attr;
        char *value = (char *) xmlGetProp(this->node, BAD_CAST name);

        attr.setName(std::string(name));
        if (value) {
            std::string value_string = std::string(value);
            attr.setValue(value_string);
            attr.setExists(true);
        } else {
            attr.setExists(false);
        }

        xmlFree(value);
        return attr;
    }

    std::string Node::getName() {
        return std::string((char *) this->node->name);
    }

    xml::Node Node::getChild() {
        xmlNode *xml_node = this->node->children;

        while (xml_node) {
            if (xml_node->type == XML_ELEMENT_NODE) {
                return xml::Node(xml_node);
            }
            xml_node = xml_node->next;
        }
    }

    xml::Node Node::getLastChild() {
        xmlNode *xml_node = this->node->children;
        xmlNode *result_node = nullptr;

        while (xml_node) {
            if (xml_node->type == XML_ELEMENT_NODE) {
                result_node = xml_node;
            }
            xml_node = xml_node->next;
        }

        return xml::Node(result_node);
    }

    std::vector<xml::Node> Node::getChildren() {
        std::vector<xml::Node> result;
        xmlNode *xml_node = this->node->children;
        while (xml_node) {
            if (xml_node->type == XML_ELEMENT_NODE) {
                result.push_back(xml::Node(xml_node));
            }
            xml_node = xml_node->next;
        }
        return result;
    }

    std::string Node::getText() {
        char *content = (char *) xmlNodeGetContent(this->node);
        std::string str(content);
        xmlFree(content);
        return str;
    }

    Node Node::getSingleElement(xmlXPathContext *xpath_context, const char *xpath) {
        xmlXPathObject *obj = xmlXPathNodeEval(this->node, BAD_CAST xpath, xpath_context);
        xmlNode *result = NULL;

        if (obj && obj->nodesetval) {
            int numnodes = obj->nodesetval->nodeNr;
            xmlNode **array = obj->nodesetval->nodeTab;

            for (int i = 0; i < numnodes; i++) {
                if (array[i]->type == XML_ELEMENT_NODE) {
                    result = array[i];
                    break;
                }
            }
        }

        xmlXPathFreeObject(obj);
        return Node(result);
    }

    std::vector<xml::Node> Node::getElements(xmlXPathContext *xpath_context, const char *xpath) {
        xmlXPathObject *obj = xmlXPathNodeEval(this->node, BAD_CAST xpath, xpath_context);
        std::vector<xml::Node> results;

        if (obj) {
            int numnodes = obj->nodesetval->nodeNr;
            xmlNode **array = obj->nodesetval->nodeTab;

            for (int i = 0; i < numnodes; i++) {
                if (array[i]->type == XML_ELEMENT_NODE) {
                    results.push_back(xml::Node(array[i]));
                }
            }
        }

        xmlXPathFreeObject(obj);
        return results;
    }
}
