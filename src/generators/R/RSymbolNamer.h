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

#ifndef PHARMML_RSYMBOLNAMER_H_
#define PHARMML_RSYMBOLNAMER_H_

#include <helpers/SymbolNamer.h>

namespace PharmML
{
    class RSymbolNamer : public SymbolNamer
    {
        public:
            RSymbolNamer() : SymbolNamer(std::unordered_set<std::string>(
                        {"if", "else", "repeat", "while", "function", "for", "in", "next", "break",
                         "TRUE", "FALSE", "NULL", "Inf", "NaN", "NA", "NA_integer_", "NA_real_", "NA_complex_", "NA_character_" })) {
                addCharSet(std::unordered_set<char32_t>(PharmML::LatinChars::ALL_EN.begin(), PharmML::LatinChars::ALL_EN.end())); 
                subInitialCharSet(std::unordered_set<char32_t>(PharmML::LatinChars::DIGITS.begin(), PharmML::LatinChars::DIGITS.end())); 
            };
    };
}

#endif
