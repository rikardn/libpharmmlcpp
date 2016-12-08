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
#include <iostream>

namespace pharmmlcpp
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

    // Override the symbol blkId:symbId with the symbol. Used for overriding Covariates in TrialDesign
    // If override not possible simply add the symbol
    void SymbolGathering::overrideSymbol(Symbol *symbol, std::string blkId, std::string symbId) {
         this->map[blkId][symbId] = symbol;
    }

    Symbol *SymbolGathering::getSymbol(std::string blkId, std::string symbId) {
        try {
            return this->map.at(blkId).at(symbId);
        } catch (std::out_of_range) {
            return nullptr;
        }
    }

    SymbolSet SymbolGathering::getAllSymbols() {
        SymbolSet set;
        for (const auto &blk_pair : this->map) {
            for (const auto &symbref_pair : blk_pair.second) {
                set.addSymbol(symbref_pair.second);
            }
        }
        return set;
    }

    void SymbolGathering::setupAllSymbRefs() {
        for (auto &blkId_pair : this->map) {        // blkId_pair = (blkId, symbRef_pair)
            for (auto &symbRef_pair : blkId_pair.second) {      // symbRef_pair = (symbId, Symbol)
                symbRef_pair.second->setupSymbRefs(*this, blkId_pair.first);
            }
        }
    }
}
