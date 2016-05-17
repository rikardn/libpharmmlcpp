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

namespace PharmML
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
        this->functioncall = nullptr;
    }
    
    // Methods to get a single object (if complex, return a nullptr)
    SymbRef *AstAnalyzer::getPureSymbRef() {
        return this->symbref;
    }
    
    Scalar *AstAnalyzer::getPureScalar() {
        return this->scalar;
    }
    
    FunctionCall *AstAnalyzer::getPureFunctionCall() {
        return this->functioncall;
    }
    
    // visitor methods
    void AstAnalyzer::visit(SymbRef *node) {
        if (first_node) {
            this->symbref = node;
            this->first_node = false;
        }
        Symbol *symbol = node->getSymbol();
        this->setValue(":" + symbol->getSymbId() + ":");
    }

    void AstAnalyzer::visit(SteadyStateParameter *node) {
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(ColumnRef *node) {
        this->first_node = false;
    }

    void AstAnalyzer::visit(TargetMapping *node) {
        this->first_node = false;
    }

    void AstAnalyzer::visit(UniopLog *node) {
        node->getChild()->accept(this);
        this->setValue("log(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopLog2 *node) {
        node->getChild()->accept(this);
        this->setValue("log2(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopLog10 *node) {
        node->getChild()->accept(this);
        this->setValue("log10(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }

    void AstAnalyzer::visit(UniopExp *node) {
        node->getChild()->accept(this);
        this->setValue("exp(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }

    void AstAnalyzer::visit(UniopMinus *node) {
        node->getChild()->accept(this);
        this->setValue("-(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }

    void AstAnalyzer::visit(UniopAbs *node) {
        node->getChild()->accept(this);
        this->setValue("abs(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopSqrt *node) {
        node->getChild()->accept(this);
        this->setValue("sqrt(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopLogistic *node) {
        node->getChild()->accept(this);
        this->setValue("logistic(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopLogit *node) {
        node->getChild()->accept(this);
        this->setValue("logit(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopProbit *node) {
        node->getChild()->accept(this);
        this->setValue("probit(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopNormcdf *node) {
        node->getChild()->accept(this);
        this->setValue("normcdf(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopFactorial *node) {
        node->getChild()->accept(this);
        this->setValue("factorial(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopFactln *node) {
        node->getChild()->accept(this);
        this->setValue("factln(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopGamma *node) {
        node->getChild()->accept(this);
        this->setValue("gamma(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopGammaln *node) {
        node->getChild()->accept(this);
        this->setValue("gammaln(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopSin *node) {
        node->getChild()->accept(this);
        this->setValue("sin(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopSinh *node) {
        node->getChild()->accept(this);
        this->setValue("sinh(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopCos *node) {
        node->getChild()->accept(this);
        this->setValue("cos(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }

    void AstAnalyzer::visit(UniopCosh *node) {
        node->getChild()->accept(this);
        this->setValue("cosh(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopTan *node) {
        node->getChild()->accept(this);
        this->setValue("tan(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopTanh *node) {
        node->getChild()->accept(this);
        this->setValue("tanh(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopCot *node) {
        node->getChild()->accept(this);
        this->setValue("cot(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopCoth *node) {
        node->getChild()->accept(this);
        this->setValue("coth(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopSec *node) {
        node->getChild()->accept(this);
        this->setValue("sec(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopSech *node) {
        node->getChild()->accept(this);
        this->setValue("sech(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopCsc *node) {
        node->getChild()->accept(this);
        this->setValue("csc(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopCsch *node) {
        node->getChild()->accept(this);
        this->setValue("csch(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopArcsin *node) {
        node->getChild()->accept(this);
        this->setValue("arcsin(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopArcsinh *node) {
        node->getChild()->accept(this);
        this->setValue("arcsinh(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopArccos *node) {
        node->getChild()->accept(this);
        this->setValue("arccos(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopArccosh *node) {
        node->getChild()->accept(this);
        this->setValue("arccosh(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopArctan *node) {
        node->getChild()->accept(this);
        this->setValue("arctan(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopArctanh *node) {
        node->getChild()->accept(this);
        this->setValue("arctanh(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopArccot *node) {
        node->getChild()->accept(this);
        this->setValue("arccot(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopArccoth *node) {
        node->getChild()->accept(this);
        this->setValue("arccoth(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopArcsec *node) {
        node->getChild()->accept(this);
        this->setValue("arcsec(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopArcsech *node) {
        node->getChild()->accept(this);
        this->setValue("arcsech(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopArccsc *node) {
        node->getChild()->accept(this);
        this->setValue("arccsc(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopArccsch *node) {
        node->getChild()->accept(this);
        this->setValue("arccsch(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopHeaviside *node) {
        node->getChild()->accept(this);
        this->setValue("heaviside(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopSign *node) {
        node->getChild()->accept(this);
        this->setValue("sign(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopFloor *node) {
        node->getChild()->accept(this);
        this->setValue("floor(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(UniopCeiling *node) {
        node->getChild()->accept(this);
        this->setValue("ceiling(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }

    void AstAnalyzer::visit(ScalarInt *node) {
        if (this->first_node) {
            this->scalar = node;
            this->first_node = false;
        }
        this->setValue("int[" + node->toString() + "}");
    }

    void AstAnalyzer::visit(ScalarReal *node) {
        if (this->first_node) {
            this->scalar = node;
            this->first_node = false;
        }
        this->setValue("real[" + node->toString() + "]");
    }

    void AstAnalyzer::visit(BinopPlus *node) {
        this->setValue("plus(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }

    void AstAnalyzer::visit(BinopMinus *node) {
        this->setValue("minus(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }

    void AstAnalyzer::visit(BinopDivide *node) {
        this->setValue("divide(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }

    void AstAnalyzer::visit(BinopTimes *node) {
        this->setValue("times(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(BinopPower *node) {
        this->setValue("power(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(BinopLogx *node) {
        this->setValue("logx(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(BinopRoot *node) {
        this->setValue("root(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(BinopMin *node) {
        this->setValue("min(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(BinopMax *node) {
        this->setValue("max(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(BinopRem *node) {
        this->setValue("rem(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(BinopAtan2 *node) {
        this->setValue("atan2(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(LogicUniopIsdefined *node) {
        this->setValue("isDefined(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(LogicUniopNot *node) {
        this->setValue("not(" + this->acceptChild(node) + ")");
        this->first_node = false;
    }

    void AstAnalyzer::visit(LogicBinopLt *node) {
        this->setValue("lt(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }

    void AstAnalyzer::visit(LogicBinopLeq *node) {
        this->setValue("leq(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(LogicBinopGt *node) {
        this->setValue("gt(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(LogicBinopGeq *node) {
        this->setValue("geq(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(LogicBinopEq *node) {
        this->setValue("eq(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(LogicBinopNeq *node) {
        this->setValue("neq(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(LogicBinopAnd *node) {
        this->setValue("and(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(LogicBinopOr *node) {
        this->setValue("or(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(LogicBinopXor *node) {        
        this->setValue("xor(" + this->infix(node, ";") + ")");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(Vector *node) {
        std::string s;
        bool first = true;
        for (AstNode *cnode : node->getElements()) {
            if (first) {
                s += this->accept(cnode);
                first = false;
            } else {
                s += ";" + this->accept(cnode);
            }
        }
        this->setValue("vector[" + s + "]");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(Piecewise *node) {
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
        this->first_node = false;
    }

    void AstAnalyzer::visit(Piece *node) {
        std::string s;
        if (node->getExpression()) {
            s += this->accept(node->getExpression()) + ";";
        }
        s += "Condition[" + this->accept(node->getCondition()) + "]";
        this->setValue("Piece[" + s + "]");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(LogicFalse *node) {
        this->setValue("false");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(LogicTrue *node) {
        this->setValue("true");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(Pi *node) {
        this->setValue("pi");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(Exponentiale *node) {
        this->setValue("exponentiale");
        this->first_node = false;
    }
    
    void AstAnalyzer::visit(NullValue *node) {
        this->setValue("null");
        this->first_node = false;
    }

    void AstAnalyzer::visit(FunctionCall *node) {
        if (this->first_node) {
            this->functioncall = node;
            this->first_node = false;
        }
        std::string s = this->accept(node->getFunction()) + "(";
        bool first = true;
        for (FunctionArgument *argument : node->getFunctionArguments()) {
            if (first) {
                s += this->accept(argument);
            } else {
                s += ";" + this->accept(argument);
            }
        }
        this->setValue(s + ")");
    }

    void AstAnalyzer::visit(FunctionArgument *node) {
        this->setValue(":" + node->getSymbId() + ":=" + this->accept(node->getArgument()));
        this->first_node = false;
    }

    void AstAnalyzer::visit(Interval *node) {
        this->first_node = false;
    }
}
