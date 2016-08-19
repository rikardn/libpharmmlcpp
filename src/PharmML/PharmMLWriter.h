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

#ifndef PHARMMLCPP_PHARMMLWRITER_H_
#define PHARMMLCPP_PHARMMLWRITER_H_

#include <unordered_map>
#include <xml/xml.h>
#include <AST/AstNodeFactory.h>
#include <AST/symbols.h>
#include <helpers/Logger.h>
#include <memory>
#include <helpers/Version.h>

namespace pharmmlcpp
{
    class PharmML;
    class PharmMLWriter
    {
        public:
            PharmMLWriter(std::string filename) : filename(filename), version(0, 8, 1) {}
            PharmMLWriter(std::string filename, std::string version) : filename(filename), version(version) {}

        private:
            std::string filename;
            Version version;
    };
}

#endif
