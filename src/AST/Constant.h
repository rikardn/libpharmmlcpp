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

#ifndef PHARMML_CONSTANT_H_
#define PHARMML_CONSTANT_H_

#include <string>
#include <AST/AstNode.h>
#include <visitors/AstNodeVisitor.h>

namespace PharmML
{
    class Constant : public AstNode
    {

    };
    
    // Null value (example usage: missing otherwise piece)
    class NullValue : public Constant
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };
    
    // Boolean primitives
    class LogicFalse : public Constant
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };
    
    class LogicTrue : public Constant
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };
    
    // Pi and Euler's constant
    class Pi : public Constant
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };
    
    class Exponentiale : public Constant
    {
        public:
            virtual void accept(AstNodeVisitor *visitor);
    };
}

#endif
