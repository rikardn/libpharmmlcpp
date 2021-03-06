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

// Temporary hack for missing filesystem class in MinGW

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#ifdef WINDOWS
#include <string>

#define SEP "\\"
#define SEPCH '\\'

namespace std::experimental::filesystem
{
    class path
    {
        private:
            std::string s;

        public:
            path() { s = ""; };
            path(std::string in) { s = in; };
            const char *c_str() { return s.c_str(); };
            std::experimental::filesystem::path operator/(const std::experimental::filesystem::path& p) {
                std::string pre;
                if (this->s == "") {
                    pre = "";
                } else {
                    pre = this->s + SEP;
                }
                std::experimental::filesystem::path n{pre + p.s};
                return n;
            };
            std::experimental::filesystem::path operator/=(const std::experimental::filesystem::path& right) {
                std::string pre;
                if (this->s == "") {
                    pre = "";
                } else {
                    pre = this->s + SEP;
                }
                this->s = pre + right.s;
                return *this;
            };
            bool operator==(const std::experimental::filesystem::path& right) { return this->s == right.s; };
            operator const std::string & () const { return this->s; };
            std::experimental::filesystem::path filename() {
                auto pos = this->s.rfind(SEPCH);
                if (pos == string::npos) {
                    return std::experimental::filesystem::path{this->s};
                } else {
                    return std::experimental::filesystem::path{this->s.substr(pos + 1, string::npos)};
                }
            };
            std::experimental::filesystem::path parent_path() {
                auto pos = this->s.rfind(SEPCH);
                if (pos == string::npos) {
                    return std::experimental::filesystem::path{".."};
                } else {
                    return std::experimental::filesystem::path{this->s.substr(0, pos)};
                }
            };
    };

    bool exists(std::experimental::filesystem::path p);
}

#endif

#endif
