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

#include <AST/AstBuilder.h>
#include <AST/Binop.h>
#include <iostream>

namespace pharmmlcpp
{
    AstNode *AstBuilder::multiplyMany(std::vector<AstNode *> list) {
        BinopTimes *prev = new BinopTimes();
        prev->setRight(list.end()[-1]);     // The ultimate element
        prev->setLeft(list.end()[-2]);      // The penultimate element
        //for (std::vector<AstNode *>::size_type i = list.size() - 3; i >= 0; i--) {  // Loop backwards from the ante-penultimate element
        for (int i = (int) list.size() - 3; i >= 0; i--) {  // Loop backwards from the ante-penultimate element
            BinopTimes *next = new BinopTimes();
            next->setRight(prev);
            next->setLeft(list[i]);
            prev = next;
        }

        return prev; 
    }

    // FIXME: Major duplication of code! Need way of creating same Binop Type 
    AstNode *AstBuilder::addMany(std::vector<AstNode *> list) {
        BinopPlus *prev = new BinopPlus();
        prev->setRight(list.back());     // The ultimate element
        prev->setLeft(list.end()[-2]);      // The penultimate element

        for (int i = (int) list.size() - 3; i >= 0; i--) {  // Loop backwards from the ante-penultimate element
            BinopPlus *next = new BinopPlus();
            next->setRight(prev);
            next->setLeft(list[i]);
            prev = next;
        }

        return prev; 
    }
}
