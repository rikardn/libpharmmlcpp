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

namespace pharmmlcpp
{
    Logger::Logger() {

    }

    Logger::Logger(std::string name) {
        this->setToolName(name);
    }

    void Logger::setToolName(std::string name) {
        tool_name = name + " ";
    }

    // Warning, error and info for simple message
    void Logger::warning(std::string message) {
        logPrint(message, warning_level);
    }

    void Logger::error(std::string message) {
        logPrint(message, error_level);
    }

    void Logger::info(std::string message) {
        logPrint(message, info_level);
    }

    // Warning, error and info for message and suffixed line number from PharmMLSection-derived objects
    void Logger::warning(std::string message, PharmMLSection *section) {
        logPrint(message + lineText(section), warning_level);
    }

    void Logger::error(std::string message, PharmMLSection *section) {
        logPrint(message + lineText(section), error_level);
    }

    void Logger::info(std::string message, PharmMLSection *section) {
        logPrint(message + lineText(section), info_level);
    }

    /* Warning, error and info as above but with (up to two) in-line line-numberings.
     * E.g. "" */
    void Logger::warning(std::string format, PharmMLSection *section_a, PharmMLSection *section_b) {
        std::string message = this->inlineFormatter(format, section_a, section_b);
        logPrint(message, warning_level);
    }

    void Logger::error(std::string format, PharmMLSection *section_a, PharmMLSection *section_b) {
        std::string message = this->inlineFormatter(format, section_a, section_b);
        logPrint(message, error_level);
    }

    void Logger::info(std::string format, PharmMLSection *section_a, PharmMLSection *section_b) {
        std::string message = this->inlineFormatter(format, section_a, section_b);
        logPrint(message, info_level);
    }

    // Generate line text
    std::string Logger::lineText(PharmMLSection *section) {
        return " at line " + std::to_string(section->getXMLNode().getLineNo());
    }

    std::string Logger::inlineLineText(PharmMLSection *section) {
        return "line " + std::to_string(section->getXMLNode().getLineNo());
    }

    // Print message to stderr
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
            std::cerr << tool_name << type <<  ": " << message << std::endl;
        }
    }

    /* Generate message from format string (containing %a/%b) and two PharmMLSection's
     * (Should be done with variadic templates instead; It's C++11 after all!) */
    std::string Logger::inlineFormatter(std::string format, PharmMLSection *section_a, PharmMLSection *section_b) {
        std::string message = format;
        if (section_a) {
            std::string inline_text = this->inlineLineText(section_a);
            size_t pos = message.find("%a", 0);
            if (pos != std::string::npos) {
                message.replace(pos, 2, inline_text);
            }
        }
        if (section_b) {
            std::string inline_text = inlineLineText(section_b);
            size_t pos = message.find("%b", 0);
            if (pos != std::string::npos) {
                message.replace(pos, 2, inline_text);
            }
        }
        return message;
    }
}
