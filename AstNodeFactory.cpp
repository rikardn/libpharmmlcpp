#include <string>
#include <iostream>
#include "xml.h"

#include "AstNodeFactory.h"
#include "AstNode.h"
#include "Uniop.h"
#include "Binop.h"
#include "symbols.h"
#include "Scalar.h"
#include "Constant.h"
#include "Piecewise.h"
#include "FunctionCall.h"

namespace PharmML
{
    AstNode *AstNodeFactory::create(xml::Node node) {
        AstNode *instance = nullptr;

        std::string name = node.getName();
        if (name == "Uniop" || name == "LogicUniop") {
            std::string op = node.getAttribute("op").getValue();
            Uniop *uniop;
            // Ordinary uniops
            if (op == "log") {
                uniop = new UniopLog();
            } else if (op == "log2") {
                uniop = new UniopLog2();
            } else if (op == "log10") {
                uniop = new UniopLog10();
            } else if (op == "exp") {
                uniop = new UniopExp();
            } else if (op == "minus") {
                uniop = new UniopMinus();
            } else if (op == "abs") {
                uniop = new UniopAbs();
            } else if (op == "sqrt") {
                uniop = new UniopSqrt();
            } else if (op == "logistic") {
                uniop = new UniopLogistic();
            } else if (op == "logit") {
                uniop = new UniopLogit();
            } else if (op == "probit") {
                uniop = new UniopProbit();
            } else if (op == "normcdf") {
                uniop = new UniopNormcdf();
            } else if (op == "factorial") {
                uniop = new UniopFactorial();
            } else if (op == "factln") {
                uniop = new UniopFactln();
            } else if (op == "gamma") {
                uniop = new UniopGamma();
            } else if (op == "gammaln") {
                uniop = new UniopGammaln();
            } else if (op == "sin") {
                uniop = new UniopSin();
            } else if (op == "sinh") {
                uniop = new UniopSinh();
            } else if (op == "cos") {
                uniop = new UniopCos();
            } else if (op == "cosh") {
                uniop = new UniopCosh();
            } else if (op == "tan") {
                uniop = new UniopTan();
            } else if (op == "tanh") {
                uniop = new UniopTanh();
            } else if (op == "cot") {
                uniop = new UniopCot();
            } else if (op == "coth") {
                uniop = new UniopCoth();
            } else if (op == "sec") {
                uniop = new UniopSec();
            } else if (op == "sech") {
                uniop = new UniopSech();
            } else if (op == "csc") {
                uniop = new UniopCsc();
            } else if (op == "csch") {
                uniop = new UniopCsch();
            } else if (op == "arcsin") {
                uniop = new UniopArcsin();
            } else if (op == "arcsinh") {
                uniop = new UniopArcsinh();
            } else if (op == "arccos") {
                uniop = new UniopArccos();
            } else if (op == "arccosh") {
                uniop = new UniopArccosh();
            } else if (op == "arctan") {
                uniop = new UniopArctan();
            } else if (op == "arctanh") {
                uniop = new UniopArctanh();
            } else if (op == "arccot") {
                uniop = new UniopArccot();
            } else if (op == "arccoth") {
                uniop = new UniopArccoth();
            } else if (op == "arcsec") {
                uniop = new UniopArcsec();
            } else if (op == "arcsech") {
                uniop = new UniopArcsech();
            } else if (op == "arccsc") {
                uniop = new UniopArccsc();
            } else if (op == "arccsch") {
                uniop = new UniopArccsch();
            } else if (op == "heaviside") {
                uniop = new UniopHeaviside();
            } else if (op == "sign") {
                uniop = new UniopSign();
            } else if (op == "floor") {
                uniop = new UniopFloor();
            } else if (op == "ceiling") {
                uniop = new UniopCeiling();
            // Logic uniops
            } else if (op == "isDefined") {
                uniop = new LogicUniopIsdefined();
            } else if (op == "not") {
                uniop = new LogicUniopNot();
            }
            uniop->setChild(this->create(node.getChild()));
            instance = uniop;
        } else if (name == "Binop" || name == "LogicBinop") {
            std::string op = node.getAttribute("op").getValue();
            Binop *binop;
            // Ordinary binops
            if (op == "plus") {
                binop = new BinopPlus();
            } else if (op == "minus") {
                binop = new BinopMinus();
            } else if (op == "divide") {
                binop = new BinopDivide();
            } else if (op == "times") {
                binop = new BinopTimes();
            } else if (op == "power") {
                binop = new BinopPower();
            } else if (op == "logx") {
                binop = new BinopLogx();
            } else if (op == "root") {
                binop = new BinopRoot();
            } else if (op == "min") {
                binop = new BinopMin();
            } else if (op == "max") {
                binop = new BinopMax();
            } else if (op == "rem") {
                binop = new BinopRem();
            } else if (op == "atan2") {
                binop = new BinopAtan2();
            // Logic binops
            } else if (op == "lt") {
                binop = new LogicBinopLt();
            } else if (op == "leq") {
                binop = new LogicBinopLeq();
            } else if (op == "gt") {
                binop = new LogicBinopGt();
            } else if (op == "geq") {
                binop = new LogicBinopGeq();
            } else if (op == "eq") {
                binop = new LogicBinopEq();
            } else if (op == "neq") {
                binop = new LogicBinopNeq();
            } else if (op == "and") {
                binop = new LogicBinopAnd();
            } else if (op == "or") {
                binop = new LogicBinopOr();
            } else if (op == "xor") {
                binop = new LogicBinopXor();
            }
            binop->setLeft(this->create(node.getChild()));
            binop->setRight(this->create(node.getLastChild()));
            instance = binop;
		} else if (name == "False") {
			instance = new LogicFalse();
		} else if (name == "True") {
			instance = new LogicTrue();
        } else if (name == "SymbRef") {
            instance = new SymbRef(node.getAttribute("symbIdRef").getValue());
        } else if (name == "Int") {
            instance = new ScalarInt(node);
        } else if (name == "Real") {
            instance = new ScalarReal(node);
        } else if (name == "Piecewise") {
            Piecewise *piecewise = new Piecewise();
            std::vector<xml::Node> children = node.getChildren();
            for (xml::Node n : children) {
                Piece *piece = new Piece();
                piecewise->addPiece(piece);
                // Assumes expression is first child and condition last child
                xml::Node expression = n.getChild();
                xml::Node condition = n.getLastChild().getChild();
                piece->setExpression(this->create(expression));
                piece->setCondition(this->create(condition));
                // Otherwise property gets lost in translation from xml::Node to AstNode so save it now
                if (condition.getName() == "Otherwise") {
                    piece->setOtherwise();
                }
            }
            instance = piecewise;
        } else if (name == "FunctionCall") {
            FunctionCall *fcall = new FunctionCall();
            std::vector<xml::Node> children = node.getChildren();
            xml::Node name_node = children[0];
            children.erase(children.begin());
            fcall->setFunctionName(this->create(name_node));
            for (xml::Node n : children) {
                FunctionArgument *arg = new FunctionArgument();
                fcall->addFunctionArgument(arg);
                arg->setSymbId(n.getAttribute("symbId").getValue());
                arg->setArgument(this->create(n.getChild()));
            }
            instance = fcall;
        } 

        return instance;
    }
}
