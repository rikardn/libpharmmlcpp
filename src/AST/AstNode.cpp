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

#include <string>
#include <iostream>
#include <xml/xml.h>

#include <AST/AstNode.h>
#include <AST/Uniop.h>
#include <AST/Binop.h>
#include <AST/symbols.h>
#include <AST/Scalar.h>
#include <AST/Constant.h>
#include <AST/Vector.h>
#include <AST/Piecewise.h>
#include <AST/FunctionCall.h>
#include <AST/Interval.h>
#include <AST/Sequence.h>

namespace pharmmlcpp
{
    std::unique_ptr<AstNode> AstNode::create(PharmMLReader &reader, xml::Node node) {
        std::unique_ptr<AstNode> instance;

        std::string name = node.getName();

        // Skip past initial ct:Assign
        if (name == "Assign") {
            node = node.getChild();
            name = node.getName();
        }

        if (name == "Uniop" || name == "LogicUniop") {
            std::string op = node.getAttribute("op").getValue();
            std::unique_ptr<Uniop> uniop;
            std::unique_ptr<AstNode> child = AstNode::create(reader, node.getChild());
            // Ordinary uniops
            if (op == "log") {
                uniop = std::make_unique<UniopLog>(std::move(child));
            } else if (op == "log2") {
                uniop = std::make_unique<UniopLog2>(std::move(child));
            } else if (op == "log10") {
                uniop = std::make_unique<UniopLog10>(std::move(child));
            } else if (op == "exp") {
                uniop = std::make_unique<UniopExp>(std::move(child));
            } else if (op == "minus") {
                uniop = std::make_unique<UniopMinus>(std::move(child));
            } else if (op == "abs") {
                uniop = std::make_unique<UniopAbs>(std::move(child));
            } else if (op == "sqrt") {
                uniop = std::make_unique<UniopSqrt>(std::move(child));
            } else if (op == "logistic") {
                uniop = std::make_unique<UniopLogistic>(std::move(child));
            } else if (op == "logit") {
                uniop = std::make_unique<UniopLogit>(std::move(child));
            } else if (op == "probit") {
                uniop = std::make_unique<UniopProbit>(std::move(child));
            } else if (op == "normcdf") {
                uniop = std::make_unique<UniopNormcdf>(std::move(child));
            } else if (op == "factorial") {
                uniop = std::make_unique<UniopFactorial>(std::move(child));
            } else if (op == "factln") {
                uniop = std::make_unique<UniopFactln>(std::move(child));
            } else if (op == "gamma") {
                uniop = std::make_unique<UniopGamma>(std::move(child));
            } else if (op == "gammaln") {
                uniop = std::make_unique<UniopGammaln>(std::move(child));
            } else if (op == "sin") {
                uniop = std::make_unique<UniopSin>(std::move(child));
            } else if (op == "sinh") {
                uniop = std::make_unique<UniopSinh>(std::move(child));
            } else if (op == "cos") {
                uniop = std::make_unique<UniopCos>(std::move(child));
            } else if (op == "cosh") {
                uniop = std::make_unique<UniopCosh>(std::move(child));
            } else if (op == "tan") {
                uniop = std::make_unique<UniopTan>(std::move(child));
            } else if (op == "tanh") {
                uniop = std::make_unique<UniopTanh>(std::move(child));
            } else if (op == "cot") {
                uniop = std::make_unique<UniopCot>(std::move(child));
            } else if (op == "coth") {
                uniop = std::make_unique<UniopCoth>(std::move(child));
            } else if (op == "sec") {
                uniop = std::make_unique<UniopSec>(std::move(child));
            } else if (op == "sech") {
                uniop = std::make_unique<UniopSech>(std::move(child));
            } else if (op == "csc") {
                uniop = std::make_unique<UniopCsc>(std::move(child));
            } else if (op == "csch") {
                uniop = std::make_unique<UniopCsch>(std::move(child));
            } else if (op == "arcsin") {
                uniop = std::make_unique<UniopArcsin>(std::move(child));
            } else if (op == "arcsinh") {
                uniop = std::make_unique<UniopArcsinh>(std::move(child));
            } else if (op == "arccos") {
                uniop = std::make_unique<UniopArccos>(std::move(child));
            } else if (op == "arccosh") {
                uniop = std::make_unique<UniopArccosh>(std::move(child));
            } else if (op == "arctan") {
                uniop = std::make_unique<UniopArctan>(std::move(child));
            } else if (op == "arctanh") {
                uniop = std::make_unique<UniopArctanh>(std::move(child));
            } else if (op == "arccot") {
                uniop = std::make_unique<UniopArccot>(std::move(child));
            } else if (op == "arccoth") {
                uniop = std::make_unique<UniopArccoth>(std::move(child));
            } else if (op == "arcsec") {
                uniop = std::make_unique<UniopArcsec>(std::move(child));
            } else if (op == "arcsech") {
                uniop = std::make_unique<UniopArcsech>(std::move(child));
            } else if (op == "arccsc") {
                uniop = std::make_unique<UniopArccsc>(std::move(child));
            } else if (op == "arccsch") {
                uniop = std::make_unique<UniopArccsch>(std::move(child));
            } else if (op == "heaviside") {
                uniop = std::make_unique<UniopHeaviside>(std::move(child));
            } else if (op == "sign") {
                uniop = std::make_unique<UniopSign>(std::move(child));
            } else if (op == "floor") {
                uniop = std::make_unique<UniopFloor>(std::move(child));
            } else if (op == "ceiling") {
                uniop = std::make_unique<UniopCeiling>(std::move(child));
            // Logic uniops. FIXME: This is really an error. not could as well be a Uniop here.
            } else if (op == "isDefined") {
                uniop = std::make_unique<LogicUniopIsdefined>(std::move(child));
            } else if (op == "not") {
                uniop = std::make_unique<LogicUniopNot>(std::move(child));
            }
            instance = std::move(uniop);
        } else if (name == "Binop" || name == "LogicBinop") {
            std::string op = node.getAttribute("op").getValue();
            std::unique_ptr<Binop> binop;
            std::unique_ptr<AstNode> left = AstNode::create(reader, node.getChild());
            std::unique_ptr<AstNode> right = AstNode::create(reader, node.getLastChild());
            // Ordinary binops
            if (op == "plus") {
                binop = std::make_unique<BinopPlus>(std::move(left), std::move(right));
            } else if (op == "minus") {
                binop = std::make_unique<BinopMinus>(std::move(left), std::move(right));
            } else if (op == "divide") {
                binop = std::make_unique<BinopDivide>(std::move(left), std::move(right));
            } else if (op == "times") {
                binop = std::make_unique<BinopTimes>(std::move(left), std::move(right));
            } else if (op == "power") {
                binop = std::make_unique<BinopPower>(std::move(left), std::move(right));
            } else if (op == "logx") {
                binop = std::make_unique<BinopLogx>(std::move(left), std::move(right));
            } else if (op == "root") {
                binop = std::make_unique<BinopRoot>(std::move(left), std::move(right));
            } else if (op == "min") {
                binop = std::make_unique<BinopMin>(std::move(left), std::move(right));
            } else if (op == "max") {
                binop = std::make_unique<BinopMax>(std::move(left), std::move(right));
            } else if (op == "rem") {
                binop = std::make_unique<BinopRem>(std::move(left), std::move(right));
            } else if (op == "atan2") {
                binop = std::make_unique<BinopAtan2>(std::move(left), std::move(right));
            // Logic binops
            } else if (op == "lt") {
                binop = std::make_unique<LogicBinopLt>(std::move(left), std::move(right));
            } else if (op == "leq") {
                binop = std::make_unique<LogicBinopLeq>(std::move(left), std::move(right));
            } else if (op == "gt") {
                binop = std::make_unique<LogicBinopGt>(std::move(left), std::move(right));
            } else if (op == "geq") {
                binop = std::make_unique<LogicBinopGeq>(std::move(left), std::move(right));
            } else if (op == "eq") {
                binop = std::make_unique<LogicBinopEq>(std::move(left), std::move(right));
            } else if (op == "neq") {
                binop = std::make_unique<LogicBinopNeq>(std::move(left), std::move(right));
            } else if (op == "and") {
                binop = std::make_unique<LogicBinopAnd>(std::move(left), std::move(right));
            } else if (op == "or") {
                binop = std::make_unique<LogicBinopOr>(std::move(left), std::move(right));
            } else if (op == "xor") {
                binop = std::make_unique<LogicBinopXor>(std::move(left), std::move(right));
            }
            instance = std::move(binop);
        } else if (name == "ConstantType") {
            std::string op = node.getAttribute("op").getValue();
            std::unique_ptr<Constant> constant;
            if (op == "pi") {
                constant = std::make_unique<Pi>();
            } else if (op == "exponentiale") {
                constant = std::make_unique<Exponentiale>();
            }
            instance = std::move(constant);
        } else if (name == "SymbRef") {
            std::unique_ptr<SymbRef> symbref = std::make_unique<SymbRef>(reader, node);
            instance = std::move(symbref);
        } else if (name == "ColumnRef") {
            std::string symbol = node.getAttribute("columnIdRef").getValue();
            instance = std::make_unique<ColumnRef>(symbol);
        } else if (name == "CatRef") {
            std::unique_ptr<CatRef> catref = std::make_unique<CatRef>(reader, node);
            instance = std::move(catref);
        } else if (name == "Int") {
            instance = std::make_unique<ScalarInt>(reader, node);
        } else if (name == "Real") {
            instance = std::make_unique<ScalarReal>(reader, node);
        } else if (name == "True") {
            instance = std::make_unique<ScalarBool>(true);
        } else if (name == "False") {
            instance = std::make_unique<ScalarBool>(false);
        } else if (name == "String") {
            instance = std::make_unique<ScalarString>(reader, node);
        } else if (name == "Id") {
            instance = std::make_unique<ScalarId>(reader, node);
        } else if (name == "NA") {
            instance = std::make_unique<ScalarNA>();
        } else if (name == "NaN") {
            instance = std::make_unique<ScalarNaN>();
        } else if (name == "plusInf") {
            instance = std::make_unique<ScalarPlusInf>();
        } else if (name == "minusInf") {
            instance = std::make_unique<ScalarMinusInf>();
        } else if (name == "BLQ") {
            instance = std::make_unique<ScalarBLQ>();
        } else if (name == "ALQ") {
            instance = std::make_unique<ScalarALQ>();
        } else if (name == "Vector") {
            instance = std::make_unique<Vector>(reader, node);
        } else if (name == "Piecewise") {
            Piecewise *piecewise = new Piecewise();
            std::vector<xml::Node> children = node.getChildren();
            for (xml::Node n : children) {
                Piece *piece = new Piece();
                piece->setXMLNode(n);
                piecewise->addPiece(piece);
                // Assumes expression is first child and condition last child
                xml::Node expression = n.getChild();
                xml::Node condition = n.getLastChild().getChild();
                piece->setExpression(AstNode::create(reader, expression));
                piece->setCondition(AstNode::create(reader, condition));
                // Otherwise property gets lost in translation from xml::Node to AstNode so save it now
                if (condition.getName() == "Otherwise") {
                    piece->setOtherwise();
                }
            }
            instance = std::unique_ptr<AstNode>(piecewise);
        } else if (name == "FunctionCall") {
            instance = std::make_unique<FunctionCall>(reader, node);
        } else if (name == "Interval") {
            instance = std::make_unique<Interval>(reader, node);
        } else if (name == "Sequence") {
            instance = std::make_unique<Sequence>(reader, node);
        }

        // The following might and might not be a good idea
        if (instance) {
            instance->setXMLNode(node);
        }

        return instance;
    }
}
