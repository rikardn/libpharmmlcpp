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

#include <string>
#include <iostream>
#include <xml/xml.h>

#include <AST/AstNodeFactory.h>
#include <AST/AstNode.h>
#include <AST/Uniop.h>
#include <AST/Binop.h>
#include <AST/symbols.h>
#include <AST/Scalar.h>
#include <AST/Constant.h>
#include <AST/Vector.h>
#include <AST/Piecewise.h>
#include <AST/FunctionCall.h>
#include <AST/Interval.h>
#include <PharmML/PharmMLContext.h>

namespace PharmML
{
    PharmMLContext *AstNodeFactory::context;

    void AstNodeFactory::setContext(PharmMLContext *context) {
        AstNodeFactory::context = context;
    }

    AstNode *AstNodeFactory::create(xml::Node node, Dependencies *deps) {
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
            uniop->setChild(AstNodeFactory::create(node.getChild(), deps));
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
            binop->setLeft(AstNodeFactory::create(node.getChild(), deps));
            binop->setRight(AstNodeFactory::create(node.getLastChild(), deps));
            instance = binop;
        } else if (name == "False") {
            instance = new LogicFalse();
        } else if (name == "True") {
            instance = new LogicTrue();
        } else if (name == "ConstantType") {
            std::string op = node.getAttribute("op").getValue();
            Constant *constant;
            if (op == "pi") {
                constant = new Pi();
            } else if (op == "exponentiale") {
                constant = new Exponentiale();
            }
            instance = constant;
        } else if (name == "SymbRef") {
            SymbRef *symbref = new SymbRef(node);
            PharmML::AstNodeFactory::context->symbRefs.push_back(symbref);
            if (deps) {
                deps->addDependency(symbref);
            }
            instance = symbref;
        } else if (name == "ColumnRef") { 
            std::string symbol = node.getAttribute("columnIdRef").getValue();
            instance = new ColumnRef(symbol);
            if (deps) {
                deps->addDependency(symbol);
            }
        } else if (name == "Int") {
            instance = new ScalarInt(node);
        } else if (name == "Real") {
            instance = new ScalarReal(node);
        } else if (name == "Vector") {
            std::string length = node.getAttribute("length").getValue();
            std::string defaultValue = node.getAttribute("default").getValue();
            Vector *vector = new Vector(length, defaultValue);
            // Get elements, cells and segments (any better way?)
            std::vector<xml::Node> children = node.getChildren();
            std::vector<xml::Node> vectorElements;
            std::vector<xml::Node> vectorCells;
            std::vector<xml::Node> vectorSegments;
            for (xml::Node node : children) {
                name = node.getName();
                if (name == "VectorElements") {
                    vectorElements.push_back(node);
                } else if (name == "VectorCell") {
                    vectorCells.push_back(node);
                } else if (name == "VectorSegment") {
                    vectorSegments.push_back(node);
                }
            }
            if (!vectorElements.empty()) {
                // Build vector object from elements
                xml::Node elements_node = vectorElements[0];
                std::vector<xml::Node> elements = elements_node.getChildren();
                for (xml::Node element : elements) {
                    vector->addElement(AstNodeFactory::create(element, deps));
                }
            } else if (!(vectorCells.empty() && vectorSegments.empty())) {
                // Build vector from cells
                for (xml::Node cell : vectorCells) {
                    std::vector<xml::Node> children = cell.getChildren();
                    int cellIndex = std::stoi(children[0].getText());
                    AstNode *cellContent = AstNodeFactory::create(children[1], deps);
                    
                    VectorCell *vectorCell = new VectorCell(cellIndex, cellContent);
                    vector->populateCell(vectorCell);
                }
                //for (xml::Node segment : vectorSegments) {
                    // TODO: Bulid vector from segments
                    //~ vector->populateSegment(segment);
                //}
            }
            instance = vector;
        } else if (name == "Piecewise") {
            Piecewise *piecewise = new Piecewise();
            std::vector<xml::Node> children = node.getChildren();
            for (xml::Node n : children) {
                Piece *piece = new Piece();
                piece->setXMLNode(n);
                piecewise->addPiece(piece);
                // Assumes expression is first child and condition last child
                xml::Node expression = n.getChild();
                xml::Node condition = n.getLastChild().getChild();
                piece->setExpression(AstNodeFactory::create(expression, deps));
                piece->setCondition(AstNodeFactory::create(condition, deps));
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
            fcall->setFunction(new SymbRef(name_node));
            for (xml::Node n : children) {
                FunctionArgument *arg = new FunctionArgument();
                fcall->addFunctionArgument(arg);
                arg->setSymbId(n.getAttribute("symbId").getValue());
                arg->setArgument(AstNodeFactory::create(n.getChild(), deps));
            }
            instance = fcall;
        } else if (name == "Interval") {
            instance = new Interval(node);
        } 
        
        // The following might and might not be a good idea
        if (instance) {
            instance->setXMLNode(node);
        }
        
        return instance;
    }
}
