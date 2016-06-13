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

#ifndef PHARMMLCPP_LOGGER_H_
#define PHARMMLCPP_LOGGER_H_

#include <string>
#include <PharmML/PharmMLSection.h>

namespace pharmmlcpp
{
    class Logger
    {
        public:
            Logger();
            Logger(std::string name);
            void setToolName(std::string name);

            void warning(std::string message);
            void error(std::string message);
            void info(std::string message);

            void error(std::string message, PharmMLSection *section);
            void warning(std::string message, PharmMLSection *section);
            void info(std::string message, PharmMLSection *section);

            void error(std::string format, PharmMLSection *section_a, PharmMLSection *section_b);
            void warning(std::string format, PharmMLSection *section_a, PharmMLSection *section_b);
            void info(std::string format, PharmMLSection *section_a, PharmMLSection *section_b);

        private:
            const int error_level = 20;
            const int warning_level = 30;
            const int info_level = 40;
            std::string tool_name;
            int currentPrintLevel = 100;

            std::string lineText(PharmMLSection *section);
            std::string inlineLineText(PharmMLSection *section);
            void logPrint(std::string message, int level);
            std::string inlineFormatter(std::string format, PharmMLSection *section_a, PharmMLSection *section_b);
    };
}

#endif
