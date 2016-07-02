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

#ifndef PHARMMLCPP_SYMBOLGATHERING_H_
#define PHARMMLCPP_SYMBOLGATHERING_H_

#include <string>
#include <unordered_map>
#include <symbols/Symbol.h>
#include <helpers/Logger.h>

namespace pharmmlcpp
{
    class Block;

    class SymbolGathering
    {
        public:
            Logger logger;
            void newBlock(Block *block);
            void globalBlock();
            void addSymbol(Symbol *symbol);
            Symbol *getSymbol(std::string blkId, std::string symbId);
            SymbolSet getAllSymbols();
            void setupAllSymbRefs();

        private:
            Block *current_block;
            std::unordered_map<std::string, std::unordered_map<std::string, Symbol *>> map;
    };
}

#endif
