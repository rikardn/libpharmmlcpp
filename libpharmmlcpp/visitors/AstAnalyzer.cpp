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

#include "AstAnalyzer.h"

namespace pharmmlcpp
{
    // private
    std::string AstAnalyzer::infix(Binop *binop, std::string op) {
        std::string result;
        binop->getLeft()->accept(this);
        result = this->getValue() + op;
        binop->getRight()->accept(this);
        result += this->getValue();
        return result;
    }

    std::string AstAnalyzer::acceptChild(Uniop *uniop) {
        uniop->getChild()->accept(this);
        return this->getValue();
    }

    std::string AstAnalyzer::accept(AstNode *node) {
        node->accept(this);
        return this->getValue();
    }

    // public
    AstAnalyzer::AstAnalyzer() {
        this->reset();
    }

    // Resets the internal state to that after construction
    void AstAnalyzer::reset() {
        this->first_node = true;
        this->symbref = nullptr;
        this->scalar = nullptr;
        this->scalar_int = nullptr;
        this->scalar_real = nullptr;
        this->scalar_bool = nullptr;
        this->scalar_string = nullptr;
        this->functioncall = nullptr;
        this->piecewise = nullptr;
        this->vector = nullptr;
        this->interval = nullptr;
        this->length = 1;
    }

    // Methods to get a single object (if complex, return a nullptr)
    SymbRef *AstAnalyzer::getPureSymbRef() {
        return this->symbref;
    }

    Scalar *AstAnalyzer::getPureScalar() {
        return this->scalar;
    }

    ScalarInt *AstAnalyzer::getPureScalarInt() {
        return this->scalar_int;
    }

    ScalarReal *AstAnalyzer::getPureScalarReal() {
        return this->scalar_real;
    }

    ScalarBool *AstAnalyzer::getPureScalarBool() {
        return this->scalar_bool;
    }

    ScalarString *AstAnalyzer::getPureScalarString() {
        return this->scalar_string;
    }

    FunctionCall *AstAnalyzer::getPureFunctionCall() {
        return this->functioncall;
    }

    Piecewise *AstAnalyzer::getPurePiecewise() {
        return this->piecewise;
    }

    Vector *AstAnalyzer::getPureVector() {
        return this->vector;
    }

    Interval *AstAnalyzer::getPureInterval() {
        return this->interval;
    }

    int AstAnalyzer::getLength() {
        return this->length;
    }

    bool AstAnalyzer::tryParsePureInt(AstNode *node, int &result) {
        this->reset();
        if (!node) {
            return false;
        }
        node->accept(this);
        pharmmlcpp::ScalarInt *scint = this->getPureScalarInt();
        if (!scint) {
            return false;
        }
        return (StringTools::isInt(scint->toString(), result));
    }

    // visitor methods
    void AstAnalyzer::visit(SymbRef *node) {
        if (first_node) {
            this->symbref = node;
            this->first_node = false;
        }
        Symbol *symbol = node->getSymbol();
        if (symbol) {
            this->setValue(":" + symbol->getSymbId() + ":");
        } else {
            this->setValue(":UNDEF:");
        }
    }

    void AstAnalyzer::visit(SteadyStateParameter *node) {
        this->first_node = false;
    }

    void AstAnalyzer::visit(ColumnRef *node) {
        this->first_node = false;
    }

    void AstAnalyzer::visit(UniopLog *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("log(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopLog2 *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("log2(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopLog10 *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("log10(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopExp *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("exp(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopMinus *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("-(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopAbs *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("abs(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopSqrt *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("sqrt(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopLogistic *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("logistic(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopLogit *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("logit(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopProbit *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("probit(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopNormcdf *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("normcdf(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopFactorial *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("factorial(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopFactln *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("factln(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopGamma *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("gamma(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopGammaln *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("gammaln(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopSin *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("sin(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopSinh *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("sinh(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopCos *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("cos(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopCosh *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("cosh(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopTan *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("tan(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopTanh *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("tanh(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopCot *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("cot(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopCoth *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("coth(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopSec *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("sec(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopSech *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("sech(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopCsc *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("csc(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopCsch *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("csch(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopArcsin *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("arcsin(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopArcsinh *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("arcsinh(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopArccos *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("arccos(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopArccosh *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("arccosh(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopArctan *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("arctan(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopArctanh *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("arctanh(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopArccot *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("arccot(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopArccoth *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("arccoth(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopArcsec *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("arcsec(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopArcsech *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("arcsech(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopArccsc *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("arccsc(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopArccsch *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("arccsch(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopHeaviside *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("heaviside(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopSign *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("sign(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopFloor *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("floor(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopCeiling *node) {
        this->first_node = false;
        node->getChild()->accept(this);
        this->setValue("ceiling(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(ScalarInt *node) {
        if (this->first_node) {
            this->scalar = node;
            this->scalar_int = node;
            this->first_node = false;
        }
        this->setValue("int[" + node->toString() + "}");
    }

    void AstAnalyzer::visit(ScalarReal *node) {
        if (this->first_node) {
            this->scalar = node;
            this->scalar_real = node;
            this->first_node = false;
        }
        this->setValue("real[" + node->toString() + "]");
    }

    void AstAnalyzer::visit(ScalarBool *node) {
        if (this->first_node) {
            this->scalar = node;
            this->scalar_bool = node;
            this->first_node = false;
        }
        if (node->toBool() == true) {
            this->setValue("true");
        } else {
            this->setValue("false");
        }
    }

    void AstAnalyzer::visit(ScalarString *node) {
        if (this->first_node) {
            this->scalar = node;
            this->scalar_string = node;
            this->first_node = false;
        }
        this->setValue("\"" + node->toString() + "\"");
    }

    void AstAnalyzer::visit(BinopPlus *node) {
        this->first_node = false;
        this->setValue("plus(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(BinopMinus *node) {
        this->first_node = false;
        this->setValue("minus(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(BinopDivide *node) {
        this->first_node = false;
        this->setValue("divide(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(BinopTimes *node) {
        this->first_node = false;
        this->setValue("times(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(BinopPower *node) {
        this->first_node = false;
        this->setValue("power(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(BinopLogx *node) {
        this->first_node = false;
        this->setValue("logx(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(BinopRoot *node) {
        this->first_node = false;
        this->setValue("root(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(BinopMin *node) {
        this->first_node = false;
        this->setValue("min(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(BinopMax *node) {
        this->first_node = false;
        this->setValue("max(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(BinopRem *node) {
        this->first_node = false;
        this->setValue("rem(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(BinopAtan2 *node) {
        this->first_node = false;
        this->setValue("atan2(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(LogicUniopIsdefined *node) {
        this->first_node = false;
        this->setValue("isDefined(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(LogicUniopNot *node) {
        this->first_node = false;
        this->setValue("not(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(LogicBinopLt *node) {
        this->first_node = false;
        this->setValue("lt(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(LogicBinopLeq *node) {
        this->first_node = false;
        this->setValue("leq(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(LogicBinopGt *node) {
        this->first_node = false;
        this->setValue("gt(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(LogicBinopGeq *node) {
        this->first_node = false;
        this->setValue("geq(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(LogicBinopEq *node) {
        this->first_node = false;
        this->setValue("eq(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(LogicBinopNeq *node) {
        this->first_node = false;
        this->setValue("neq(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(LogicBinopAnd *node) {
        this->first_node = false;
        this->setValue("and(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(LogicBinopOr *node) {
        this->first_node = false;
        this->setValue("or(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(LogicBinopXor *node) {
        this->first_node = false;
        this->setValue("xor(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(Vector *node) {
        if (first_node) {
            this->vector = node;
            this->first_node = false;
        }
        std::string s;
        bool first = true;
        for (std::shared_ptr<AstNode> cnode : node->getElements()) {
            if (first) {
                s += this->accept(cnode.get());
                first = false;
            } else {
                s += ";" + this->accept(cnode.get());
            }
        }
        this->length = node->getElements().size();  // Length of the vector
        this->setValue("vector[" + s + "]");
    }

    void AstAnalyzer::visit(Piecewise *node) {
        if (first_node) {
            this->piecewise = node;
            this->first_node = false;
        }
        std::string s;
        bool first = true;
        for (Piece *piece : node->getPieces()) {
            if (first) {
                s += this->accept(piece);
                first = false;
            } else {
                s += ";" + this->accept(piece);
            }
        }
        this->setValue("Piecewise[" + s + "]");
    }

    void AstAnalyzer::visit(Piece *node) {
        this->first_node = false;
        std::string s;
        if (node->getExpression()) {
            s += this->accept(node->getExpression()) + ";";
        }
        if (!node->isOtherwise()) {
            s += "Condition[" + this->accept(node->getCondition()) + "]";
            this->setValue("Piece[" + s + "]");
        } else {
            this->setValue("Otherwise[" + s + "]");
        }
    }

    void AstAnalyzer::visit(Pi *node) {
        this->first_node = false;
        this->setValue("pi");
    }

    void AstAnalyzer::visit(Exponentiale *node) {
        this->first_node = false;
        this->setValue("exponentiale");
    }

    void AstAnalyzer::visit(NullValue *node) {
        this->first_node = false;
        this->setValue("null");
    }

    void AstAnalyzer::visit(FunctionCall *node) {
        if (this->first_node) {
            this->functioncall = node;
            this->first_node = false;
        }
        std::string s = this->accept(node->getFunction()) + "(";
        bool first = true;
        for (auto const &argument : node->getFunctionArguments()) {
            if (first) {
                s += this->accept(argument.get());
            } else {
                s += ";" + this->accept(argument.get());
            }
        }
        this->setValue(s + ")");
    }

    void AstAnalyzer::visit(FunctionArgument *node) {
        this->first_node = false;
        this->setValue(":" + node->getSymbId() + ":=" + this->accept(node->getArgument()));
    }

    void AstAnalyzer::visit(Interval *node) {
        if (first_node) {
            this->interval = node;
            this->first_node = false;
        }
    }
}
