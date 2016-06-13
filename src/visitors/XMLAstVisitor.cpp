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

#include "XMLAstVisitor.h"

namespace pharmmlcpp
{
    // private
    void XMLAstVisitor::setValue(xml::Node node) {
        this->value = node;
    }

    xml::Node XMLAstVisitor::acceptLeft(Binop *binop) {
        binop->getLeft()->accept(this);
        return this->getValue();
    }

    xml::Node XMLAstVisitor::acceptRight(Binop *binop) {
        binop->getRight()->accept(this);
        return this->getValue();
    }

    xml::Node XMLAstVisitor::acceptChild(Uniop *uniop) {
        uniop->getChild()->accept(this);
        return this->getValue();
    }

    xml::Node XMLAstVisitor::accept(AstNode *node) {
        node->accept(this);
        return this->getValue();
    }

    void XMLAstVisitor::createUniop(Uniop *node, std::string op) {
        xml::Node uniop("Uniop", xml::Namespace::math);
        uniop.setAttribute("op", op);
        uniop.addChild(this->accept(node->getChild()));
        this->setValue(uniop);
    }

    void XMLAstVisitor::createLogicUniop(LogicUniop *node, std::string op) {
        xml::Node uniop("LogicUniop", xml::Namespace::math);
        uniop.setAttribute("op", op);
        uniop.addChild(this->accept(node->getChild()));
        this->setValue(uniop);
    }

    void XMLAstVisitor::createBinop(Binop *node, std::string op) {
        xml::Node binop("Binop", xml::Namespace::math);
        binop.setAttribute("op", op);
        binop.addChild(this->accept(node->getLeft()));
        binop.addChild(this->accept(node->getRight()));
        this->setValue(binop);
    }

    void XMLAstVisitor::createLogicBinop(Binop *node, std::string op) {
        xml::Node binop("LogicBinop", xml::Namespace::math);
        binop.setAttribute("op", op);
        binop.addChild(this->accept(node->getLeft()));
        binop.addChild(this->accept(node->getRight()));
        this->setValue(binop);
    }

    // public
    xml::Node XMLAstVisitor::getValue() {
        return this->value;
    }

    void XMLAstVisitor::visit(SymbRef *node) {
        xml::Node symbref("SymbRef", xml::Namespace::ct);
        if (!node->getBlkIdRef().empty()) {
            symbref.setAttribute("blkIdRef", node->getBlkIdRef());
        }
        symbref.setAttribute("symbIdRef", node->getSymbIdRef());
        this->setValue(symbref);
    }

    void XMLAstVisitor::visit(SteadyStateParameter *node) {
    }

    void XMLAstVisitor::visit(ColumnRef *node) {
        xml::Node ref("ColumnRef", xml::Namespace::ds);
        ref.setAttribute("columnIdRef", node->toString());
        this->setValue(ref);
    }

    void XMLAstVisitor::visit(UniopLog *node) {
        this->createUniop(node, "log");
    }

    void XMLAstVisitor::visit(UniopLog2 *node) {
        this->createUniop(node, "log2");
    }

    void XMLAstVisitor::visit(UniopLog10 *node) {
        this->createUniop(node, "log10");
    }

    void XMLAstVisitor::visit(UniopExp *node) {
        this->createUniop(node, "exp");
    }

    void XMLAstVisitor::visit(UniopMinus *node) {
        this->createUniop(node, "minus");
    }

    void XMLAstVisitor::visit(UniopAbs *node) {
        this->createUniop(node, "abs");
    }

    void XMLAstVisitor::visit(UniopSqrt *node) {
        this->createUniop(node, "sqrt");
    }

    void XMLAstVisitor::visit(UniopLogistic *node) {
        this->createUniop(node, "logistic");
    }

    void XMLAstVisitor::visit(UniopLogit *node) {
        this->createUniop(node, "logit");
    }

    void XMLAstVisitor::visit(UniopProbit *node) {
        this->createUniop(node, "probit");
    }

    void XMLAstVisitor::visit(UniopNormcdf *node) {
        this->createUniop(node, "normcdf");
    }

    void XMLAstVisitor::visit(UniopFactorial *node) {
        this->createUniop(node, "factorial");
    }

    void XMLAstVisitor::visit(UniopFactln *node) {
        this->createUniop(node, "factln");
    }

    void XMLAstVisitor::visit(UniopGamma *node) {
        this->createUniop(node, "gamma");
    }

    void XMLAstVisitor::visit(UniopGammaln *node) {
        this->createUniop(node, "gammaln");
    }

    void XMLAstVisitor::visit(UniopSin *node) {
        this->createUniop(node, "sin");
    }

    void XMLAstVisitor::visit(UniopSinh *node) {
        this->createUniop(node, "sinh");
    }

    void XMLAstVisitor::visit(UniopCos *node) {
        this->createUniop(node, "cos");
    }

    void XMLAstVisitor::visit(UniopCosh *node) {
        this->createUniop(node, "cosh");
    }

    void XMLAstVisitor::visit(UniopTan *node) {
        this->createUniop(node, "tan");
    }

    void XMLAstVisitor::visit(UniopTanh *node) {
        this->createUniop(node, "tanh");
    }

    void XMLAstVisitor::visit(UniopCot *node) {
        this->createUniop(node, "cot");
    }

    void XMLAstVisitor::visit(UniopCoth *node) {
        this->createUniop(node, "coth");
    }

    void XMLAstVisitor::visit(UniopSec *node) {
        this->createUniop(node, "sec");
    }

    void XMLAstVisitor::visit(UniopSech *node) {
        this->createUniop(node, "sech");
    }

    void XMLAstVisitor::visit(UniopCsc *node) {
        this->createUniop(node, "csc");
    }

    void XMLAstVisitor::visit(UniopCsch *node) {
        this->createUniop(node, "csch");
    }

    void XMLAstVisitor::visit(UniopArcsin *node) {
        this->createUniop(node, "arcsin");
    }

    void XMLAstVisitor::visit(UniopArcsinh *node) {
        this->createUniop(node, "arcsinh");
    }

    void XMLAstVisitor::visit(UniopArccos *node) {
        this->createUniop(node, "arccos");
    }

    void XMLAstVisitor::visit(UniopArccosh *node) {
        this->createUniop(node, "arccosh");
    }

    void XMLAstVisitor::visit(UniopArctan *node) {
        this->createUniop(node, "arctan");
    }

    void XMLAstVisitor::visit(UniopArctanh *node) {
        this->createUniop(node, "arctanh");
    }

    void XMLAstVisitor::visit(UniopArccot *node) {
        this->createUniop(node, "arccot");
    }

    void XMLAstVisitor::visit(UniopArccoth *node) {
        this->createUniop(node, "arccoth");
    }

    void XMLAstVisitor::visit(UniopArcsec *node) {
        this->createUniop(node, "arcsec");
    }

    void XMLAstVisitor::visit(UniopArcsech *node) {
        this->createUniop(node, "arcsech");
    }

    void XMLAstVisitor::visit(UniopArccsc *node) {
        this->createUniop(node, "arccsc");
    }

    void XMLAstVisitor::visit(UniopArccsch *node) {
        this->createUniop(node, "arccsch");
    }

    void XMLAstVisitor::visit(UniopHeaviside *node) {
        this->createUniop(node, "heaviside");
    }

    void XMLAstVisitor::visit(UniopSign *node) {
        this->createUniop(node, "sign");
    }

    void XMLAstVisitor::visit(UniopFloor *node) {
        this->createUniop(node, "floor");
    }

    void XMLAstVisitor::visit(UniopCeiling *node) {
        this->createUniop(node, "ceiling");
    }

    void XMLAstVisitor::visit(ScalarInt *node) {
        xml::Node scalar_int("Int", xml::Namespace::ct);
        scalar_int.setContent(node->toString());
        this->setValue(scalar_int);
    }

    void XMLAstVisitor::visit(ScalarReal *node) {
        xml::Node scalar_real("Real", xml::Namespace::ct);
        scalar_real.setContent(node->toString());
        this->setValue(scalar_real);
    }

    void XMLAstVisitor::visit(ScalarBool *node) {
        std::string name;
        if (node->toBool() == true) {
            name = "True";
        } else {
            name = "False";
        }
        xml::Node scalar_bool(name, xml::Namespace::ct);
        this->setValue(scalar_bool);
    }

    void XMLAstVisitor::visit(ScalarString *node) {
        xml::Node scalar_string("String", xml::Namespace::ct);
        scalar_string.setContent(node->toString());
        this->setValue(scalar_string);
    }

    void XMLAstVisitor::visit(BinopPlus *node) {
        this->createBinop(node, "plus");
    }

    void XMLAstVisitor::visit(BinopMinus *node) {
        this->createBinop(node, "minus");
    }

    void XMLAstVisitor::visit(BinopDivide *node) {
        this->createBinop(node, "divide");
    }

    void XMLAstVisitor::visit(BinopTimes *node) {
        this->createBinop(node, "node");
    }

    void XMLAstVisitor::visit(BinopPower *node) {
        this->createBinop(node, "power");
    }

    void XMLAstVisitor::visit(BinopLogx *node) {
        this->createBinop(node, "logx");
    }

    void XMLAstVisitor::visit(BinopRoot *node) {
        this->createBinop(node, "root");
    }

    void XMLAstVisitor::visit(BinopMin *node) {
        this->createBinop(node, "min");
    }

    void XMLAstVisitor::visit(BinopMax *node) {
        this->createBinop(node, "max");
    }

    void XMLAstVisitor::visit(BinopRem *node) {
        this->createBinop(node, "rem");
    }

    void XMLAstVisitor::visit(BinopAtan2 *node) {
        this->createBinop(node, "atan2");
    }

    void XMLAstVisitor::visit(LogicUniopIsdefined *node) {
        this->createLogicUniop(node, "isDefined");
    }

    void XMLAstVisitor::visit(LogicUniopNot *node) {
        this->createLogicUniop(node, "not");
    }

    void XMLAstVisitor::visit(LogicBinopLt *node) {
        this->createLogicBinop(node, "lt");
    }

    void XMLAstVisitor::visit(LogicBinopLeq *node) {
        this->createLogicBinop(node, "leq");
    }

    void XMLAstVisitor::visit(LogicBinopGt *node) {
        this->createLogicBinop(node, "gt");
    }

    void XMLAstVisitor::visit(LogicBinopGeq *node) {
        this->createLogicBinop(node, "geq");
    }

    void XMLAstVisitor::visit(LogicBinopEq *node) {
        this->createLogicBinop(node, "eq");
    }

    void XMLAstVisitor::visit(LogicBinopNeq *node) {
        this->createLogicBinop(node, "neq");
    }

    void XMLAstVisitor::visit(LogicBinopAnd *node) {
        this->createLogicBinop(node, "and");
    }

    void XMLAstVisitor::visit(LogicBinopOr *node) {
        this->createLogicBinop(node, "or");
    }

    void XMLAstVisitor::visit(LogicBinopXor *node) {
        this->createLogicBinop(node, "xor");
    }

    void XMLAstVisitor::visit(Vector *node) {
    }

    void XMLAstVisitor::visit(Piecewise *node) {
    }

    void XMLAstVisitor::visit(Piece *node) {
    }

    void XMLAstVisitor::visit(Pi *node) {
    }

    void XMLAstVisitor::visit(Exponentiale *node) {
    }

    void XMLAstVisitor::visit(NullValue *node) {
    }

    void XMLAstVisitor::visit(FunctionCall *node) {
    }

    void XMLAstVisitor::visit(FunctionArgument *node) {
    }

    void XMLAstVisitor::visit(Interval *node) {
        xml::Node interval("Interval", xml::Namespace::ct);
        xml::Node le("LeftEndpoint", xml::Namespace::ct);
        if (node->isLeftEndpointOpen()) {
            le.setAttribute("type", "open");
        }
        xml::Node re("RightEndpoint", xml::Namespace::ct);
        if (node->isRightEndpointOpen()) {
            re.setAttribute("type", "open");
        }
        interval.addChild(le);
        interval.addChild(re);
        xml::Node le_assign("Assign", xml::Namespace::ct);
        le.addChild(le_assign);
        xml::Node re_assign("Assign", xml::Namespace::ct);
        re.addChild(re_assign);
        le_assign.addChild(this->accept(node->getLeftEndpoint()));
        re_assign.addChild(this->accept(node->getRightEndpoint()));
        this->setValue(interval);
    }
}
