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

#ifndef PHARMMLCPP_VERSION_H_
#define PHARMMLCPP_VERSION_H_

#include <string>

namespace pharmmlcpp
{
    class Version
    {
        public:
            Version(int major, int minor, int patch);
            Version(std::string version);
            bool operator==(const Version &rhs) const { return (this->major == rhs.major && this->minor == rhs.minor && this->patch == rhs.patch); }
            bool operator!=(const Version &rhs) const { return !(*this == rhs); }
            bool operator<(const Version &rhs) const { return (this->major < rhs.major || (this->major == rhs.major && this->minor < rhs.minor) || (this->major == rhs.major && this->minor == rhs.minor && this->patch < rhs.patch)); }
            bool operator>(const Version& rhs) const { return rhs < *this; }
            bool operator<=(const Version& rhs) const { return !(*this > rhs); }
            bool operator>=(const Version& rhs) const  { return !(*this < rhs); }

            std::string toString();

        private:
            int major;
            int minor;
            int patch;
    };
}

#endif
