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

#include "MacroGathering.h"
#include <PharmML/Block.h>
#include <iostream>

namespace pharmmlcpp
{
    void MacroGathering::newBlock(Block *block) {
        this->current_block = block;
    }

    void MacroGathering::addMacro(PKMacro *macro) {
        std::string blkId = current_block->getBlkId();
        if (macro->isCompartment()) {
            this->cmt_map[blkId][macro->getCmtNum()] = macro;
        } else if (macro->isAdministration()) {
            this->adm_map[blkId][macro->getAdmNum()] = macro;
        }
    }

    PKMacro *MacroGathering::getCmtMacro(std::string blkId, int cmt_num) {
        try {
            return this->cmt_map.at(blkId).at(cmt_num);
        } catch (std::out_of_range) {
            return nullptr;
        }
    }

    PKMacro *MacroGathering::getAdmMacro(std::string blkId, int adm_num) {
        try {
            return this->adm_map.at(blkId).at(adm_num);
        } catch (std::out_of_range) {
            return nullptr;
        }
    }
}
