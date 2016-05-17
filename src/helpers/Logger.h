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

#ifndef PHARMML_LOGGER_H_
#define PHARMML_LOGGER_H_

#include <string>

namespace PharmML
{
    class Logger
    {
        public:
            void setToolname(std::string name);
            
            void warning(std::string message);
            void error(std::string message);
            void info(std::string message);

        private:
            const int error_level = 20;
            const int warning_level = 30;
            const int info_level = 40;
            std::string tool_name;
            int currentPrintLevel = 100;
            void logPrint(std::string message, int level);
    };
}

#endif
