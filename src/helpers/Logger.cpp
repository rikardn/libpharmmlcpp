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

#include "Logger.h"
#include <iostream>

namespace PharmML
{
    void Logger::setToolname(std::string name) {
        tool_name = name + " ";
    }

    void Logger::warning(std::string message) {
        logPrint(message, warning_level);
    }

    void Logger::warning(std::string message, PharmMLSection *section) {
        logPrint(message + lineText(section), warning_level);
    }

    void Logger::error(std::string message) {
        logPrint(message, error_level);
    }
     
    void Logger::error(std::string message, PharmMLSection *section) {
        logPrint(message + lineText(section), error_level);
    }

    void Logger::info(std::string message) {
        logPrint(message, info_level);
    }

    void Logger::info(std::string message, PharmMLSection *section) {
        logPrint(message + lineText(section), info_level);
    }

    std::string Logger::lineText(PharmMLSection *section) {
        return " at line " + std::to_string(section->getXMLNode().getLineNo());
    }

    void Logger::logPrint(std::string message, int level) {
        std::string type;
        if (level == warning_level) {
            type = "Warning";
        } else if (level == error_level) {
            type = "Error";
        } else if (level == info_level) {
            type = "Info";
        }

        if (currentPrintLevel > level) {
            std::cout << tool_name << type <<  ": " << message << std::endl;
        }
    }
}
