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

namespace std::experimental::filesystem
{
    class path
    {
        private:
            std::string s;

        public:
            path(std::string in) { s = in };
            char *c_str() { return s.c_str() };
            path operator/(const path& p) { path n; n.s = this->s + "\\" + p.s };
    };
}
