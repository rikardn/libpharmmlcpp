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

#include <symbols/SymbolGathering.h>
#include <PharmML/Block.h>

namespace PharmML
{
    void SymbolGathering::newBlock(Block *block) {
        this->current_block = block;
    }
    
    void SymbolGathering::globalBlock() {
        this->current_block = nullptr;
    }

    void SymbolGathering::addSymbol(Symbol *symbol) {
        std::string blkId;
        if (current_block) {
            blkId = current_block->getBlkId();
        }
        this->map[blkId][symbol->getSymbId()] = symbol;
    }
}
