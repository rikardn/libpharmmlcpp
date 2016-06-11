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

#ifndef PHARMML_STRINGTOOLS_H_
#define PHARMML_STRINGTOOLS_H_

#include <sstream>
#include <string>
#include <algorithm>
#include "limits.h"

namespace StringTools
{
    bool isInt(std::string str, int &result);
    bool iequals(std::string str1, std::string str2);
    std::string trimLeadingWhitespace(std::string str);
    std::string trimTrailingWhitespace(std::string str);
    std::string mergeLines(std::string str);
    std::string removeWhitespace(std::string str);
}

#endif
