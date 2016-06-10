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

namespace PharmML
{
    // private
    void RAstGenerator::setValue(std::string str) {
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
        result = "(" + this->getValue() + op;
        binop->getRight()->accept(this);
        result += this->getValue() + ")";
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
        this->setValue(node->getSymbol()->getName());
    }

    void RAstGenerator::visit(SteadyStateParameter *node) {
        node->getSymbRef()->accept(this);
        std::string symbref = this->getValue();
        node->getAssignment()->accept(this);
        std::string assignment = this->getValue();
        this->setValue(symbref + " = " + assignment);
    }

    void RAstGenerator::visit(ColumnRef *node) {
        this->setValue(node->toString());
    }

    void RAstGenerator::visit(UniopLog *node) {
        this->setValue("log(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopLog2 *node) {
        this->setValue("log2(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopLog10 *node) {
        this->setValue("log10(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopExp *node) {
        this->setValue("exp(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopMinus *node) {
        this->setValue("(-" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopAbs *node) {
        this->setValue("abs(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopSqrt *node) {
        this->setValue("sqrt(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopLogistic *node) {
        this->setValue("(1/(1 + exp(-" + this->acceptChild(node) + ")))");
    }

    void RAstGenerator::visit(UniopLogit *node) {
        this->setValue("log((" + this->acceptChild(node) + ")/(1 - " + this->acceptChild(node) + "))");
    }

    void RAstGenerator::visit(UniopProbit *node) {
        this->setValue("qnorm(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopNormcdf *node) {
        this->setValue("pnorm(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopFactorial *node) {
        this->setValue("factorial(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopFactln *node) {
        this->setValue("lfactorial(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopGamma *node) {
        this->setValue("gamma(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopGammaln *node) {
        this->setValue("lgamma(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopSin *node) {
        this->setValue("sin(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopSinh *node) {
        this->setValue("sinh(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopCos *node) {
        this->setValue("cos(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopCosh *node) {
        this->setValue("cosh(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopTan *node) {
        this->setValue("tan(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopTanh *node) {
        this->setValue("tanh(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopCot *node) {
        this->setValue("(1/tan(" + this->acceptChild(node) + "))");
    }

    void RAstGenerator::visit(UniopCoth *node) {
        this->setValue("(1/tanh(" + this->acceptChild(node) + "))");
    }

    void RAstGenerator::visit(UniopSec *node) {
        this->setValue("(1/cos(" + this->acceptChild(node) + "))");
    }

    void RAstGenerator::visit(UniopSech *node) {
        this->setValue("(1/cosh(" + this->acceptChild(node) + "))");
    }

    void RAstGenerator::visit(UniopCsc *node) {
        this->setValue("(1/sin(" + this->acceptChild(node) + "))");
    }

    void RAstGenerator::visit(UniopCsch *node) {
        this->setValue("(1/sinh(" + this->acceptChild(node) + "))");
    }

    void RAstGenerator::visit(UniopArcsin *node) {
        this->setValue("asin(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArcsinh *node) {
        this->setValue("asinh(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArccos *node) {
        this->setValue("acos(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArccosh *node) {
        this->setValue("acosh(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArctan *node) {
        this->setValue("atan(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArctanh *node) {
        this->setValue("atanh(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArccot *node) {
        this->setValue("atan(1/" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArccoth *node) {
        this->setValue("atanh(1/" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArcsec *node) {
        this->setValue("acos(1/" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArcsech *node) {
        this->setValue("acosh(1/" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArccsc *node) {
        this->setValue("asin(1/" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopArccsch *node) {
        this->setValue("asinh(1/" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopHeaviside *node) {
        this->setValue("((sign(" + this->acceptChild(node) + ") + 1) / 2)");
    }

    void RAstGenerator::visit(UniopSign *node) {
        this->setValue("sign(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopFloor *node) {
        this->setValue("floor(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(UniopCeiling *node) {
        this->setValue("ceiling(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(ScalarInt *node) {
        this->setValue("(" + node->toString() + ")");
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
        
        this->setValue("(" + s + ")");
    }

    void RAstGenerator::visit(BinopPlus *node) {
        this->setValue("(" + this->acceptLeft(node) + " + " + this->acceptRight(node) + ")");
    }

    void RAstGenerator::visit(BinopMinus *node) {
        this->setValue("(" + this->acceptLeft(node) + " - " + this->acceptRight(node) + ")");
    }

    void RAstGenerator::visit(BinopDivide *node) {
        this->setValue("(" + this->acceptLeft(node) + " / " + this->acceptRight(node) + ")");
    }

    void RAstGenerator::visit(BinopTimes *node) {
        this->setValue("(" + this->acceptLeft(node) + " * " + this->acceptRight(node) + ")");
    }

    void RAstGenerator::visit(BinopPower *node) {
        this->setValue("(" + this->acceptLeft(node) + " ^ " + this->acceptRight(node) + ")");
    }

    void RAstGenerator::visit(BinopLogx *node) {
        this->setValue("log(" + this->acceptLeft(node) + ", base = " + this->acceptRight(node) + ")");
    }

    void RAstGenerator::visit(BinopRoot *node) {
        this->setValue("(" + this->acceptLeft(node) + " ^ (1/" + this->acceptRight(node) + "))");
    }

    void RAstGenerator::visit(BinopMin *node) {
        this->setValue("min(" + this->acceptLeft(node) + ", " + this->acceptRight(node) + ")");
    }

    void RAstGenerator::visit(BinopMax *node) {
        this->setValue("max(" + this->acceptLeft(node) + ", " + this->acceptRight(node) + ")");
    }

    void RAstGenerator::visit(BinopRem *node) {
        this->setValue("(" + this->acceptLeft(node) + " %% " + this->acceptRight(node) + ")");
    }

    void RAstGenerator::visit(BinopAtan2 *node) {
        this->setValue("atan2(" + this->acceptLeft(node) + ", " + this->acceptRight(node) + ")");
    }

    void RAstGenerator::visit(LogicUniopIsdefined *node) {
        this->setValue("!is.null(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(LogicUniopNot *node) {
        this->setValue("!(" + this->acceptChild(node) + ")");
    }

    void RAstGenerator::visit(LogicBinopLt *node) {
        this->setValue(this->infix(node, " < "));
    }

    void RAstGenerator::visit(LogicBinopLeq *node) {
        this->setValue(this->infix(node, " <= "));
    }

    void RAstGenerator::visit(LogicBinopGt *node) {
        this->setValue(this->infix(node, " > "));
    }

    void RAstGenerator::visit(LogicBinopGeq *node) {
        this->setValue(this->infix(node, " >= "));
    }

    void RAstGenerator::visit(LogicBinopEq *node) {
        this->setValue(this->infix(node, " == "));
    }

    void RAstGenerator::visit(LogicBinopNeq *node) {
        this->setValue(this->infix(node, " != "));
    }

    void RAstGenerator::visit(LogicBinopAnd *node) {
        this->setValue(this->infix(node, " && "));
    }

    void RAstGenerator::visit(LogicBinopOr *node) {
        this->setValue(this->infix(node, " || "));
    }

    void RAstGenerator::visit(LogicBinopXor *node) {
        this->setValue("((" + this->acceptLeft(node) + " || " + this->acceptRight(node) + ")" +
            " && !(" + this->acceptLeft(node) + " && " + this->acceptRight(node) + "))");
    }

    void RAstGenerator::visit(Vector *node) {
        std::vector<AstNode *> elements = node->getElements();
        std::string s = "c(";
        bool first = true;
        for (AstNode *element : elements) {
            if (first) {
                first = false;
            } else {
                s += ", ";
            }
            element->accept(this);
            s += this->getValue();
        }
        this->setValue(s + ")");
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
        this->setValue(s + std::string(pieces.size(), ')'));
    }

    void RAstGenerator::visit(Piece *node) {
        node->getCondition()->accept(this);
        std::string cond = this->getValue();
        node->getExpression()->accept(this);
        std::string expr = this->getValue();
        this->setValue(cond + ", " + expr);
    }

    void RAstGenerator::visit(LogicFalse *node) {
        this->setValue("(FALSE)");
    }

    void RAstGenerator::visit(LogicTrue *node) {
        this->setValue("(TRUE)");
    }

    void RAstGenerator::visit(Pi *node) {
        this->setValue("(pi)");
    }

    void RAstGenerator::visit(Exponentiale *node) {
        this->setValue("exp(1)");
    }

    void RAstGenerator::visit(NullValue *node) {
        this->setValue("NULL");
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
        this->setValue(this->getValue() + "(" + argument_list + ")");
    }

    void RAstGenerator::visit(FunctionArgument *node) {
        node->getArgument()->accept(this);
        this->setValue(node->getSymbId() + "=" + this->getValue());
    }

    void RAstGenerator::visit(Interval *node) {
        std::string result = "list(left=" + this->accept(node->getLeftEndpoint());
        result += ", right=" + this->accept(node->getRightEndpoint());
        result += ", openleft=" + this->getLogicLiteral(node->isLeftEndpointOpen());
        result += ", openright=" + this->getLogicLiteral(node->isRightEndpointOpen());
        result += ")";
        this->setValue(result);
    }
}
