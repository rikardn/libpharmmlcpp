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

#include "AstTransformation.h"
#include <visitors/AstAnalyzer.h>

namespace pharmmlcpp
{
    // Transform an AstNode into a vector of AstNodes
    // If the AstNode is a pure Vector the elements will be returned
    // and if not the single node will be put in a vector
    std::vector<std::shared_ptr<AstNode>> AstTransformation::toVector(std::shared_ptr<AstNode> node) {
        AstAnalyzer analyzer;
        node->accept(&analyzer);
        Vector *vector = analyzer.getPureVector();
        if (vector) {
            return vector->getElements();       // FIXME Currently this is a copy. Might have changed!
        } else {
            std::vector<std::shared_ptr<AstNode>> ast_vector;
            ast_vector.push_back(node);
            return ast_vector;
        } 
    }
}
