/* pharmmltool - A command line tool for mainly validation of PharmML files 
 * Copyright (C) 2015 Rikard Nordgren
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * 
 * his library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <stdio.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/valid.h>
#include <libxml/xmlschemas.h>
#include <libxml/catalog.h>

void usage()
{
    printf("Usage: pharmmltool <command> <cmd-options>\n"
            "Where <command> is one of:\n"
            "    pretty <file>               -- Pretty print a pharmml file\n"
            "    validate <file>             -- Validate a pharmml file against schema\n"
            "options:\n"
            "   --schema-path=<path>        Override the default path of the schemas\n"
            "   --version                   Print version information and exit\n"
          );
    exit(0);
}

void validate(const char *xmlPath, const char *schema_path)
{
    std::string path;
    if (schema_path) {
        path = schema_path;
    } else {
        path = "/usr/share/libpharmmlcpp/pharmml_internalRelease_0_8_1";
    }

    path += "/pharmml-schema/definitions/";
    std::string catalog_file = path + "xmlCatalog.xml";
    std::string schema_file = path + "pharmml.xsd";

    if (xmlLoadCatalog(catalog_file.c_str()) != 0) {
        printf("Error when loading catalog\n");
        exit(5);
    }

    int result = 42;
    xmlSchemaParserCtxtPtr parserCtxt = NULL;
    xmlSchemaPtr schema = NULL;
    xmlSchemaValidCtxtPtr validCtxt = NULL;

    parserCtxt = xmlSchemaNewParserCtxt(schema_file.c_str());

    if (parserCtxt == NULL) {
        fprintf(stderr, "Could not create XSD schema parsing context.\n");
        goto leave;
    }

    schema = xmlSchemaParse(parserCtxt);

    if (schema == NULL) {
        fprintf(stderr, "Could not parse XSD schema.\n");
        goto leave;
    }

    validCtxt = xmlSchemaNewValidCtxt(schema);

    if (!validCtxt) {
        fprintf(stderr, "Could not create XSD schema validation context.\n");
        goto leave;
    }

    result = xmlSchemaValidateFile(validCtxt, xmlPath, 0);

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
    printf("\n");
    printf("Validation successful: %s (result: %d)\n", (result == 0) ? "YES" : "NO", result);

    xmlCatalogCleanup();
}

void pretty(const char *filename)
{
    xmlDocPtr doc = xmlReadFile(filename, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse %s\n", filename);
        exit(5);
    }

    xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);

    xmlFreeDoc(doc);
}

int main(int argc, const char *argv[])
{
    const char *command = argv[1];

    const char *schema_path = NULL;

    // Scan for options
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--version") == 0) {
            printf("pharmmltool 0.2\n");
            exit(0);
        }
        if (strncmp(argv[i], "--schema-path=", 14) == 0) {
            schema_path = argv[i] + 14;
        }
    } 

    if (argc < 2) {
        usage();
    }

    LIBXML_TEST_VERSION

    if (strcmp(command, "validate") == 0) {
        validate(argv[2], schema_path);
    } else if (strcmp(command, "pretty") == 0) {
        pretty(argv[2]);
    } else {
        usage();
    }

    return 0;
}
