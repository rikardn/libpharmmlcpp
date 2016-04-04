#include "RGenerator.h"

namespace PharmML
{
    std::string RGenerator::visit(SymbRef *node) {
        return node->toString();
    }

    std::string RGenerator::visit(UniopLog *node) {
        return("log(" + node->acceptChild(this) + ")");
    }

    std::string RGenerator::visit(UniopExp *node) {
        return("exp(" + node->acceptChild(this) + ")");
    }

    std::string RGenerator::visit(UniopMinus *node) {
        return("(-" + node->acceptChild(this) + ")");
    }

    std::string RGenerator::visit(ScalarInt *node) {
        return("(" + node->toString() + ")");
    }

    std::string RGenerator::visit(BinopPlus *node) {
        return("(" + node->acceptLeft(this) + " + " + node->acceptRight(this) + ")");
    }

    std::string RGenerator::visit(BinopMinus *node) {
        return("(" + node->acceptLeft(this) + " - " + node->acceptRight(this) + ")");
    }

    std::string RGenerator::visit(BinopDivide *node) {
        return("(" + node->acceptLeft(this) + " / " + node->acceptRight(this) + ")");
    }

    std::string RGenerator::visit(BinopTimes *node) {
        return("(" + node->acceptLeft(this) + " * " + node->acceptRight(this) + ")");
    }

    std::string RGenerator::visit(LogicBinopLt *node) {
        return(node->infix(this, " < "));
    }

    std::string RGenerator::visit(LogicBinopLeq *node) {
        return(node->infix(this, " <= "));
    }

    std::string RGenerator::visit(Piecewise *node) {
        std::vector<Piece *> pieces = node->getPieces();
        std::string s = "ifelse(" + pieces[0]->getCondition()->accept(this);
        for (Piece *p : pieces) {
            s += ", " + p->getExpression()->accept(this); 
        }
        return(s + ")");
    }

    // Never visited. Limited piecewise for now.
    std::string RGenerator::visit(Piece *node) {
        return("{" + node->getCondition()->accept(this) + "}");
    }

    std::string RGenerator::visit(Covariate *node) {
        return(node->getTransformedName() + " <- " + node->getAssignment()->accept(this));
    }

    std::string RGenerator::visit(Variable *node) {
        if (node->getAssignment()) {
            return(node->getSymbId() + " <- " + node->getAssignment()->accept(this));
        } else {
            return std::string();
        }
    }
}
