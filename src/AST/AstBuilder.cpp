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
    std::unique_ptr<AstNode> AstBuilder::multiplyMany(std::vector<std::unique_ptr<AstNode>> &list) {
        std::unique_ptr<AstNode> prev = std::make_unique<BinopTimes>(std::move(list.end()[-2]), std::move(list.end()[-1]));      // The ultimate and penultimate element
        for (int i = (int) list.size() - 3; i >= 0; i--) {  // Loop backwards from the ante-penultimate element
            std::unique_ptr<AstNode> next = std::make_unique<BinopTimes>(std::move(list[i]), std::move(prev));
            prev = std::move(next);
        }

        return std::move(prev);
    }


    // FIXME: Major duplication of code! Need way of creating same Binop Type 
    std::unique_ptr<AstNode> AstBuilder::addMany(std::vector<std::unique_ptr<AstNode>> &list) {
        std::unique_ptr<AstNode> prev = std::make_unique<BinopPlus>(std::move(list.end()[-2]), std::move(list.end()[-1]));      // The ultimate and penultimate element
        for (int i = (int) list.size() - 3; i >= 0; i--) {  // Loop backwards from the ante-penultimate element
            std::unique_ptr<AstNode> next = std::make_unique<BinopPlus>(std::move(list[i]), std::move(prev));
            prev = std::move(next);
        }

        return std::move(prev); 
    }
}
