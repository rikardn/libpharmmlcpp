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
            "    compact <file>              -- Remove indentations and newlines\n"
            "    indent <file>               -- Indent a PharmML file\n"
            "    validate <file>             -- Validate a PharmML file against schema\n"
            "    version <file>              -- Print the version of a PharmML file\n"
            "    convert <infile> <outfile   -- Convert to a later version of PharmML\n"
            "                                   default is to version 0.8.1\n"
            "options:\n"
            "   --version                   Print version information and exit\n"
            "cmd-options:\n"
            "   --schema-path=<path>        Override the default path of the schemas\n"
            "   --target-version=<version>  Version to convert to. Either 0.8.1 or 0.9\n"
          );
    exit(0);
}

void error(std::string msg)
{
    std::cerr << msg << std::endl;
    exit(5);
}

void validate(std::string xmlPath, std::string schema_path)
{
    schema_path += "/pharmml-schema/definitions/";
    std::string catalog_file = schema_path + "xmlCatalog.xml";
    std::string schema_file = schema_path + "pharmml.xsd";

    if (xmlLoadCatalog(catalog_file.c_str()) != 0) {
        error("Error when loading catalog");
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

    result = xmlSchemaValidateFile(validCtxt, xmlPath.c_str(), 0);

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

void indent(std::string filename, bool addindent)
{
    xmlDocPtr doc = xmlReadFile(filename.c_str(), NULL, 0);
    if (doc == NULL) {
        error("Failed to parse " + filename);
    }

    xmlSaveFormatFileEnc(filename.c_str(), doc, "UTF-8", addindent ? 1 : 0);

    xmlFreeDoc(doc);
}

void version(std::string filename)
{
    xmlDocPtr doc = xmlReadFile(filename.c_str(), NULL, 0);
    if (doc == NULL) {
        error("Failed to parse " + filename);
    }

	xmlNode *root = xmlDocGetRootElement(doc);
	if (xmlStrcmp(root->name, BAD_CAST "PharmML") != 0) {
		xmlFreeDoc(doc);
        error("File " + filename + " is not PharmML");
	}
	xmlChar *version = xmlGetProp(root, BAD_CAST "writtenVersion");
	printf("%s\n", version);
    xmlFreeDoc(doc);
}

void convert(std::string inputfile, std::string outputfile, std::string version)
{
    // Check version of input file
}
 
enum class Command { validate, indent, compact, version, convert };

int main(int argc, const char *argv[])
{
    xmlLineNumbersDefault(1);
    xmlThrDefIndentTreeOutput(1);
    xmlKeepBlanksDefault(0);
    xmlThrDefTreeIndentString("  ");

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
    } else if (command_string == "indent") {
        command = Command::indent;
    } else if (command_string == "compact") {
        command = Command::compact;
    } else if (command_string == "version") {
        command = Command::version;
    } else if (command_string == "convert") {
        command = Command::convert;
    } else {
        std::cout << "Unknow command: " << command_string << std::endl;
        usage();
    }

    // Read command options
    std::vector<std::string> remaining_arguments;
    std::string target_version = "0.8.1";
    std::string schema_path = "/usr/share/libpharmmlcpp/pharmml_internalRelease_0_8_1";

    for (std::string arg : arguments) {
        if (arg.compare(0, 14, "--schema-path=") == 0) {
            if (command == Command::validate) {
                schema_path = arg.substr(14, std::string::npos);
            } else {
                error("Option --schema-path can only be used with the validate command");
            }
        } else if (arg.compare(0, 17, "--target-version=") == 0) {
            if (command == Command::convert) {
                target_version = arg.substr(17, std::string::npos);
                if (target_version != "0.8.1" or target_version != "0.9") {
                    error("Unknown PharmML version " + target_version + ". Can only convert to PharmML 0.8.1 or PharmML 0.9");
                }
            } else {
                error("Option --target-version can only be used with the convert command");
            }
        } else {
            remaining_arguments.push_back(arg);
        }
    } 

    int numargs = remaining_arguments.size();
    if (!(command == Command::convert && numargs == 2 ||
            command == Command::validate && numargs == 1 ||
            command == Command::indent && numargs == 1 ||
            command == Command::compact && numargs == 1 ||
            command == Command::version && numargs == 1)) {
        error("Too few arguments");
    }

    LIBXML_TEST_VERSION

    if (command == Command::validate) {
        validate(remaining_arguments[0], schema_path);
    } else if (command == Command::indent) {
        indent(remaining_arguments[0], true);
    } else if (command == Command::compact) {
        indent(remaining_arguments[0], false);
    } else if (command == Command::version) {
        version(remaining_arguments[0]);
    } else if (command == Command::convert) {
        convert(remaining_arguments[0], remaining_arguments[1], target_version);
    }

    return 0;
}
