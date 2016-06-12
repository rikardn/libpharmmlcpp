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
 * Lesser General Public License for more detailstr.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "StringTools.h"

namespace StringTools
{
    // Try to convert a string into an int (result) and report sucess/failure
    bool isInt(std::string str, int &result) {
        const char *c_str = str.c_str();

        int base = 10;
        char *end_str;
        long long_int;
        errno = 0;
        long_int = std::strtol(c_str, &end_str, base);
        if ((errno == ERANGE && long_int == LONG_MAX) ||
            long_int > INT_MAX || long_int < INT_MIN ||
            *c_str == '\0' || *end_str != '\0') {
            return false;
        }

        result = (int)long_int;
        return true;
    }

    // Case-insensitive identity check two strings
    bool iequals(std::string str1, std::string str2) {
        if (str1.length() != str2.length()) {
            return false;
        } else {
            std::transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
            std::transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
            return str1 == str2;
        }
    }

    // Remove left whitespace (indentation) for each line
    std::string trimLeadingWhitespace(std::string str) {
        std::istringstream stream(str);
        std::string out;
        std::string line;    
        while (std::getline(stream, line)) {
            line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(::isspace))));
            out += line + '\n';
        }
        return out;
    }

    // Remove right whitespace (trailing space) for each line
    std::string trimTrailingWhitespace(std::string str) {
        std::istringstream stream(str);
        std::string out;
        std::string line;    
        while (std::getline(stream, line)) {
            line.erase(std::find_if(line.rbegin(), line.rend(), std::not1(std::ptr_fun<int, int>(::isspace))).base(), line.end());
            out += line + '\n';
        }
        return out;
    }

    // Remove all newline characters
    std::string mergeLines(std::string str) {
        str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
        str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
        str.erase(std::remove(str.begin(), str.end(), '\f'), str.end());
        return str;
    }

    // Remove all whitespace characters (space, FF, LF, CR, htab, vtab)
    std::string removeWhitespace(std::string str) {
        str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
        return str;
    }

    // Replace all occurences of 'sub_str' in 'str' with 'replacement'
    void replaceSubstrings(std::string &str, const std::string &sub_str, const std::string &replacement) {
        size_t pos = 0;
        while((pos = str.find(sub_str, pos)) != std::string::npos){
            str.replace(pos, sub_str.length(), replacement);
            pos += replacement.length();
        }
    }
}
