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
#include "TextFormatter.h"

namespace PharmML
{
    // [[deprecated("Replaced by TextFormatter::createInlineVector/createIndentedVector")]]
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
    TextFormatter::TextFormatter(int size, char symbol) {
        this->indentSize = size;
        this->indentSymbol = symbol;
    }

    // Private: Generate indentation string
    std::string TextFormatter::genIndentation() {
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
    void TextFormatter::addCSV(std::string str) {
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
    void TextFormatter::add(std::string str, bool ignore_separator) {
        // Add as CSV only if separator isn't to be ignored
        if (this->vectorLevels.empty() || ignore_separator) {
            this->rows.push_back(this->genIndentation() + str);
        } else {
            this->addCSV(str);
        }
    }

    // Append last row added
    void TextFormatter::append(std::string str) {
        // If first row, create empty row
        if (this->rows.empty()) {
            this->rows.push_back("");
        }
        std::string &last_row = this->rows.back();
        last_row.append(str);
    }

    // Convenience method: Add and THEN increase indent
    void TextFormatter::indentAdd(std::string str) {
        if (this->vectorLevels.empty()) {
            this->add(str);
        } else {
            this->addCSV(str);
        }
        this->openIndent();
    }

    // Convenience method: Reduce indent and THEN add
    void TextFormatter::outdentAdd(std::string str) {
        this->closeIndent();
        if (this->vectorLevels.empty()) {
            this->add(str);
        } else {
            this->addCSV(str);
        }
    }

    // Split and add lines of multiline string individually
    void TextFormatter::addMany(std::string str) {
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
    void TextFormatter::addMany(std::vector<std::string> strs, bool separate) {
        if (!strs.empty()) {
            // First row adds a separator (if vector open)
            this->add(strs.front(), false);

            // If desired, subsequent lines adds separators (if vector open)
            for(auto it = strs.begin()+1; it != strs.end(); ++it) {
                this->add(*it, !separate);
            }
        }
    }

    // Add the contents of other TextFormatter
    void TextFormatter::addMany(TextFormatter& tf) {
        this->addMany(tf.rows, false);
    }

    // Increase indent one level
    void TextFormatter::openIndent() {
        if (this->vectorLevels.empty()) {
            this->indentLevel++;
        } else {
            this->vectorLevels.back().indentLevel++;
        }
    }

    // Decrease indent one level
    void TextFormatter::closeIndent() {
        if (this->vectorLevels.empty()) {
            this->indentLevel--;
        } else {
            this->vectorLevels.back().indentLevel--;
        }
    }

    // Open a vector with supplied enclosure (e.g. "c()"), extra indent and CSV separator
    void TextFormatter::openVector(std::string enclosure, int add_indent, std::string separator) {
        // Split enclosure into prefix, left/right parenthesis
        char right = 0, left = 0;
        if (enclosure.length() >= 2) {
            right = enclosure.back();
            enclosure.pop_back();
            left = enclosure.back();
            enclosure.pop_back();
        }

        // Create new vector level (nested structure allowed)
        struct VectorLevel vl;
        vl.endSymbol = right;
        vl.separator = separator;
        vl.indentLevel = add_indent;
        vl.multiline = (add_indent != 0); // No offset indent == single line

        // Add the vector header and vector level to stacks
        if (right && left) {
            this->add(enclosure + left, false);
        }
        this->vectorLevels.push_back(vl);
    }

    // Close a vector opened earlier
    void TextFormatter::closeVector() {
        if (!this->vectorLevels.empty()) {
            // Get ending symbol (right parenthesis)
            struct VectorLevel &vl = this->vectorLevels.back();
            std::string end;
            if (vl.endSymbol) {
                end = std::string(1, vl.endSymbol);
            } else {
                end = "";
            }

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
    std::string TextFormatter::createInlineVector(std::vector<std::string> strs, std::string enclosure, std::string separator) {
        TextFormatter vector(0, ' ');
        vector.openVector(enclosure, 0, separator);

        for (std::string str : strs) {
            vector.add(str, false);
        }

        // Do not include final newline for inline vector
        vector.noFinalNewline();
        return vector.createString();
    }

    // Instance-agnostic multi-row creation of a vector (formatVector replacement)
    std::string TextFormatter::createIndentedVector(std::vector<std::string> strs, std::string enclosure, std::string separator) {
        TextFormatter vector(1, ' ');
        vector.openVector(enclosure, 1, separator);

        for (std::string str : strs) {
            vector.add(str, false);
        }

        return vector.createString();
    }

    // Create a multirow string from this object
    std::string TextFormatter::createString() {
        // Close vectors if still on stack (optional elision!)
        while (!vectorLevels.empty()) {
            this->closeVector();
        }

        // Combine all rows with optional trailing newline char
        std::string result;
        for (auto &row : this->rows) {
            if (!this->final_newline && &row == &this->rows.back()) {
                result += row;
            } else {
                result += row + "\n";
            }
        }
        return result;
    }

    // Set the formatter to not output trailing newline upon createString
    void TextFormatter::noFinalNewline() {
        this->final_newline = false;
    }

    // Instance-agnostic createInlineVector wrapper for bare CSV lists
    std::string TextFormatter::createCommaSeparatedList(std::vector<std::string> list) {
        return createInlineVector(list, "", ", ");
    }

    void TextFormatter::emptyLine() {
        this->rows.push_back("");
    }
}
