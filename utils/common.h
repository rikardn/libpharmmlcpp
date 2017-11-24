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

// Hack for lack of filesystem support in MinGW

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

#ifndef WINDOWS
#include <experimental/filesystem>
#else
#include "filesystem.h"
#endif

namespace fs = std::experimental::filesystem;

void error(std::string msg);
std::vector<std::string> split(std::string const &in, char sep);
bool ends_with(const std::string &str, const std::string &suffix);
fs::path which(std::string command);
fs::path auxfile_path(std::string command);
void indent(std::string filename, bool addindent);
std::string version(std::string filename, std::string type);
void validate(std::string xmlPath, std::string schema_path, fs::path auxpath, std::string type);
