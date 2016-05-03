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
#include "AstNodeVisitor.h"
#include "symbols.h"
#include "Uniop.h"
#include "Binop.h"
#include "Scalar.h"
#include "Constant.h"
#include "Vector.h"
#include "Piecewise.h"
#include "DerivativeVariable.h"
#include "FunctionCall.h"
#include "Interval.h"
#include "Distribution.h"
#include "ColumnMapping.h"
#include "Interventions.h"
#include "RAstGenerator.h"

namespace PharmML
{
    class PopEDAstGenerator : public RAstGenerator
    {
        public:
            void visit(SymbRef *node);
    };
}

#endif
