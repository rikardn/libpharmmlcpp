/* libpharmmlcpp - Library to handle PharmML
 * Copyright (C) 2016 Rikard Nordgren and Gunnar Yngman
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the te   rms of the GNU Lesser General Public
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

#include <iostream>
#include "RTextFormatter.h"

namespace PharmML
{
    // [[deprecated("Replaced by RFormatter::createVector")]]
    // Pretend you're a C++14 compiler and parse above line in your mind
    std::string formatVector(std::vector<std::string> vector, std::string prefix, std::string quote, int pre_indent) {
        std::string s = prefix + "(";
        std::string sep = ", ";
        if (pre_indent > 0) {
            sep = ",\n" + std::string(pre_indent + s.size(), ' ');
        }
        
        bool first = true;
        for (std::string element : vector) {
            if (first) {
                first = false;
            } else {
                s += sep;
            }
            s += quote + element + quote;
        }
        return(s + ")");
    }

    // Construct via setting prefered indent size and symbol
    RFormatter::RFormatter(int size, char symbol) {
        this->indentSize = size;
        this->indentSymbol = symbol;
    }
    
    // Private: Generate indentation string
    std::string RFormatter::genIndentation() {
        int amount = this->indentLevel * this->indentSize;
        // Add up all open vectors
        for (struct VectorLevel vl: this->vectorLevels) {
            amount += vl.indentLevel * this->indentSize;
        }
        
        if (amount > 0) {
            return std::string(amount, this->indentSymbol);
        } else {
            return std::string();
        }
    }
    
    // Private: Add CSV-style value and separate it if necessary
    void RFormatter::addCSV(std::string str) {
        struct VectorLevel &vl = this->vectorLevels.back();
        
        // Append separator on last row if not first object
        if (!vl.empty) {
            this->append(vl.separator);
        }
        
        // For multiline add a row, for single line append on last row
        if (vl.multiline) {
            this->rows.push_back(this->genIndentation() + str);
        } else {
            this->append(str);
        }
        
        // We're not empty anymore
        if (vl.empty) {
            vl.empty = false;
        }
    }

    // Add a single unit (a row or CSV-style element if open vector)
    void RFormatter::add(std::string str, bool ignore_separator) {
        // Add as CSV only if separator isn't to be ignored
        if (this->vectorLevels.empty() || ignore_separator) {
            this->rows.push_back(this->genIndentation() + str);
        } else {
            this->addCSV(str);
        }
    }
    
    // Append last row added
    void RFormatter::append(std::string str) {
        // If first row, create empty row
        if (this->rows.empty()) {
            this->rows.push_back("");
        }
        std::string &last_row = this->rows.back();
        last_row.append(str);
    }
    
    // Convenience method: Add and THEN increase indent
    void RFormatter::indentAdd(std::string str) {
        if (this->vectorLevels.empty()) {
            this->add(str);
        } else {
            this->addCSV(str);
        }
        this->openIndent();
    }
    
    // Convenience method: Reduce indent and THEN add
    void RFormatter::outdentAdd(std::string str) {
        this->closeIndent();
        if (this->vectorLevels.empty()) {
            this->add(str);
        } else {
            this->addCSV(str);
        }
    }
    
    // Split and add lines of multiline string individually
    void RFormatter::addMany(std::string str) {
        // Split multi-line string into rows
        std::stringstream ss(str);
        std::string row;
        std::vector<std::string> rows;
        while(std::getline(ss,row,'\n')){
            rows.push_back(row);
        }
        // Call to plural form of add()
        this->addMany(rows);
    }
    
    // Add vector (plural of add()) as individual units
    void RFormatter::addMany(std::vector<std::string> strs, bool separate) {
        // First row adds a separator (if vector open)
        this->add(strs.front(), false);
        
        // If desired, subsequent lines adds separators (if vector open)
        for(auto it = strs.begin()+1; it != strs.end(); ++it) {
            this->add(*it, !separate);
        }
    }

    // Increase indent one level
    void RFormatter::openIndent() {
        if (this->vectorLevels.empty()) {
            this->indentLevel++;
        } else {
            this->vectorLevels.back().indentLevel++;
        }
    }
    
    // Decrease indent one level
    void RFormatter::closeIndent() {
        if (this->vectorLevels.empty()) {
            this->indentLevel--;
        } else {
            this->vectorLevels.back().indentLevel--;
        }
    }
    
    // Open a vector with supplied enclosure (e.g. "c()"), extra indent and CSV separator
    void RFormatter::openVector(std::string enclosure, int add_indent, std::string separator) {
        // Split enclosure into prefix, left/right parenthesis
        char right = enclosure.back();
        enclosure.pop_back();
        char left = enclosure.back();
        enclosure.pop_back();
        
        // Create new vector level (nested structure allowed)
        struct VectorLevel vl;
        vl.endSymbol = right;
        vl.separator = separator;
        vl.indentLevel = add_indent;
        vl.multiline = (add_indent != 0); // No offset indent == single line
        
        // Add the vector header and vector level to stacks
        this->add(enclosure + left, false);
        this->vectorLevels.push_back(vl);
    }
    
    // Close a vector opened earlier
    void RFormatter::closeVector() {
        if (!this->vectorLevels.empty()) {
            // Get ending symbol (right parenthesis)
            struct VectorLevel &vl = this->vectorLevels.back();
            std::string end = std::string(1, vl.endSymbol);
            
            // Restore indent level (offset => 0) and push symbol
            vl.indentLevel = 0;
            if (vl.multiline) {
                this->add(end, true);
            } else {
                this->append(end);
            }
            
            // Pop this vector level from stack
            this->vectorLevels.pop_back();
        }
    }

    // Instance-agnostic inline creation of a vector (formatVector replacement)
    std::string RFormatter::createVector(std::vector<std::string> strs, std::string enclosure, int indent_size, std::string separator, bool final_newline) {
        RFormatter vector(indent_size, ' ');
        if (indent_size == 0) {
            vector.openVector(enclosure, 0, separator);
        } else {
            vector.openVector(enclosure, 1, separator);
        }
        
        for (std::string str : strs) {
            vector.add(str, false);
        }
        
        return vector.createString(final_newline);
    }
    
    // Create a multirow string from this object
    std::string RFormatter::createString(bool final_newline) {
        // Close vectors if still on stack (optional elision!)
        while (!vectorLevels.empty()) {
            this->closeVector();
        }
        
        // Combine all rows with optional trailing newline char
        std::string result;
        for (auto &row : this->rows) {
            if (!final_newline && &row == &this->rows.back()) {
                result += row;
            } else {
                result += row + "\n";
            }
        }
        return result;
    }

    std::string RFormatter::createCommaSeparatedList(std::vector<std::string> list) {
        std::string result;
        for (int i = 0; i < list.size() - 1; i++) {
            result += list[i] + ", ";
        }
        result += list.back();

        return result;
    }

    void RFormatter::emptyLine() {
        this->rows.push_back("");
    }
}
