/* pharmmltool - A command line tool for validation, conversion etc of PharmML files
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
#include <algorithm>
#include <cstdlib>

#include "config.h"

// Hack for lack of filesystem support in MinGW
#ifndef WINDOWS
#include <experimental/filesystem>
#else
#include "filesystem.h"
#endif

#include <stdio.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/valid.h>
#include <libxml/xmlschemas.h>
#include <libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

namespace fs = std::experimental::filesystem;

std::string version(std::string filename);

void usage()
{
    printf("Usage: pharmmltool <options> <command> <cmd-options>\n"
            "Where <command> is one of:\n"
            "    compact <file>              -- Remove indentations and newlines\n"
            "    indent <file>               -- Indent a PharmML file\n"
            "    validate <file>             -- Validate a PharmML file against schema\n"
            "    version <file>              -- Print the version of a PharmML file\n"
            "    convert <infile> <outfile>  -- Convert to a later version of PharmML\n"
            "                                   default is to version 0.8.1\n"
            "options:\n"
            "   --version                   Print version information and exit\n"
            "   --help                      View this usage text\n"
            "\n"
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

void validate(std::string xmlPath, std::string schema_path, fs::path auxpath)
{
    fs::path path;
    if (schema_path.empty()) {
        path = auxpath / fs::path{"schema"};
        std::string pharmml_version = version(xmlPath);
        if (pharmml_version != "0.8.1" && pharmml_version != "0.9") {
            error("Validate only support PharmML versions 0.8.1 and 0.9");
        }
        path /= fs::path{pharmml_version};
    } else {
        path = fs::path{schema_path};
    }

    path /= fs::path{"pharmml-schema/definitions"};
    fs::path catalog_file = path / fs::path{"xmlCatalog.xml"};
    fs::path schema_file = path / fs::path{"pharmml.xsd"};

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

std::string version(std::string filename)
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
    std::string string_version = std::string((char *) version);
    xmlFreeDoc(doc);

    return(string_version);
}

std::vector<std::string> split(std::string const &in, char sep)
{
    std::string::size_type b = 0;
    std::vector<std::string> result;

    while ((b = in.find_first_not_of(sep, b)) != std::string::npos) {
        auto e = in.find_first_of(sep, b);
        result.push_back(in.substr(b, e-b));
        b = e;
    }
    return result;
}

bool ends_with(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() &&
        str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

fs::path which(std::string command)
{
    std::string path_env = std::getenv("PATH");

    char split_char;
    if (WINDOWS) {
        path_env = ".;" + path_env;     // Windows always have the current directory first in the path
        split_char = ';';
        if (not ends_with(command, std::string{".exe"})) {
            command += ".exe";
        }
    } else {
        split_char = ':';
    }
    std::vector<std::string> path_dirs = split(path_env, split_char);
    fs::path command_path{command};
    for (std::string path_string : path_dirs) {
        fs::path path(path_string);
        if (fs::exists(path / command_path)) {
            return path;
        }
    }
}

fs::path auxfile_path(std::string command)
{
    // Check where data files are available
    // 1. If command has path use that
    // 2. If no path in command use PATH
    // 3. Look in installation path

    fs::path command_path(command);

    fs::path aux_path;

    if (command_path.filename() == command_path) {
        aux_path = which(command_path);
    } else {
        aux_path = command_path.parent_path();
    }

    fs::path search_path;
    if (WINDOWS) {
        search_path = fs::path{"transformations\\0_6to0_8_1.xslt"};
    } else {
        search_path = fs::path{"transformations"};
    }

    if (fs::exists(aux_path / search_path)) {
        return aux_path;
    } else {
        aux_path = fs::path{DATA_PATH};
        if (fs::exists(aux_path / search_path)) {
            return aux_path;
        } else {
            error("Error: Cannot find auxiliary data files (XML schemas and style sheets)");
        }
    }
}

void transform(fs::path xslt, std::string inputfile, std::string outputfile)
{
    auto stylesheet = xsltParseStylesheetFile(BAD_CAST xslt.c_str());
    auto doc = xmlParseFile(inputfile.c_str());

    const char *params = NULL;
    auto result = xsltApplyStylesheet(stylesheet, doc, &params);
    xsltSaveResultToFilename(outputfile.c_str(), result, stylesheet, 0);

    xmlFreeDoc(doc);
    xmlFreeDoc(result);
    xsltFreeStylesheet(stylesheet);
}

void convert(std::string inputfile, std::string outputfile, std::string output_version, fs::path transformations_path)
{
    std::string input_version = version(inputfile);

    std::string original_input_version = input_version;
    if (input_version == "0.6.1") {
        input_version = "0.6";
    }

    if (input_version != "0.6" && input_version != "0.8.1") {
        error("pharmmltool error: found PharmML version " + input_version +
            "\npharmmltool can only convert from version 0.6 and 0.8.1");
    }

    std::cout << "Converting PharmML" << std::endl;
    std::cout << "    from version " + original_input_version << std::endl;
    std::cout << "      to version " + output_version << std::endl;

    std::string xslt_name;
    if (input_version == "0.6" && output_version == "0.8.1") {
        transform(transformations_path / fs::path{"0_6to0_8_1.xslt"}, inputfile, outputfile); 
    } else if (input_version == "0.6" && output_version == "0.9") {
        transform(transformations_path / fs::path{"0_6to0_8_1.xslt"}, inputfile, outputfile); 
        transform(transformations_path / fs::path{"0_8_1to0_9.xslt"}, outputfile, outputfile); 
    } else if (input_version == "0.8.1" && output_version == "0.9") {
        transform(transformations_path / fs::path{"0_8_1to0_9.xslt"}, inputfile, outputfile);
    }
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
            std::cout << "pharmmltool " VERSION << std::endl;
            exit(0);
        } else if (arg == "--help") {
            usage();
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
    std::string target_version = "0.9";
    std::string schema_path;

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
                if (target_version != "0.8.1" && target_version != "0.9") {
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
        error("Wrong number of arguments");
    }

    LIBXML_TEST_VERSION

    if (command == Command::validate) {
        auto auxpath = auxfile_path(argv[0]);
        validate(remaining_arguments[0], schema_path, auxpath);
    } else if (command == Command::indent) {
        indent(remaining_arguments[0], true);
    } else if (command == Command::compact) {
        indent(remaining_arguments[0], false);
    } else if (command == Command::version) {
        std::string pharmml_version = version(remaining_arguments[0]);
        std::cout << pharmml_version << std::endl;
    } else if (command == Command::convert) {
        auto auxpath = auxfile_path(argv[0]);
        auxpath /= fs::path{"transformations"};
        convert(remaining_arguments[0], remaining_arguments[1], target_version, auxpath);
    }

    return 0;
}
