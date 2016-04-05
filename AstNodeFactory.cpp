#include <string>
#include <iostream>
#include "xml.h"

#include "AstNodeFactory.h"
#include "AstNode.h"
#include "Uniop.h"
#include "Binop.h"
#include "symbols.h"
#include "Scalar.h"
#include "Piecewise.h"

namespace PharmML
{
    AstNode *AstNodeFactory::create(xml::Node node) {
        AstNode *instance = nullptr;

        std::string name = node.getName();
        if (name == "Uniop") {
            std::string op = node.getAttribute("op").getValue();
            Uniop *uniop;
            if (op == "log") {
                uniop = new UniopLog();
            } else if (op == "exp") {
                uniop = new UniopExp();
            } else if (op == "minus") {
                uniop = new UniopMinus();
            } else if (op == "abs") {
                uniop = new UniopAbs();
            }
            uniop->setChild(this->create(node.getChild()));
            instance = uniop;
        } else if (name == "Binop" || name == "LogicBinop") {
            std::string op = node.getAttribute("op").getValue();
            Binop *binop;
            if (op == "plus") {
                binop = new BinopPlus();
            } else if (op == "minus") {
                binop = new BinopMinus();
            } else if (op == "divide") {
                binop = new BinopDivide();
            } else if (op == "times") {
                binop = new BinopTimes();
            } else if (op == "lt") {
                binop = new LogicBinopLt();
            } else if (op == "leq") {
                binop = new LogicBinopLeq();
            }
            binop->setLeft(this->create(node.getChild()));
            binop->setRight(this->create(node.getLastChild()));
            instance = binop;
        } else if (name == "SymbRef") {
            instance = new SymbRef(node.getAttribute("symbIdRef").getValue());
        } else if (name == "Int") {
            instance = new ScalarInt(node);
        } else if (name == "Piecewise") {
            Piecewise *piecewise = new Piecewise();
            std::vector<xml::Node> children = node.getChildren();
            for (xml::Node n : children) {
                Piece *piece = new Piece();
                piecewise->addPiece(piece);
                piece->setExpression(this->create(n.getChild()));
                piece->setCondition(this->create(n.getLastChild().getChild()));     // Go past math:Condition 
            }
            instance = piecewise;
        }

        return instance;
    }
}
