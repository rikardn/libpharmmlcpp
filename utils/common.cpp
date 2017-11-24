/* pharmmlutils - A set of tools for validation, conversion etc of PharmML and SO files
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

#include "common.h"
#include "config.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <libxml/parser.h>
#include <libxml/valid.h>
#include <libxml/xmlschemas.h>
#include <libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

void error(std::string msg)
{
    std::cerr << msg << std::endl;
    exit(5);
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
#ifdef WINDOWS
        path_env = ".;" + path_env;     // Windows always have the current directory first in the path
        split_char = ';';
        if (not ends_with(command, std::string{".exe"})) {
            command += ".exe";
        }
#else
        split_char = ':';
#endif
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
#ifdef WINDOWS
    search_path = fs::path{"transformations\\0_6to0_8_1.xslt"};
#else
    search_path = fs::path{"transformations"};
#endif

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

void indent(std::string filename, bool addindent)
{
    xmlKeepBlanksDefault(0);
    xmlDocPtr doc = xmlReadFile(filename.c_str(), NULL, 0);
    if (doc == NULL) {
        error("Failed to parse " + filename);
    }

    xmlSaveFormatFileEnc(filename.c_str(), doc, "UTF-8", addindent ? 1 : 0);

    xmlFreeDoc(doc);
}

// type is either PharmML or SO
std::string version(std::string filename, std::string type)
{
    xmlDocPtr doc = xmlReadFile(filename.c_str(), NULL, 0);
    if (doc == NULL) {
        error("Failed to parse " + filename);
    }

	xmlNode *root = xmlDocGetRootElement(doc);
	if (xmlStrcmp(root->name, BAD_CAST type.c_str()) != 0) {
		xmlFreeDoc(doc);
        error("File " + filename + " is not " + type);
	}
	xmlChar *version = xmlGetProp(root, BAD_CAST "writtenVersion");
    std::string string_version = std::string((char *) version);
    xmlFreeDoc(doc);

    return(string_version);
}

void validate(std::string xmlPath, std::string schema_path, fs::path auxpath, std::string type)
{
    std::string file_version;

    fs::path catalog_file;
    fs::path schema_file;

    if (type == "PharmML") {
        if (schema_path.empty()) {
            fs::path path = auxpath / fs::path{"schema"};
            std::string file_version = version(xmlPath, type);
            if (file_version != "0.8.1" && file_version != "0.9") {
                error("Validate only support PharmML versions 0.8.1 and 0.9");
            }
            path /= fs::path{file_version} / fs::path{"pharmml-schema/definitions"};
            catalog_file = path / fs::path{"xmlCatalog.xml"};
            schema_file = path / fs::path{"pharmml.xsd"};
        } else {
            fs::path path = fs::path{schema_path} / fs::path{"pharmml-schema/definitions"};
            catalog_file = path / fs::path{"xmlCatalog.xml"};
            schema_file = path / fs::path{"pharmml.xsd"};
        }
    } else {
        if (schema_path.empty()) {
            fs::path path = auxpath / fs::path{"schema"};
            std::string file_version = version(xmlPath, type);
            if (file_version != "0.3.1") {
                error("Validate only supports SO version 0.3.1");
            }
            catalog_file = path / fs::path{"0.8.1/pharmml-schema/definitions/xmlCatalog.xml"};
            schema_file = path / fs::path{"SO_" + file_version} / fs::path{"standardisedOutput.xsd"};
        } else {
            catalog_file = fs::path{schema_path} / fs::path{"xmlCatalog.xml"};
            schema_file = fs::path{schema_path} / fs::path{"standardisedOutput.xsd"};
        }
    }

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
