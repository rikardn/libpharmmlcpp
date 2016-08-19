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

#include "Version.h"
#include <iostream>

namespace pharmmlcpp
{
    Version::Version(int major, int minor, int patch) {
        this->major = major;
        this->minor = minor;
        this->patch = patch;
    }

    Version::Version(std::string version) {
        unsigned int first_dot_index = version.find_first_of(".");
        unsigned int last_dot_index = version.find_last_of(".");

        std::string major;
        std::string minor;
        std::string patch;

        if (first_dot_index == std::string::npos) {
            major = version;
        } else {
            major = version.substr(0, first_dot_index);
            if (first_dot_index != last_dot_index) {
                minor = version.substr(first_dot_index + 1, last_dot_index - first_dot_index - 1);
                patch = version.substr(last_dot_index + 1);
            } else {
                minor = version.substr(first_dot_index + 1);
                patch = "0";
            }
        }

        this->major = std::stoi(major);
        this->minor = std::stoi(minor);
        this->patch = std::stoi(patch);
    }

    std::string Version::toString() {
        std::string version = std::to_string(this->major) + "." + std::to_string(this->minor);
        if (patch != 0) {
            version += "." + std::to_string(this->patch);
        }
        return version;
    }
}
