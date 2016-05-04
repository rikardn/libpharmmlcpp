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

#ifndef PHARMML_RTEXTFORMATTER_H_
#define PHARMML_RTEXTFORMATTER_H_

#include <string>
#include <sstream>
#include <vector>

namespace PharmML
{
    std::string formatVector(std::vector<std::string> vector, std::string prefix, std::string quote = "'", int pre_indent = 0);
    
    class RFormatter
    {
        private:
            int indentLevel = 0;
            int indentSize;
            char indentSymbol;
            std::vector<std::string> rows;
            
            struct VectorLevel {
                bool empty = true;
                int indentLevel = 0;
                char endSymbol;
                bool multiline;
                std::string separator;
            };
            std::vector<struct VectorLevel> vectorLevels;
            
            std::string genIndentation();
            void addCSV(std::string str);
        
        public:
            RFormatter(int size = 4, char symbol = ' ');
            void add(std::string str, bool ignore_separator = false);
            void append(std::string str);
            void indentAdd(std::string str);
            void outdentAdd(std::string str);
            void addMany(std::string str);
            void addMany(std::vector<std::string> strs, bool separate = true);
            
            void openIndent();
            void closeIndent();
            void openVector(std::string enclosure = "c()", int add_indent = 0, std::string separator = ", ");
            void closeVector();
            
            static std::string createVector(std::vector<std::string> strs, std::string enclosure = "c()", int indent_size = 0, std::string separator = ", ", bool final_newline = false);
            std::string createString(bool final_newline = true);
    };
}

#endif
