/* sotool - A command line tool for validation, conversion etc of SO files
 * Copyright (C) 2017 Rikard Nordgren
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
    printf("Usage: sotool <options> <command> <cmd-options>\n"
            "Where <command> is one of:\n"
            "    compact <file>              -- Remove indentations and newlines\n"
            "    indent <file>               -- Indent an SO-file\n"
            "    validate <file>             -- Validate an SO-file against schema\n"
            "    version <file>              -- Print the version of an SO file\n"
            "    merge <dest> <source files> -- Merge SOBlocks from multiple files into one file\n"
            "    compress <file>             -- Compress the SO inplace using gzip compression\n"
            "    uncompress <file>           -- Uncompress the SO inplace\n"
            "\n"
            "options:\n"
            "   --version                   Print version information and exit\n"
            "   --help                      View this usage text\n"
            "\n"
            "cmd-options:\n"
            "   --schema-path=<path>        Override the default path of the schemas\n"
          );
    exit(0);
}

void compress(std::string filename, int level)
{
    xmlDocPtr doc = xmlReadFile(filename.c_str(), NULL, 5);
    if (doc == NULL) {
        error("Failed to parse " + filename);
    }

    xmlSetDocCompressMode(doc, level);
    xmlSaveFile(filename.c_str(), doc);

    xmlFreeDoc(doc);
}

void merge(const std::string dest, std::vector<std::string> source)
{
    if (source.empty()) {
        std::cerr << "No source files specified" << std::endl;
        exit(5);
    }

    xmlDocPtr doc = xmlReadFile(source[0].c_str(), NULL, 0);
    if (doc == NULL) {
        std::cerr << "Failed to parse " + source[0] << std::endl;
        exit(5);
    }
    xmlNodePtr root = xmlDocGetRootElement(doc);

    std::cout << "Merging: " + source[0] << std::endl;
    xmlDocPtr source_doc;
    xmlNodePtr source_root;

    for (int i = 1; i < source.size(); i++) {
        source_doc = xmlReadFile(source[i].c_str(), NULL, 0);
        if (source_doc == NULL) {
            std::cerr << "Failed to parse " + source[i] << std::endl;
            xmlFreeDoc(doc);
            exit(5);
        }
        source_root = xmlDocGetRootElement(source_doc);
        xmlNodePtr child = source_root->children;
        while (child) {
            if (strcmp((char *) child->name, "SOBlock") == 0) {
                xmlNodePtr new_node = xmlCopyNode(child, 1);
                xmlAddChild(root, new_node);
            }
            child = child->next;
        }
        std::cout << "Merging: " + source[i] << std::endl;
        xmlFreeDoc(source_doc);
    }

    xmlSaveFormatFileEnc(dest.c_str(), doc, "UTF-8", 0);
    xmlFreeDoc(doc);
}

enum class Command { validate, indent, compact, version, merge, compress, uncompress };

int main(int argc, const char *argv[])
{
    LIBXML_TEST_VERSION

    xmlLineNumbersDefault(1);
    xmlThrDefIndentTreeOutput(1);
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
            std::cout << "sotool " VERSION << std::endl;
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
    } else if (command_string == "merge") {
        command = Command::merge;
    } else if (command_string == "compress") {
        command = Command::compress;
    } else if (command_string == "uncompress") {
        command = Command::uncompress;
    } else {
        std::cout << "Unknow command: " << command_string << std::endl;
        usage();
    }

    // Read command options
    std::vector<std::string> remaining_arguments;
    std::string schema_path;

    for (std::string arg : arguments) {
        if (arg.compare(0, 14, "--schema-path=") == 0) {
            if (command == Command::validate) {
                schema_path = arg.substr(14, std::string::npos);
            } else {
                error("Option --schema-path can only be used with the validate command");
            }
        } else {
            remaining_arguments.push_back(arg);
        }
    } 

    int numargs = remaining_arguments.size();
    if (!(command == Command::validate && numargs == 1 ||
            command == Command::indent && numargs == 1 ||
            command == Command::compress && numargs == 1 ||
            command == Command::compact && numargs == 1 ||
            command == Command::uncompress && numargs == 1 ||
            command == Command::version && numargs == 1)) {
        error("Wrong number of arguments");
    }

    if (command == Command::validate) {
        auto auxpath = auxfile_path(argv[0]);
        validate(remaining_arguments[0], schema_path, auxpath, "SO");
    } else if (command == Command::indent) {
        indent(remaining_arguments[0], true);
    } else if (command == Command::compact) {
        indent(remaining_arguments[0], false);
    } else if (command == Command::version) {
        std::string so_version = version(remaining_arguments[0], "SO");
        std::cout << so_version << std::endl;
    } else if (command == Command::merge) {
        std::string destination = remaining_arguments[0];
        remaining_arguments.erase(remaining_arguments.begin());
        merge(destination, remaining_arguments);
    } else if (command == Command::compress) {
        compress(remaining_arguments[0], 9);
    } else if (command == Command::uncompress) {
        compress(remaining_arguments[0], 0);
    }

    return 0;
}
