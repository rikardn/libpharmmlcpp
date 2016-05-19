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

#ifndef PHARMML_POPEDASTGENERATOR_H_
#define PHARMML_POPEDASTGENERATOR_H_

#include <string>
#include <visitors/AstNodeVisitor.h>
#include <AST/symbols.h>
#include <AST/Uniop.h>
#include <AST/Binop.h>
#include <AST/Scalar.h>
#include <AST/Constant.h>
#include <AST/Vector.h>
#include <AST/Piecewise.h>
#include <symbols/DerivativeVariable.h>
#include <AST/FunctionCall.h>
#include <AST/Interval.h>
#include <PharmML/Distribution.h>
#include <PharmML/ColumnMapping.h>
#include <PharmML/Interventions.h>
#include <generators/R/RAstGenerator.h>
#include <generators/PopED/PopEDSymbols.h>


namespace PharmML
{
    class PopEDAstGenerator : public RAstGenerator
    {
        private:
            PopEDSymbols *symbgen;      // FIXME: This is so C++98. Should use unique_ptr<>

        public:
            PopEDAstGenerator();
            PopEDAstGenerator(PopEDSymbols *symbgen);
            ~PopEDAstGenerator();
            void visit(SymbRef *node) override;
    };
}

#endif
