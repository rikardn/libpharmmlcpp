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

#include "MDLAstGenerator.h"
#include <generators/TextFormatter.h>

namespace pharmmlcpp
{
    // Parenthesize from a (root) node, accept it and return string
    std::string MDLAstGenerator::acceptRoot(AstNode *node) {
        node->accept(&this->parenthesizer);
        return this->accept(node);
    }

    // Set string value and parenthesize it if required
    void MDLAstGenerator::setParenthesizedValue(AstNode *node, std::string str) {
        if (node->hasParentheses()) {
            str = "(" + str + ")";
        }
        this->value = str;
    }

    // private
    void MDLAstGenerator::setValue(std::string str) {
        this->value = str;
    }

    /// Set category flag after visit to CatRef object (MDL syntax is: "SYMBREF = SYMBREF.CATREF")
    void MDLAstGenerator::setCategoryFlag() {
        this->category = true;
    }

    void MDLAstGenerator::clearCategoryFlag() {
        this->category = false;
    }

    std::string MDLAstGenerator::acceptLeft(Binop *binop) {
        binop->getLeft()->accept(this);
        return this->getValue();
    }

    std::string MDLAstGenerator::acceptRight(Binop *binop) {
        binop->getRight()->accept(this);
        return this->getValue();
    }

    std::string MDLAstGenerator::infix(Binop *binop, std::string op) {
        binop->getLeft()->accept(this);
        std::string lnode = this->getValue();
        binop->getRight()->accept(this);
        std::string rnode = this->getValue();
        if (binop->hasParentheses()) {
            return "(" + lnode + op + rnode + ")";
        } else {
            return lnode + op + rnode;
        }
    }

    std::string MDLAstGenerator::acceptChild(Uniop *uniop) {
        uniop->getChild()->accept(this);
        return this->getValue();
    }

    std::string MDLAstGenerator::getLogicLiteral(bool value) {
        if (value) {
            return "true";
        } else {
            return "false";
        }
    }

    // public
    MDLAstGenerator::MDLAstGenerator(std::shared_ptr<Logger> logger) {
        this->logger = logger;
        // MDL requires conditions to always be parenthesized
        this->parenthesizer.forceParenthesizedConditions();
    }

    std::string MDLAstGenerator::getValue() {
        return this->value;
    }

    /// Get category flag after visit to CatRef object (MDL syntax is: "SYMBREF = SYMBREF.CATREF")
    bool MDLAstGenerator::getCategoryFlag() {
        return this->category;
    }

    std::string MDLAstGenerator::accept(AstNode *node) {
        node->accept(this);
        return this->getValue();
    }

    void MDLAstGenerator::visit(SymbRef *node) {
        Symbol *symbol = node->getSymbol();
        if (symbol != nullptr) {
            this->setValue(symbol->getName());
        } else {
            this->setValue("UNDEF");
        }
    }

    void MDLAstGenerator::visit(SteadyStateParameter *node) {
        node->getSymbRef()->accept(this);
        std::string symbref = this->getValue();
        node->getAssignment()->accept(this);
        std::string assignment = this->getValue();
        this->setValue(symbref + " = " + assignment);
    }

    void MDLAstGenerator::visit(ColumnRef *node) {
        this->setValue(node->toString());
    }

    void MDLAstGenerator::visit(CatRef *node) {
        // FIXME: Implement Category object and reference system (Category in Categorical in Covariate)
        this->setCategoryFlag();
        this->setValue(node->getCatRef());
    }

    void MDLAstGenerator::visit(UniopLog *node) {
        this->setValue("ln(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopLog2 *node) {
        this->setValue("log2(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopLog10 *node) {
        this->setValue("log10(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopExp *node) {
        this->setValue("exp(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopMinus *node) {
        this->setValue("(-" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopAbs *node) {
        this->setValue("abs(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopSqrt *node) {
        this->setValue("sqrt(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopLogistic *node) {
        this->setValue("invLogit(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopLogit *node) {
        this->setValue("logit(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopProbit *node) {
        this->setValue("qnorm(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopNormcdf *node) {
        this->setValue("pnorm(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopFactorial *node) {
        this->setValue("factorial(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopFactln *node) {
        this->setValue("lfactorial(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopGamma *node) {
        this->setValue("gamma(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopGammaln *node) {
        this->setValue("lgamma(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopSin *node) {
        this->setValue("sin(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopSinh *node) {
        this->setValue("sinh(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopCos *node) {
        this->setValue("cos(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopCosh *node) {
        this->setValue("cosh(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopTan *node) {
        this->setValue("tan(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopTanh *node) {
        this->setValue("tanh(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopCot *node) {
        this->setValue("(1/tan(" + this->acceptChild(node) + "))");
    }

    void MDLAstGenerator::visit(UniopCoth *node) {
        this->setValue("(1/tanh(" + this->acceptChild(node) + "))");
    }

    void MDLAstGenerator::visit(UniopSec *node) {
        this->setValue("(1/cos(" + this->acceptChild(node) + "))");
    }

    void MDLAstGenerator::visit(UniopSech *node) {
        this->setValue("(1/cosh(" + this->acceptChild(node) + "))");
    }

    void MDLAstGenerator::visit(UniopCsc *node) {
        this->setValue("(1/sin(" + this->acceptChild(node) + "))");
    }

    void MDLAstGenerator::visit(UniopCsch *node) {
        this->setValue("(1/sinh(" + this->acceptChild(node) + "))");
    }

    void MDLAstGenerator::visit(UniopArcsin *node) {
        this->setValue("asin(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopArcsinh *node) {
        this->setValue("asinh(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopArccos *node) {
        this->setValue("acos(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopArccosh *node) {
        this->setValue("acosh(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopArctan *node) {
        this->setValue("atan(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopArctanh *node) {
        this->setValue("atanh(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopArccot *node) {
        this->setValue("atan(1/" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopArccoth *node) {
        this->setValue("atanh(1/" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopArcsec *node) {
        this->setValue("acos(1/" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopArcsech *node) {
        this->setValue("acosh(1/" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopArccsc *node) {
        this->setValue("asin(1/" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopArccsch *node) {
        this->setValue("asinh(1/" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopHeaviside *node) {
        this->setValue("((sign(" + this->acceptChild(node) + ") + 1) / 2)");
    }

    void MDLAstGenerator::visit(UniopSign *node) {
        this->setValue("sign(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopFloor *node) {
        this->setValue("floor(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(UniopCeiling *node) {
        this->setValue("ceiling(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(ScalarInt *node) {
        this->setParenthesizedValue(node, node->toString());
    }

    void MDLAstGenerator::visit(ScalarReal *node) {
        std::string s = node->toString();
        s.erase (s.find_last_not_of('0') + 1, std::string::npos);
        this->setParenthesizedValue(node, s);
    }

    void MDLAstGenerator::visit(ScalarBool *node) {
        this->setParenthesizedValue(node, this->getLogicLiteral(node->toBool()));
    }

    void MDLAstGenerator::visit(ScalarString *node) {
        this->setParenthesizedValue(node, "\"" + node->toString() + "\"");
    }

    void MDLAstGenerator::visit(BinopPlus *node) {
        this->setParenthesizedValue(node, this->acceptLeft(node) + " + " + this->acceptRight(node));
    }

    void MDLAstGenerator::visit(BinopMinus *node) {
        this->setParenthesizedValue(node, this->acceptLeft(node) + " - " + this->acceptRight(node));
    }

    void MDLAstGenerator::visit(BinopDivide *node) {
        this->setParenthesizedValue(node, this->acceptLeft(node) + " / " + this->acceptRight(node));
    }

    void MDLAstGenerator::visit(BinopTimes *node) {
        this->setParenthesizedValue(node, this->acceptLeft(node) + " * " + this->acceptRight(node));
    }

    void MDLAstGenerator::visit(BinopPower *node) {
        this->setValue("(" + this->acceptLeft(node) + " ^ " + this->acceptRight(node) + ")");
    }

    void MDLAstGenerator::visit(BinopLogx *node) {
        this->setValue("ln((" + this->acceptLeft(node) + ") / ln(" + this->acceptRight(node) + "))");
    }

    void MDLAstGenerator::visit(BinopRoot *node) {
        this->setValue("(" + this->acceptLeft(node) + " ^ (1/" + this->acceptRight(node) + "))");
    }

    void MDLAstGenerator::visit(BinopMin *node) {
        this->setValue("min(" + this->acceptLeft(node) + ", " + this->acceptRight(node) + ")");
    }

    void MDLAstGenerator::visit(BinopMax *node) {
        this->setValue("max(" + this->acceptLeft(node) + ", " + this->acceptRight(node) + ")");
    }

    void MDLAstGenerator::visit(BinopRem *node) {
        this->setValue("(" + this->acceptLeft(node) + " mod " + this->acceptRight(node) + ")");
    }

    void MDLAstGenerator::visit(BinopAtan2 *node) {
        this->setValue("atan2(" + this->acceptLeft(node) + ", " + this->acceptRight(node) + ")");
    }

    void MDLAstGenerator::visit(LogicUniopIsdefined *node) {
        this->setValue("isDefined(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(LogicUniopNot *node) {
        this->setValue("!(" + this->acceptChild(node) + ")");
    }

    void MDLAstGenerator::visit(LogicBinopLt *node) {
        this->setValue(this->infix(node, " < "));
    }

    void MDLAstGenerator::visit(LogicBinopLeq *node) {
        this->setValue(this->infix(node, " <= "));
    }

    void MDLAstGenerator::visit(LogicBinopGt *node) {
        this->setValue(this->infix(node, " > "));
    }

    void MDLAstGenerator::visit(LogicBinopGeq *node) {
        this->setValue(this->infix(node, " >= "));
    }

    void MDLAstGenerator::visit(LogicBinopEq *node) {
        // Special since one node might be a CatRef, i.e. a Category reference (e.g. "female")
        this->clearCategoryFlag();
        std::string lnode = this->acceptLeft(node);
        bool lnode_is_cat = this->getCategoryFlag();

        this->clearCategoryFlag();
        std::string rnode = this->acceptRight(node);
        bool rnode_is_cat = this->getCategoryFlag();

        if (lnode_is_cat && !rnode_is_cat) {
            this->setParenthesizedValue(node, rnode + "." + lnode + " == " + rnode);
        } else if (!lnode_is_cat && rnode_is_cat) {
            this->setParenthesizedValue(node, lnode + " == " + lnode + "." + rnode);
        } else {
            this->setParenthesizedValue(node, lnode + " == " + rnode);
        }
    }

    void MDLAstGenerator::visit(LogicBinopNeq *node) {
        this->setValue(this->infix(node, " != "));
    }

    void MDLAstGenerator::visit(LogicBinopAnd *node) {
        this->setValue(this->infix(node, " && "));
    }

    void MDLAstGenerator::visit(LogicBinopOr *node) {
        this->setValue(this->infix(node, " || "));
    }

    void MDLAstGenerator::visit(LogicBinopXor *node) {
        this->setValue("((" + this->acceptLeft(node) + " || " + this->acceptRight(node) + ")" +
            " && !(" + this->acceptLeft(node) + " && " + this->acceptRight(node) + "))");
    }

    void MDLAstGenerator::visit(Vector *node) {
        std::vector<std::shared_ptr<AstNode>> elements = node->getElements();
        std::vector<std::string> str_elements;
        for (std::shared_ptr<AstNode> element : elements) {
            str_elements.push_back(this->acceptRoot(element.get()));
        }

        this->setValue(TextFormatter::createInlineVector(str_elements, "[]", ", "));
    }

    void MDLAstGenerator::visit(Piecewise *node) {
        TextFormatter form;

        std::vector<Piece *> pieces = node->getPieces();
        if (pieces.size() > 2) {
            this->logger->warning("More than 2 pieces", pieces[2]);
        }

        Piece *otherwise = nullptr;
        form.indentAdd("if");
        for (Piece *p : pieces) {
            if (!p->isOtherwise()) {
                p->accept(this);
                form.addMany(this->getValue());
            } else {
                otherwise = p; // Only one otherwise per Piece
            }
        }
        if (otherwise) {
            otherwise->getExpression()->accept(this);
            form.addMany("else " + this->getValue());
        } else {
            this->logger->warning("Otherwise piece missing", pieces.back()->getCondition());
        }
        form.closeIndent();

        this->setValue(form.createString());
    }

    void MDLAstGenerator::visit(Piece *node) {
        this->ast_analyzer.reset();
        node->getExpression()->accept(&this->ast_analyzer);
        if (ast_analyzer.getPurePiecewise()) {
            this->logger->warning("Nested piecewise detected", node->getExpression());
        }

        node->getCondition()->accept(this);
        std::string cond = this->getValue();
        node->getExpression()->accept(this);
        std::string expr = this->getValue();

        this->setValue(cond + " then " + expr);
    }

    void MDLAstGenerator::visit(Pi *node) {
        this->setValue("(pi)");
    }

    void MDLAstGenerator::visit(Exponentiale *node) {
        this->setValue("exp(1)");
    }

    void MDLAstGenerator::visit(NullValue *node) {
        this->setValue("null");
    }

    void MDLAstGenerator::visit(FunctionCall *node) {
        TextFormatter form;

        std::string name = this->accept(node->getFunction());
        form.openVector(name + "()", 0, ", ");
        for (const std::unique_ptr<FunctionArgument> &arg : node->getFunctionArguments()) {
            form.add(this->accept(arg.get()));
        }
        form.closeVector();

        form.noFinalNewline();
        this->setValue(form.createString());
    }

    void MDLAstGenerator::visit(FunctionArgument *node) {
        node->getArgument()->accept(this);
        this->setValue(node->getSymbId() + " = " + this->getValue());
    }

    void MDLAstGenerator::visit(Interval *node) {
        std::string result = "list(left=" + this->accept(node->getLeftEndpoint());
        result += ", right=" + this->accept(node->getRightEndpoint());
        result += ", openleft=" + this->getLogicLiteral(node->isLeftEndpointOpenClosed());
        result += ", openright=" + this->getLogicLiteral(node->isRightEndpointOpenClosed());
        result += ")";
        this->setValue(result);
    }
}
