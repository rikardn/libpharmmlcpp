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
    void AstAnalyzer::setPureState() {
        this->pure_symbref = true;
        this->pure_scalar = true;
        this->pure_functioncall = true;
    }
    
    void AstAnalyzer::clearPureState() {
        this->pure_symbref = false;
        this->pure_scalar = false;
        this->pure_functioncall = false;
    }

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
        this->setPureState();
    }
    
    // Methods to get a single object (if complex, return a nullptr)
    SymbRef *AstAnalyzer::getPureSymbRef() {
        if (pure_symbref) {
            return this->symbref;
        } else {
            return nullptr;
        }
    }
    
    Scalar *AstAnalyzer::getPureScalar() {
        if (pure_scalar) {
            return this->scalar;
        } else {
            return nullptr;
        }
    }
    
    FunctionCall *AstAnalyzer::getPureFunctionCall() {
        if (pure_functioncall) {
            return this->functioncall;
        } else {
            return nullptr;
        }
    }
    
    // visitor methods
    void AstAnalyzer::visit(SymbRef *node) {
        this->pure_scalar = false;
        this->pure_functioncall = false;
        this->symbref = node;
        
        Symbol *symbol = node->getSymbol();
        this->setValue(":" + symbol->getSymbId() + ":");
    }

    void AstAnalyzer::visit(SteadyStateParameter *node) {
        this->clearPureState();
    }
    
    void AstAnalyzer::visit(ColumnRef *node) {
        this->clearPureState();
    }

    void AstAnalyzer::visit(TargetMapping *node) {
        this->clearPureState();
    }

    void AstAnalyzer::visit(UniopLog *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("log(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopLog2 *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("log2(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopLog10 *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("log10(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopExp *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("exp(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopMinus *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("-(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopAbs *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("abs(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopSqrt *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("sqrt(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopLogistic *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("logistic(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopLogit *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("logit(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopProbit *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("probit(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopNormcdf *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("normcdf(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopFactorial *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("factorial(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopFactln *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("factln(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopGamma *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("gamma(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopGammaln *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("gammaln(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopSin *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("sin(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopSinh *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("sinh(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopCos *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("cos(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(UniopCosh *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("cosh(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopTan *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("tan(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopTanh *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("tanh(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopCot *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("cot(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopCoth *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("coth(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopSec *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("sec(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopSech *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("sech(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopCsc *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("csc(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopCsch *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("csch(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopArcsin *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("arcsin(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopArcsinh *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("arcsinh(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopArccos *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("arccos(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopArccosh *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("arccosh(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopArctan *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("arctan(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopArctanh *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("arctanh(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopArccot *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("arccot(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopArccoth *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("arccoth(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopArcsec *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("arcsec(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopArcsech *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("arcsech(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopArccsc *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("arccsc(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopArccsch *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("arccsch(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopHeaviside *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("heaviside(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopSign *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("sign(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopFloor *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("floor(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(UniopCeiling *node) {
        this->clearPureState();
        node->getChild()->accept(this);
        
        this->setValue("ceiling(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(ScalarInt *node) {
        this->pure_symbref = false;
        this->pure_functioncall = false;
        this->scalar = node;
        
        this->setValue("int[" + node->toString() + "}");
    }

    void AstAnalyzer::visit(ScalarReal *node) {
        this->pure_symbref = false;
        this->pure_functioncall = false;
        this->scalar = node;
        
        this->setValue("real[" + node->toString() + "]");
    }

    void AstAnalyzer::visit(BinopPlus *node) {
        this->clearPureState();
        
        this->setValue("plus(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(BinopMinus *node) {
        this->clearPureState();
        
        this->setValue("minus(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(BinopDivide *node) {
        this->clearPureState();
        
        this->setValue("divide(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(BinopTimes *node) {
        this->clearPureState();
        
        this->setValue("times(" + this->infix(node, ";") + ")");
    }
    
    void AstAnalyzer::visit(BinopPower *node) {
        this->clearPureState();
        
        this->setValue("power(" + this->infix(node, ";") + ")");
    }
    
    void AstAnalyzer::visit(BinopLogx *node) {
        this->clearPureState();
        
        this->setValue("logx(" + this->infix(node, ";") + ")");
    }
    
    void AstAnalyzer::visit(BinopRoot *node) {
        this->clearPureState();
        
        this->setValue("root(" + this->infix(node, ";") + ")");
    }
    
    void AstAnalyzer::visit(BinopMin *node) {
        this->clearPureState();
        
        this->setValue("min(" + this->infix(node, ";") + ")");
    }
    
    void AstAnalyzer::visit(BinopMax *node) {
        this->clearPureState();
        
        this->setValue("max(" + this->infix(node, ";") + ")");
    }
    
    void AstAnalyzer::visit(BinopRem *node) {
        this->clearPureState();
        
        this->setValue("rem(" + this->infix(node, ";") + ")");
    }
    
    void AstAnalyzer::visit(BinopAtan2 *node) {
        this->clearPureState();
        
        this->setValue("atan2(" + this->infix(node, ";") + ")");
    }
    
    void AstAnalyzer::visit(LogicUniopIsdefined *node) {
        this->clearPureState();
        
        this->setValue("isDefined(" + this->acceptChild(node) + ")");
    }
    
    void AstAnalyzer::visit(LogicUniopNot *node) {
        this->clearPureState();
        
        this->setValue("not(" + this->acceptChild(node) + ")");
    }

    void AstAnalyzer::visit(LogicBinopLt *node) {
        this->clearPureState();
        
        this->setValue("lt(" + this->infix(node, ";") + ")");
    }

    void AstAnalyzer::visit(LogicBinopLeq *node) {
        this->clearPureState();
        
        this->setValue("leq(" + this->infix(node, ";") + ")");
    }
    
    void AstAnalyzer::visit(LogicBinopGt *node) {
        this->clearPureState();
        
        this->setValue("gt(" + this->infix(node, ";") + ")");
    }
    
    void AstAnalyzer::visit(LogicBinopGeq *node) {
        this->clearPureState();
        
        this->setValue("geq(" + this->infix(node, ";") + ")");
    }
    
    void AstAnalyzer::visit(LogicBinopEq *node) {
        this->clearPureState();
        
        this->setValue("eq(" + this->infix(node, ";") + ")");
    }
    
    void AstAnalyzer::visit(LogicBinopNeq *node) {
        this->clearPureState();
        
        this->setValue("neq(" + this->infix(node, ";") + ")");
    }
    
    void AstAnalyzer::visit(LogicBinopAnd *node) {
        this->clearPureState();
        
        this->setValue("and(" + this->infix(node, ";") + ")");
    }
    
    void AstAnalyzer::visit(LogicBinopOr *node) {
        this->clearPureState();
        
        this->setValue("or(" + this->infix(node, ";") + ")");
    }
    
    void AstAnalyzer::visit(LogicBinopXor *node) {
        this->clearPureState();
        
        this->setValue("xor(" + this->infix(node, ";") + ")");
    }
    
    void AstAnalyzer::visit(Vector *node) {
        this->clearPureState();
        
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
    }
    
    void AstAnalyzer::visit(Piecewise *node) {
        this->clearPureState();
        
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
        this->clearPureState();
        
        std::string s;
        if (node->getExpression()) {
            s += this->accept(node->getExpression()) + ";";
        }
        s += "Condition[" + this->accept(node->getCondition()) + "]";
        this->setValue("Piece[" + s + "]");
    }
    
    void AstAnalyzer::visit(LogicFalse *node) {
        this->clearPureState();
        
        this->setValue("false");
    }
    
    void AstAnalyzer::visit(LogicTrue *node) {
        this->clearPureState();
        
        this->setValue("true");
    }
    
    void AstAnalyzer::visit(Pi *node) {
        this->clearPureState();
        
        this->setValue("pi");
    }
    
    void AstAnalyzer::visit(Exponentiale *node) {
        this->clearPureState();
        
        this->setValue("exponentiale");
    }
    
    void AstAnalyzer::visit(NullValue *node) {
        this->clearPureState();
        
        this->setValue("null");
    }

    void AstAnalyzer::visit(FunctionCall *node) {
        this->pure_symbref = false;
        this->pure_scalar = false;
        this->functioncall = node;
        
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
        this->clearPureState();
        
        this->setValue(":" + node->getSymbId() + ":=" + this->accept(node->getArgument()));
    }

    void AstAnalyzer::visit(Interval *node) {
        this->clearPureState();
    }
}
