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

#include "RAstGenerator.h"

namespace pharmmlcpp
{
    // Parenthesize from a (root) node, accept it and return string
    std::string RAstGenerator::acceptRoot(AstNode *node) {
        node->accept(&this->parenthesizer);
        return this->accept(node);
    }

    // private
    void RAstGenerator::setValue(std::string str) {
        this->value = str;
    }

    // Set string value and parenthesize it if required
    void RAstGenerator::setParenthesizedValue(AstNode *node, std::string str) {
        if (node->hasParentheses()) str = "(" + str + ")";
        this->value = str;
    }

    std::string RAstGenerator::acceptLeft(Binop *binop) {
        binop->getLeft()->accept(this);
        return this->getValue();
    }

    std::string RAstGenerator::acceptRight(Binop *binop) {
        binop->getRight()->accept(this);
        return this->getValue();
    }

    std::string RAstGenerator::infix(Binop *binop, std::string op) {
        std::string result;
        binop->getLeft()->accept(this);
        result = this->getValue() + op;
        binop->getRight()->accept(this);
        result += this->getValue();
        return result;
    }

    std::string RAstGenerator::acceptChild(Uniop *uniop) {
        uniop->getChild()->accept(this);
        return this->getValue();
    }

    std::string RAstGenerator::accept(AstNode *node) {
        node->accept(this);
        return this->getValue();
    }

    std::string RAstGenerator::getLogicLiteral(bool value) {
        if (value) {
            return "TRUE";
        } else {
            return "FALSE";
        }
    }

    // public
    std::string RAstGenerator::getValue() {
        return this->value;
    }

    void RAstGenerator::visit(SymbRef *node) {
        this->setParenthesizedValue(node, node->getSymbol()->getName());
    }

    void RAstGenerator::visit(SteadyStateParameter *node) {
        node->getSymbRef()->accept(this);
        std::string symbref = this->getValue();
        node->getAssignment()->accept(this);
        std::string assignment = this->getValue();
        this->setParenthesizedValue(node, symbref + " = " + assignment);
    }

    void RAstGenerator::visit(ColumnRef *node) {
        this->setParenthesizedValue(node, node->toString());
    }

    void RAstGenerator::visit(UniopLog *node) {
        this->setParenthesizedValue(node, "log(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopLog2 *node) {
        this->setParenthesizedValue(node, "log2(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopLog10 *node) {
        this->setParenthesizedValue(node, "log10(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopExp *node) {
        this->setParenthesizedValue(node, "exp(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopMinus *node) {
        this->setParenthesizedValue(node, "-" + this->acceptChild(node));
    }

    void RAstGenerator::visit(UniopAbs *node) {
        this->setParenthesizedValue(node, "abs(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopSqrt *node) {
        this->setParenthesizedValue(node, "sqrt(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopLogistic *node) {
        this->setParenthesizedValue(node, "1/(1 + exp(-" + this->acceptChild(node) + "))");
    }

    void RAstGenerator::visit(UniopLogit *node) {
        this->setParenthesizedValue(node, "log((" + this->acceptChild(node) + ")/(1 - " + this->acceptChild(node) + "))");
    }

    void RAstGenerator::visit(UniopProbit *node) {
        this->setParenthesizedValue(node, "qnorm(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopNormcdf *node) {
        this->setParenthesizedValue(node, "pnorm(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopFactorial *node) {
        this->setParenthesizedValue(node, "factorial(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopFactln *node) {
        this->setParenthesizedValue(node, "lfactorial(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopGamma *node) {
        this->setParenthesizedValue(node, "gamma(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopGammaln *node) {
        this->setParenthesizedValue(node, "lgamma(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopSin *node) {
        this->setParenthesizedValue(node, "sin(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopSinh *node) {
        this->setParenthesizedValue(node, "sinh(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopCos *node) {
        this->setParenthesizedValue(node, "cos(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopCosh *node) {
        this->setParenthesizedValue(node, "cosh(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopTan *node) {
        this->setParenthesizedValue(node, "tan(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopTanh *node) {
        this->setParenthesizedValue(node, "tanh(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopCot *node) {
        this->setParenthesizedValue(node, "1/tan(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopCoth *node) {
        this->setParenthesizedValue(node, "1/tanh(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopSec *node) {
        this->setParenthesizedValue(node, "1/cos(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopSech *node) {
        this->setParenthesizedValue(node, "1/cosh(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopCsc *node) {
        this->setParenthesizedValue(node, "1/sin(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopCsch *node) {
        this->setParenthesizedValue(node, "1/sinh(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArcsin *node) {
        this->setParenthesizedValue(node, "asin(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArcsinh *node) {
        this->setParenthesizedValue(node, "asinh(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArccos *node) {
        this->setParenthesizedValue(node, "acos(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArccosh *node) {
        this->setParenthesizedValue(node, "acosh(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArctan *node) {
        this->setParenthesizedValue(node, "atan(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArctanh *node) {
        this->setParenthesizedValue(node, "atanh(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArccot *node) {
        this->setParenthesizedValue(node, "atan(1/" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArccoth *node) {
        this->setParenthesizedValue(node, "atanh(1/" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArcsec *node) {
        this->setParenthesizedValue(node, "acos(1/" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArcsech *node) {
        this->setParenthesizedValue(node, "acosh(1/" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArccsc *node) {
        this->setParenthesizedValue(node, "asin(1/" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArccsch *node) {
        this->setParenthesizedValue(node, "asinh(1/" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopHeaviside *node) {
        this->setParenthesizedValue(node, "(sign(" + this->acceptChild(node) + ") + 1) / 2");
    }

    void RAstGenerator::visit(UniopSign *node) {
        this->setParenthesizedValue(node, "sign(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopFloor *node) {
        this->setParenthesizedValue(node, "floor(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopCeiling *node) {
        this->setParenthesizedValue(node, "ceiling(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(ScalarInt *node) {
        this->setParenthesizedValue(node, node->toString());
    }

    void RAstGenerator::visit(ScalarReal *node) {
        // Format as scientific or float, whichever is shorter
        double d = node->toDouble();
        size_t len = std::snprintf(0, 0, "%.*G", 8, d);
        std::string s(len+1, 0);
        std::snprintf(&s[0], len+1, "%.*G", 8, d);
        s.pop_back();
        
        // Remove trailing zeroes after decimal point
        if (s.find('.') != std::string::npos) {
            // Find last zero (before 'E' if present)
            size_t exp_pos, last_nonzero, num_zeroes;
            if ((exp_pos = s.find('E')) == std::string::npos) {
                last_nonzero = s.find_last_not_of('0');
                num_zeroes = (s.length() - last_nonzero - 1);
            } else {
                last_nonzero = s.find_last_not_of('0', (exp_pos - 1));
                num_zeroes = (exp_pos - last_nonzero - 1);
            }
            if (last_nonzero != std::string::npos) {
                // Erase zeroes up to and including decimal point
                s.erase(last_nonzero + 1, num_zeroes);
                if(s.at(last_nonzero) == '.') {
                    s.erase(last_nonzero, 1);
                }
            }
        }
        
        this->setParenthesizedValue(node, s);
    }

    void RAstGenerator::visit(ScalarBool *node) {
        if (node->toBool() == true) {
            this->setParenthesizedValue(node, "TRUE");
        } else {
            this->setParenthesizedValue(node, "FALSE");
        }
    }
    
    void RAstGenerator::visit(ScalarString *node) {
        this->setParenthesizedValue(node, node->toString());
    }

    void RAstGenerator::visit(BinopPlus *node) {
        this->setParenthesizedValue(node, this->acceptLeft(node) + " + " + this->acceptRight(node));
    }

    void RAstGenerator::visit(BinopMinus *node) {
        this->setParenthesizedValue(node, this->acceptLeft(node) + " - " + this->acceptRight(node));
    }

    void RAstGenerator::visit(BinopDivide *node) {
        this->setParenthesizedValue(node, this->acceptLeft(node) + " / " + this->acceptRight(node));
    }

    void RAstGenerator::visit(BinopTimes *node) {
        this->setParenthesizedValue(node, this->acceptLeft(node) + " * " + this->acceptRight(node));
    }

    void RAstGenerator::visit(BinopPower *node) {
        this->setParenthesizedValue(node, this->acceptLeft(node) + " ^ " + this->acceptRight(node));
    }

    void RAstGenerator::visit(BinopLogx *node) {
        this->setParenthesizedValue(node, "log(" + this->acceptLeft(node) + ", base = " + this->acceptRight(node) + ")");
    }

    void RAstGenerator::visit(BinopRoot *node) {
        this->setParenthesizedValue(node, this->acceptLeft(node) + " ^ (1/" + this->acceptRight(node) + ")");
    }

    void RAstGenerator::visit(BinopMin *node) {
        this->setParenthesizedValue(node, "min(" + this->acceptLeft(node) + ", " + this->acceptRight(node) + ")");
    }

    void RAstGenerator::visit(BinopMax *node) {
        this->setParenthesizedValue(node, "max(" + this->acceptLeft(node) + ", " + this->acceptRight(node) + ")");
    }

    void RAstGenerator::visit(BinopRem *node) {
        this->setParenthesizedValue(node, this->acceptLeft(node) + " %% " + this->acceptRight(node));
    }

    void RAstGenerator::visit(BinopAtan2 *node) {
        this->setParenthesizedValue(node, "atan2(" + this->acceptLeft(node) + ", " + this->acceptRight(node) + ")");
    }

    void RAstGenerator::visit(LogicUniopIsdefined *node) {
        this->setParenthesizedValue(node, "!is.null(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(LogicUniopNot *node) {
        this->setParenthesizedValue(node, "!" + this->acceptChild(node));
    }

    void RAstGenerator::visit(LogicBinopLt *node) {
        this->setParenthesizedValue(node, this->infix(node, " < "));
    }

    void RAstGenerator::visit(LogicBinopLeq *node) {
        this->setParenthesizedValue(node, this->infix(node, " <= "));
    }

    void RAstGenerator::visit(LogicBinopGt *node) {
        this->setParenthesizedValue(node, this->infix(node, " > "));
    }

    void RAstGenerator::visit(LogicBinopGeq *node) {
        this->setParenthesizedValue(node, this->infix(node, " >= "));
    }

    void RAstGenerator::visit(LogicBinopEq *node) {
        this->setParenthesizedValue(node, this->infix(node, " == "));
    }

    void RAstGenerator::visit(LogicBinopNeq *node) {
        this->setParenthesizedValue(node, this->infix(node, " != "));
    }

    void RAstGenerator::visit(LogicBinopAnd *node) {
        this->setParenthesizedValue(node, this->infix(node, " && "));
    }

    void RAstGenerator::visit(LogicBinopOr *node) {
        this->setParenthesizedValue(node, this->infix(node, " || "));
    }

    void RAstGenerator::visit(LogicBinopXor *node) {
        this->setParenthesizedValue(node, "xor(" + this->acceptLeft(node) + ", " + this->acceptRight(node) + ")");
    }

    void RAstGenerator::visit(Vector *node) {
        std::vector<std::shared_ptr<AstNode>> elements = node->getElements();
        std::string s = "c(";
        bool first = true;
        for (std::shared_ptr<AstNode> element : elements) {
            if (first) {
                first = false;
            } else {
                s += ", ";
            }
            element->accept(this);
            s += this->getValue();
        }
        this->setParenthesizedValue(node, s + ")");
    }

    void RAstGenerator::visit(Piecewise *node) {
        std::vector<Piece *> pieces = node->getPieces();
        Piece *otherwise = nullptr;
        std::string s = "ifelse(";
        for (Piece *p : pieces) {
            if (!p->isOtherwise()) {
                p->accept(this);
                s += this->getValue() + ", (";
            } else {
                otherwise = p; // Only one otherwise per Piece
            }
        }
        if (otherwise == nullptr) {
            // And the missing otherwise said, Let it be 'NULL'. And all was good.
            NullValue *null = new NullValue();
            null->accept(this);
            s += this->getValue() + ")";
        } else {
            otherwise->getExpression()->accept(this);
            s += this->getValue();
        }
        this->setParenthesizedValue(node, s + std::string(pieces.size(), ')'));
    }

    void RAstGenerator::visit(Piece *node) {
        node->getCondition()->accept(this);
        std::string cond = this->getValue();
        node->getExpression()->accept(this);
        std::string expr = this->getValue();
        this->setParenthesizedValue(node, cond + ", " + expr);
    }

    void RAstGenerator::visit(Pi *node) {
        this->setParenthesizedValue(node, "pi");
    }

    void RAstGenerator::visit(Exponentiale *node) {
        this->setParenthesizedValue(node, "exp(1)");
    }

    void RAstGenerator::visit(NullValue *node) {
        this->setParenthesizedValue(node, "NULL");
    }

    void RAstGenerator::visit(FunctionCall *node) {
        bool first = true;
        std::string argument_list;
        for (FunctionArgument *arg : node->getFunctionArguments()) {
            if (first) {
                first = false;
            } else {
                argument_list += ", ";
            }
            arg->accept(this);
            argument_list += this->getValue();
        }
        node->getFunction()->accept(this);
        this->setParenthesizedValue(node, this->getValue() + "(" + argument_list + ")");
    }

    void RAstGenerator::visit(FunctionArgument *node) {
        node->getArgument()->accept(this);
        this->setParenthesizedValue(node, node->getSymbId() + "=" + this->getValue());
    }

    void RAstGenerator::visit(Interval *node) {
        std::string result = "list(left=" + this->accept(node->getLeftEndpoint());
        result += ", right=" + this->accept(node->getRightEndpoint());
        result += ", openleft=" + this->getLogicLiteral(node->isLeftEndpointOpenClosed());
        result += ", openright=" + this->getLogicLiteral(node->isRightEndpointOpenClosed());
        result += ")";
        this->setParenthesizedValue(node, result);
    }
}
