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
#include <vector>
#include <stdio.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/valid.h>
#include <libxml/xmlschemas.h>
#include <libxml/catalog.h>

void usage()
{
    printf("Usage: pharmmltool <options> <command> <cmd-options>\n"
            "Where <command> is one of:\n"
            "    pretty <file>               -- Pretty print a PharmML file\n"
            "    validate <file>             -- Validate a pharmml file against schema\n"
            "    version <file>              -- Print the version of a PharmML file\n"
            "    convert <file>              -- Convert to a later version of PharmML\n"
            "                                   default is to version 0.8.1\n"
            "options:\n"
            "   --version                   Print version information and exit\n"
            "cmd-options:\n"
            "   --schema-path=<path>        Override the default path of the schemas\n"
            "   --target-version=<version>  Version to convert to. Either 0.8.1 or 0.9\n"
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

void version(const char *filename)
{
    xmlDocPtr doc = xmlReadFile(filename, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse %s\n", filename);
        exit(5);
    }

	xmlNode *root = xmlDocGetRootElement(doc);
	if (xmlStrcmp(root->name, BAD_CAST "PharmML") != 0) {
		fprintf(stderr, "File %s is not PharmML\n", filename);
		xmlFreeDoc(doc);
		exit(5);
	}
	xmlChar *version = xmlGetProp(root, BAD_CAST "writtenVersion");
	printf("%s\n", version);
    xmlFreeDoc(doc);
}

void convert(const char *filename)
{
    // Check version of input file
}
 
enum class Command { validate, pretty, version, convert };

int main(int argc, const char *argv[])
{
    std::vector<std::string> arguments(argv + 1, argv + argc);

    if (arguments.empty()) {
        usage();
    }

    // Scan for global options
    for (std::string arg : arguments) {
        if (arg.substr(0, 2) != "--") {
            break;
        }
        if (arg == "--version") {
            std::cout << "pharmmltool 0.3" << std::endl;
            exit(0);
        } else {
            std::cout << "Unknown option: " << arg << std::endl;
            usage();
        }
    }

    // Read command
    std::string command_string = arguments[0];
    arguments.erase(arguments.begin() + 0);

    Command command;
    if (command_string == "validate") {
        command = Command::validate;
    } else if (command_string == "pretty") {
        command = Command::pretty;
    } else if (command_string == "version") {
        command = Command::version;
    } else if (command_string == "convert") {
        command = Command::convert;
    } else {
        usage();
    }

    // Read command options
    const char *schema_path = NULL;
    const char *pharmml_version = "0.8.1";

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--schema-path=", 14) == 0) {
            schema_path = argv[i] + 14;
        }
        if (strncmp(argv[i], "--target-version=", 17) == 0) {
            pharmml_version = argv[i] + 17;
            if (!(strcmp(pharmml_version, "0.8.1") == 0 || strcmp(pharmml_version, "0.9") == 0)) {
                printf("Unknown PharmML version %s. Can only convert to PharmML 0.8.1 or PharmML 0.9\n", pharmml_version);
                exit(5);
            }
        }
    } 

    if (argc < 2) {
        usage();
    }

    LIBXML_TEST_VERSION

    if (command == Command::validate) {
        validate(argv[2], schema_path);
    } else if (command == Command::pretty) {
        pretty(argv[2]);
    } else if (command == Command::version) {
        version(argv[2]);
    } else if (command == Command::convert) {
        convert(argv[2]);
    } else {
        usage();
    }

    return 0;
}
