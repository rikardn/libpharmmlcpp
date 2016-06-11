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

#include <string>
#include "xml.h"

namespace xml
{
    std::string nsToPrefix(Namespace ns) {
        std::string ns_prefix;

        switch (ns) {
            case Namespace::ct:
                ns_prefix = "ct:";
            case Namespace::ds:
                ns_prefix = "ds:";
            case Namespace::math:
                ns_prefix = "math:";
        }

        return ns_prefix;
    }

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

    Node::Node() {
    }

    Node::Node(xmlNode *node) {
        this->node = node;
    }

    // FIXME: Must add namespace
    Node::Node(std::string name) {
        this->node = xmlNewNode(NULL, BAD_CAST name.c_str());
    }

    Node::Node(std::string name, Namespace ns) {
        this->node = xmlNewNode(NULL, BAD_CAST (nsToPrefix(ns) + name).c_str());
    }

    Node::Node(std::string name, std::string content) {
        this->node = xmlNewNode(NULL, BAD_CAST name.c_str());
        xmlNodeSetContent(this->node, BAD_CAST content.c_str());
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

    void Node::setAttribute(std::string name, std::string value) {
        xmlSetProp(this->node, BAD_CAST name.c_str(), BAD_CAST value.c_str());
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

        return xml::Node(nullptr);
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

    void Node::replaceNode(xml::Node new_node) {
        xmlReplaceNode(this->node, new_node.node);
        xmlFreeNode(this->node);
        this->node = new_node.node;
    }

    xml::Node Node::createChild(std::string name) {
        xmlNode *child = xmlNewChild(this->node, NULL, BAD_CAST name.c_str(), NULL);
        return xml::Node(child);
    }

    void Node::addChild(xml::Node child) {
        xmlAddChild(this->node, child.node);
    }

    void Node::setContent(std::string content) {
        xmlNodeSetContent(this->node, BAD_CAST content.c_str());
    }

    long Node::getLineNo() {
        return xmlGetLineNo(this->node);
    }

    std::string buildNamespace(std::string name, std::string namespace_version) {
        return "http://www.pharmml.org/pharmml/" + namespace_version + "/" + name;
    }

    Node nodeFromString(std::string xml_string) {
        //xmlDocPtr doc = xmlParseMemory(xml_string.c_str(), xml_string.size()); 
        xmlDocPtr doc = xmlReadMemory(xml_string.c_str(), xml_string.size(), NULL, "UTF-8", XML_PARSE_NOERROR);
        xmlNodePtr node = xmlDocGetRootElement(doc);
        xml::Node xml_node(node);
        xml_node.setAttribute("xmlns:ct", xml::buildNamespace("CommonTypes", "0.8.1").c_str());
        xml_node.setAttribute("xmlns:msteps", xml::buildNamespace("ModellingSteps", "0.8.1").c_str());

        return xml_node;
    }

    xml::Node Document::getRoot() {
        xmlNode *root = xmlDocGetRootElement(this->doc);
        xml::Node node(root);
        return node;
    }

    void Document::write(std::string filename) {
        xmlSaveFormatFileEnc(filename.c_str(), this->doc, "UTF-8", 1);
    }

    void Document::read(std::string filename) {
        xmlKeepBlanksDefault(0);        // Otherwise updated XML will not get any indentation
        this->doc = xmlReadFile(filename.c_str(), NULL, 0);
        if (!this->doc) {
            throw std::runtime_error("File " + filename + " not found");
        }
    }

    void Document::validate() {
        if (xmlLoadCatalog("pharmml_internalRelease_0_8_1/pharmml-schema/definitions/xmlCatalog.xml") != 0) {
            return;
        }
        int result = 42;
        xmlSchemaParserCtxtPtr parserCtxt = NULL;
        xmlSchemaPtr schema = NULL;
        xmlSchemaValidCtxtPtr validCtxt = NULL;

        parserCtxt = xmlSchemaNewParserCtxt("pharmml_internalRelease_0_8_1/pharmml-schema/definitions/pharmml.xsd");

        if (parserCtxt == NULL) {
            goto leave;
        }

        schema = xmlSchemaParse(parserCtxt);

        if (schema == NULL) {
            goto leave;
        }

        validCtxt = xmlSchemaNewValidCtxt(schema);

        if (!validCtxt) {
            goto leave;
        }

        result = xmlSchemaValidateDoc(validCtxt, this->doc);

leave:

        if (parserCtxt) {
            xmlSchemaFreeParserCtxt(parserCtxt);
        }

        if (schema) {
            xmlSchemaFree(schema);
        }

        if (validCtxt) {
            xmlSchemaFreeValidCtxt(validCtxt);
        }
        if (result != 0) {
            printf("\n");
            printf("Validation successful: %s (result: %d)\n", (result == 0) ? "YES" : "NO", result);
            exit(10);           // Gah! FIXME!
        }

        xmlCatalogCleanup();
    }

    Document::~Document() {
        if (this->doc) {
            xmlFreeDoc(this->doc);
        }
    }

}
