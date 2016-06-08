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

#ifndef PHARMML_SYMBOLNAMER_H_
#define PHARMML_SYMBOLNAMER_H_

#include <set>
#include <unordered_map>
#include <unordered_set>
#include <codecvt>
#include <locale>
#include <iostream>
#include <string>
#include <algorithm>

#include <symbols/Symbol.h>

namespace PharmML
{
    namespace LatinChars {
        const std::vector<char32_t> DIGITS      = {'0','1','2','3','4','5','6','7','8','9'};
        const std::vector<char32_t> LOWER_ALPHA = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
        const std::vector<char32_t> UPPER_ALPHA = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
        const std::vector<char32_t> UNDERSCORE  = {'_'};
        const std::vector<char32_t> PERIOD      = {'.'};
        const std::vector<char32_t> HYPHEN      = {'-'};

        const std::vector<char32_t> ALL         = {
            '0','1','2','3','4','5','6','7','8','9'
            ,'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
            ,'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
            ,'_','.','-'
        };
    };

    class SymbolNamer
    {
        public:
            SymbolNamer();
            SymbolNamer(std::unordered_set<std::u32string> reserved_words);
            SymbolNamer(std::unordered_set<std::string> reserved_words);

            // Set target tool restrictions
            void addCharSet(std::unordered_set<char32_t> chars);
            void addInitialCharSet(std::unordered_set<char32_t> chars);
            void setReservedPrefix(std::u32string prefix);

            // Name getters
            std::u32string getName(Symbol *symbol);
            std::string getNameString(Symbol *symbol);

        private:
            // Target tool restrictions
            std::unordered_set<std::u32string> illegal_words;
            std::unordered_set<char32_t> legal_chars;
            std::unordered_set<char32_t> legal_initial_chars;
            std::u32string reserved_prefix;

            // Earlier generated names
            std::unordered_set<std::u32string> names;
            std::unordered_map<Symbol*, std::u32string> name_map;

            // Private helper functions
            bool charInSet(char32_t ch, const std::unordered_set<char32_t> &ch_set);
            
            char32_t substituteStandardChar(char32_t ch, const std::unordered_set<char32_t> &legal_chars);
            std::u32string substituteIllegalChars(std::u32string name, const std::unordered_set<char32_t> &legal_chars, const std::unordered_set<char32_t> &legal_initial_chars);
            
            char32_t substituteOtherCase(char32_t ch, const std::unordered_set<char32_t> &legal_chars);
            std::u32string shiftIllegalCase(std::u32string name, const std::unordered_set<char32_t> &legal_chars);
            
            std::u32string escapeCollision(std::u32string name, const std::unordered_set<std::u32string> &illegals);
            std::u32string stringFromNumerals(uint num, const std::vector<char32_t> &numerals);
    };
}

#endif
