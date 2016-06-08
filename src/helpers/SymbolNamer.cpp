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

#include "SymbolNamer.h"

namespace PharmML
{
    // Initialize new SymbolNamer (without illegal word set)
    SymbolNamer::SymbolNamer() {

    } 
    // Initialize new SymbolNamer with list of illegal word (e.g. target reserved)
    SymbolNamer::SymbolNamer(std::unordered_set<std::u32string> illegal_words) {
        this->illegal_words = illegal_words;
    }

    // Add a charset to collection of word legal (throughout) chars
    void SymbolNamer::addCharSet(std::unordered_set<char32_t> chars) {
        this->legal_chars.insert(chars.begin(), chars.end());
    }

    // Add a charset to collection of word legal initial chars
    void SymbolNamer::addInitialCharSet(std::unordered_set<char32_t> chars) {
        this->legal_initial_chars.insert(chars.begin(), chars.end());
    }

    // Set a prefix to prepend for collisions with reserved words
    void SymbolNamer::setReservedPrefix(std::u32string prefix) {
        this->reserved_prefix = prefix;
    }

    // Get name of symbol or generate name if none exists
    std::u32string SymbolNamer::getName(Symbol *symbol) {
        // Return name if symbol already named
        if (this->name_map.find(symbol) != this->name_map.end()) {
            return name_map[symbol];
        }

        // Start with symbId as name
        std::string legacy_name = symbol->getSymbId(); // TODO: Remove when full Unicode support
        std::u32string name = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.from_bytes(legacy_name);

        // Try to shift case for each character if illegal
        name = this->shiftIllegalCase(name, this->legal_chars);

        // Substitute characters not in legal set
        for (auto it = name.begin(); it < name.end(); ++it) {
            if (!this->charInSet(*it, this->legal_chars)) {
                *it = this->substituteStandardChar(*it, this->legal_chars);
            }
            if (it == name.begin() && !this->charInSet(*it, this->legal_initial_chars)) {
                *it = this->substituteStandardChar(*it, this->legal_initial_chars);
            }
        }

        // If illegal word was formed, try to modify into legal word
        if (this->illegal_words.count(name) > 0) {
            if (!this->reserved_prefix.empty()) {
                name = this->reserved_prefix + name;
            }
            name = this->avoidCollision(name, this->illegal_words);
        }

        // Modify if name is not unique
        name = this->avoidCollision(name, this->names);

        // Remember and return generated name
        names.insert(name);
        this->name_map[symbol] = name;
        return name;
    }

    // Private helper functions

    // Returns true if char is in set
    bool SymbolNamer::charInSet(char32_t ch, const std::unordered_set<char32_t> &ch_set) {
        auto got = ch_set.find(ch);
        if (got != ch_set.end()) {
            return true;
        } else {
            return false;
        }
    }

    // Try to substitute the (illegal) char with alternative (standard) char
    char32_t SymbolNamer::substituteStandardChar(char32_t ch, const std::unordered_set<char32_t> &legal_chars) {
        // Defined substitutions
        char32_t def_sub = 'X';
        std::unordered_map<char32_t, char32_t> std_alts = {
            {u'\uC3A5', 'a'}, {u'\uC385', 'A'}, // å,Å
            {u'\uC3A4', 'a'}, {u'\uC384', 'A'}, // ä,Ä
            {u'\uC3B6', 'O'}, {u'\uC396', 'O'}, // ö,Ö
            {u'\u0020', '_'}, {u'\u0009', '_'}, // space,htab
            {u'\u000A', '_'}, {u'\u000D', '_'}  // LF,CR
        };

        // Substitute
        char32_t sub;
        if (std_alts.count(ch) > 0) {
            sub = std_alts[ch];
        } else {
            sub = def_sub;
        }

        // Substitute again if not legal
        if (legal_chars.count(sub) == 0) {
            if (legal_chars.count(def_sub) > 0) {
                sub = def_sub;
            } else {
                // Fallback on any legal character
                sub = *(legal_chars.begin());
            }
        }
        return sub;
    }

    // Try to substitute the other case char. Returns same char if not convertable and char 0 if not legal.
    char32_t SymbolNamer::substituteOtherCase(char32_t ch, const std::unordered_set<char32_t> &legal_chars) {
        // Get latin conversion maps
        std::unordered_map<char32_t, char32_t> upper_latin, lower_latin;
        std::transform(std::begin(LatinChars::LOWER_ALPHA), std::end(LatinChars::LOWER_ALPHA)
            , std::begin(LatinChars::UPPER_ALPHA)
            , std::inserter(upper_latin, std::begin(upper_latin))
            , [] (char32_t l, char32_t u) { return std::make_pair(l, u); });
        std::transform(std::begin(LatinChars::UPPER_ALPHA), std::end(LatinChars::UPPER_ALPHA)
            , std::begin(LatinChars::LOWER_ALPHA)
            , std::inserter(lower_latin, std::begin(lower_latin))
            , [] (char32_t u, char32_t l) { return std::make_pair(u, l); });

        // Swap latin case
        char32_t other_case = 0;
        if (upper_latin.count(ch) > 0) {
            other_case = upper_latin[ch];
        } else if (lower_latin.count(ch) > 0) {
            other_case = lower_latin[ch];
        }

        // Return same char if no conversion, 0 if conversion is not legal and resulting char if sucessful
        if (other_case == 0) {
            return ch;
        } else if (legal_chars.count(other_case) > 0) {
            return other_case;
        } else {
            return 0;
        }
    }

    std::u32string SymbolNamer::shiftIllegalCase(std::u32string name, const std::unordered_set<char32_t> &legal_chars) {
        std::u32string new_name = name;

        // Try to swap case for each illegal character
        for (auto it = new_name.begin(); it < new_name.end(); ++it) {
            if (!this->charInSet(*it, this->legal_chars)) {
                char32_t sub = this->substituteOtherCase(*it, legal_chars);
                if (sub == 0) {
                    sub = this->substituteStandardChar(*it, legal_chars);
                }
                *it = sub;
            }
        }

        return new_name;
    }

    // Check if name collides with set and if so, try to iterate a new word
    std::u32string SymbolNamer::avoidCollision(std::u32string name, const std::unordered_set<std::u32string> &illegals) {
        // Return immediately if no collision
        std::u32string new_name = name;
        if (illegals.count(new_name) == 0) {
            return new_name;
        }
        
        // Try to get legal suffix separator
        const std::vector<char32_t> try_seps = {'_','-','.'};
        std::u32string sep;
        for (char32_t try_sep : try_seps) {
            if (this->legal_chars.count(try_sep) > 0) {
                sep = std::u32string(1, try_sep);
                break;
            }
        }

        // Get legal digits for iteration
        std::vector<char32_t> digits;
        std::set_intersection(LatinChars::DIGITS.begin(), LatinChars::DIGITS.end(), this->legal_chars.begin(), this->legal_chars.end(), std::back_inserter(digits));

        // Try to iterate away from collisions
        new_name = name + sep;
        if (digits.size() > 0) {
            uint version = 1;
            while (illegals.count(new_name)) {
                std::u32string suffix = this->stringFromNumerals(version, digits);
                new_name = name + sep + suffix;
                if (illegals.count(new_name) == 0) {
                    return new_name;
                }
                version++;
            };
        } else {
            // Just try any mess of legal characters remaining...
            std::vector<char32_t> mess(this->legal_chars.begin(), this->legal_chars.end());
            uint version = 1;
            while (illegals.count(new_name)) {
                std::u32string suffix = this->stringFromNumerals(version, mess);
                new_name = name + sep + suffix;
                if (illegals.count(new_name) == 0) {
                    return new_name;
                }
                version++;
            };
        }
        return new_name;
    }

    std::u32string SymbolNamer::stringFromNumerals(uint num, const std::vector<char32_t> &numerals) {
        std::u32string result;
        uint remainder = num;
        while (remainder > 0) {
            uint digit;
            if (remainder < numerals.size()) {
                digit = remainder;
                remainder = 0;
            } else {
                digit = remainder / numerals.size();
                remainder -= digit * numerals.size();
            }
            result += numerals.at(digit);
        }
        return result;
    }
}
