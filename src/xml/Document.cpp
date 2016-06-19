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

#include <xml/Document.h>

namespace xml {
    Document::~Document() {
        xmlFreeDoc(this->doc);
    }

    Document::Document(std::string filename) {
        xmlKeepBlanksDefault(0);        // Otherwise updated XML will not get any indentation
        this->doc = xmlReadFile(filename.c_str(), NULL, 0);
        if (!this->doc) {
            throw std::runtime_error("File " + filename + " not found");
        }
    }

    xml::Node Document::getRoot() {
        xmlNode *root = xmlDocGetRootElement(this->doc);
        xml::Node node(root);
        return node;
    }

    void Document::write(std::string filename) {
        xmlSaveFormatFileEnc(filename.c_str(), this->doc, "UTF-8", 1);
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
}
