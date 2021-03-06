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
#include "common.h"

#include <stdio.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/valid.h>
#include <libxml/xmlschemas.h>
#include <libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

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
    std::string input_version = version(inputfile, "PharmML");

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
    LIBXML_TEST_VERSION

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

    if (command == Command::validate) {
        auto auxpath = auxfile_path(argv[0]);
        validate(remaining_arguments[0], schema_path, auxpath, "PharmML");
    } else if (command == Command::indent) {
        indent(remaining_arguments[0], true);
    } else if (command == Command::compact) {
        indent(remaining_arguments[0], false);
    } else if (command == Command::version) {
        std::string pharmml_version = version(remaining_arguments[0], "PharmML");
        std::cout << pharmml_version << std::endl;
    } else if (command == Command::convert) {
        auto auxpath = auxfile_path(argv[0]);
        auxpath /= fs::path{"transformations"};
        convert(remaining_arguments[0], remaining_arguments[1], target_version, auxpath);
    }

    return 0;
}
