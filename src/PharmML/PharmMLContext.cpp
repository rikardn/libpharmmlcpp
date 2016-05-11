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

#include <iostream> // DEBUG_OUTPUT
#include <PharmML/PharmMLContext.h>
#include <PharmML/Model.h>

namespace PharmML
{
    std::string PharmMLContext::buildNamespace(std::string name, std::string namespace_version) {
        return "http://www.pharmml.org/pharmml/" + namespace_version + "/" + name;
    }

    std::string PharmMLContext::getNamespaceVersion() {
        xml::Node root = this->getRoot();
        std::string version = root.getAttribute("writtenVersion").getValue();
        int first_dot_index = version.find_first_of(".");
        int last_dot_index = version.find_last_of(".");
        if (first_dot_index != last_dot_index) {
            version = version.substr(0, last_dot_index);
        }
        return version;
    }

    PharmMLContext::PharmMLContext(const char *filename, Model *model) {
        this->factory.setContext(this);
        this->model = model;
        xmlKeepBlanksDefault(0);        // Otherwise updated XML will not get any indentation
        this->doc = xmlReadFile(filename, NULL, 0);
        if (!this->doc) {
            throw std::runtime_error("File " + std::string(filename) + " not found");
        }
        //this->validateDocument();
        this->xpath_context = xmlXPathNewContext(this->doc);
        std::string version = getNamespaceVersion();
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "x", BAD_CAST buildNamespace("PharmML", version).c_str());
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "math", BAD_CAST buildNamespace("Maths", version).c_str());
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "ct", BAD_CAST buildNamespace("CommonTypes", version).c_str());
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "ds", BAD_CAST buildNamespace("Dataset", version).c_str());
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "mdef", BAD_CAST buildNamespace("ModelDefinition", version).c_str());
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "msteps", BAD_CAST buildNamespace("ModellingSteps", version).c_str());
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "design", BAD_CAST buildNamespace("TrialDesign", version).c_str());
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "po", BAD_CAST "http://www.pharmml.org/probonto/ProbOnto");
    }
    
    // TODO: This and this->symbols/this->symbRefs would be better suited within Consolidator?
    // FIXME: Not needed as symbols already know what they are reffering to via getSymbol()
    Symbol *PharmMLContext::resolveSymbref(SymbRef *symbRef) {
        std::string name = symbRef->toString();
        Symbol *symbol = this->symbols[name];
        return symbol;
    }

    xmlDoc *PharmMLContext::getDocument() {
        return this->doc;
    }

    void PharmMLContext::validateDocument() {
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
            exit(10);
        }

        xmlCatalogCleanup();
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

    void PharmMLContext::write(const char *filename) {
        xmlSaveFormatFileEnc(filename, this->doc, "UTF-8", 1);
    }

    void PharmMLContext::fillSymbRefs() {
        for (SymbRef *symbref : this->symbRefs) {
            std::string symbId = symbref->toString();
            if (this->symbols.count(symbId) != 0) {
                symbref->setSymbol(this->symbols[symbId]);
            } else {
                std::cout << "WARN: context->symbols[" + symbId + "] is undefined; Symbol hasn't reported in so a SymbRef is left dangling!" << std::endl; // DEBUG_OUTPUT
            }
        }
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
