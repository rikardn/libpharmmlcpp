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

#ifndef PHARMMLCPP_ASTPARENTHESIZER_H_
#define PHARMMLCPP_ASTPARENTHESIZER_H_

#include <string>
#include <unordered_set>
#include <helpers/StringTools.h>
#include <visitors/AstNodeVisitor.h>
#include <visitors/StringVisitor.h>
#include <AST/symbols.h>
#include <AST/Uniop.h>
#include <AST/Binop.h>
#include <AST/Scalar.h>
#include <AST/Constant.h>
#include <AST/Vector.h>
#include <AST/Piecewise.h>
#include <AST/Interval.h>
#include <AST/FunctionCall.h>
#include <symbols/DerivativeVariable.h>
#include <PharmML/Distribution.h>
#include <PharmML/ColumnMapping.h>
#include <PharmML/Interventions.h>

namespace pharmmlcpp
{
    // Defines if node is left or right associating
    // None: Node is end-of-chain (e.g. "1") or self-enclosing
    //       (e.g. "log(..)"); Don't parenthesize it or child
    enum class NodeAssociativity {
        Left,
        Right,
        Both,
        None // FIXME: Split into None (end-nodes) and Inner (self-enclosing nodes)
    };

    // Direction a parent accepted child. Determines location of child
    // within parenthesized parent:
    // LeftChild: "(child.."
    // RightChild: "..child)"
    // OnlyChild: "(child)"
    // MiddleChild: "(..child..)"
    enum class AcceptDirection {
        LeftChild,
        RightChild,
        OnlyChild,
        MiddleChild // FIXME: Integrate when N-ary ops are added
    };

    // To access node properties
    enum class AstOperator {
        SymbRef,
        SteadyStateParameter,
        ColumnRef,

        UniopLog,
        UniopLog2,
        UniopLog10,
        UniopExp,
        UniopMinus,
        UniopAbs,
        UniopSqrt,
        UniopLogistic,
        UniopLogit,
        UniopProbit,
        UniopNormcdf,
        UniopFactorial,
        UniopFactln,
        UniopGamma,
        UniopGammaln,
        UniopSin,
        UniopSinh,
        UniopCos,
        UniopCosh,
        UniopTan,
        UniopTanh,
        UniopCot,
        UniopCoth,
        UniopSec,
        UniopSech,
        UniopCsc,
        UniopCsch,
        UniopArcsin,
        UniopArcsinh,
        UniopArccos,
        UniopArccosh,
        UniopArctan,
        UniopArctanh,
        UniopArccot,
        UniopArccoth,
        UniopArcsec,
        UniopArcsech,
        UniopArccsc,
        UniopArccsch,
        UniopHeaviside,
        UniopSign,
        UniopFloor,
        UniopCeiling,

        ScalarInt,
        ScalarReal,
        ScalarBool,
        ScalarString,

        BinopPlus,
        BinopMinus,
        BinopDivide,
        BinopTimes,
        BinopPower,
        BinopLogx,
        BinopRoot,
        BinopMin,
        BinopMax,
        BinopRem,
        BinopAtan2,

        Pi,
        Exponentiale,
        NullValue,

        LogicUniopIsdefined,
        LogicUniopNot,

        LogicBinopLt,
        LogicBinopLeq,
        LogicBinopGt,
        LogicBinopGeq,
        LogicBinopEq,
        LogicBinopNeq,
        LogicBinopAnd,
        LogicBinopOr,
        LogicBinopXor,

        Vector,

        Piecewise,
        Piece,

        FunctionCall,
        FunctionArgument,
        Interval,
    };

    // Holds properties of node within stack (of parents)
    struct NodeProperties {
        int priority;
        NodeAssociativity associativity;
        bool commutative; // FIXME: Misnamed (means that parentheses can be removed if same priority)
        bool parenthesized;
        AstOperator node_type; // For double minus removal and commutativity resolution
    };

    // Holds stack of node properties (of parents)
    class NodePropertiesStack
    {
        public:
            NodePropertiesStack(AstNodeVisitor *visitor);
            int size();
            void setProperties(NodeProperties properties);
            void setNodeType(AstOperator node_type);
            const NodeProperties &getCurrentProperties();
            void acceptUniop(Uniop *node);
            void acceptBinop(Binop *node);
            NodeProperties *getParentLeft();
            NodeProperties *getParentRight();
            bool adjacentLeftParenthesis();
            bool adjacentRightParenthesis();

        private:
            AstNodeVisitor *visitor;
            NodeProperties set_properties;
            std::vector<NodeProperties> properties;
            std::vector<AcceptDirection> directions;

            void popStacks();
            void setParenthesesProperty(AstNode *node);
    };

    // Visitor
    class AstParenthesizer : public AstNodeVisitor
    {
        public:
            AstParenthesizer();

            void visit(SymbRef *node) override;
            void visit(SteadyStateParameter *node) override;
            void visit(ColumnRef *node) override;

            void visit(UniopLog *node) override;
            void visit(UniopLog2 *node) override;
            void visit(UniopLog10 *node) override;
            void visit(UniopExp *node) override;
            void visit(UniopMinus *node) override;
            void visit(UniopAbs *node) override;
            void visit(UniopSqrt *node) override;
            void visit(UniopLogistic *node) override;
            void visit(UniopLogit *node) override;
            void visit(UniopProbit *node) override;
            void visit(UniopNormcdf *node) override;
            void visit(UniopFactorial *node) override;
            void visit(UniopFactln *node) override;
            void visit(UniopGamma *node) override;
            void visit(UniopGammaln *node) override;
            void visit(UniopSin *node) override;
            void visit(UniopSinh *node) override;
            void visit(UniopCos *node) override;
            void visit(UniopCosh *node) override;
            void visit(UniopTan *node) override;
            void visit(UniopTanh *node) override;
            void visit(UniopCot *node) override;
            void visit(UniopCoth *node) override;
            void visit(UniopSec *node) override;
            void visit(UniopSech *node) override;
            void visit(UniopCsc *node) override;
            void visit(UniopCsch *node) override;
            void visit(UniopArcsin *node) override;
            void visit(UniopArcsinh *node) override;
            void visit(UniopArccos *node) override;
            void visit(UniopArccosh *node) override;
            void visit(UniopArctan *node) override;
            void visit(UniopArctanh *node) override;
            void visit(UniopArccot *node) override;
            void visit(UniopArccoth *node) override;
            void visit(UniopArcsec *node) override;
            void visit(UniopArcsech *node) override;
            void visit(UniopArccsc *node) override;
            void visit(UniopArccsch *node) override;
            void visit(UniopHeaviside *node) override;
            void visit(UniopSign *node) override;
            void visit(UniopFloor *node) override;
            void visit(UniopCeiling *node) override;

            void visit(ScalarInt *node) override;
            void visit(ScalarReal *node) override;
            void visit(ScalarBool *node) override;
            void visit(ScalarString *node) override;

            void visit(BinopPlus *node) override;
            void visit(BinopMinus *node) override;
            void visit(BinopDivide *node) override;
            void visit(BinopTimes *node) override;
            void visit(BinopPower *node) override;
            void visit(BinopLogx *node) override;
            void visit(BinopRoot *node) override;
            void visit(BinopMin *node) override;
            void visit(BinopMax *node) override;
            void visit(BinopRem *node) override;
            void visit(BinopAtan2 *node) override;

            void visit(Pi *node) override;
            void visit(Exponentiale *node) override;
            void visit(NullValue *node) override;

            void visit(LogicUniopIsdefined *node) override;
            void visit(LogicUniopNot *node) override;

            void visit(LogicBinopLt *node) override;
            void visit(LogicBinopLeq *node) override;
            void visit(LogicBinopGt *node) override;
            void visit(LogicBinopGeq *node) override;
            void visit(LogicBinopEq *node) override;
            void visit(LogicBinopNeq *node) override;
            void visit(LogicBinopAnd *node) override;
            void visit(LogicBinopOr *node) override;
            void visit(LogicBinopXor *node) override;

            void visit(Vector *node) override;

            void visit(Piecewise *node) override;
            void visit(Piece *node) override;

            void visit(FunctionCall *node) override;
            void visit(FunctionArgument *node) override;
            void visit(Interval *node) override;

        private:
            struct EnumClassHash
            {
                template <typename T>
                std::size_t operator()(T t) const
                {
                    return static_cast<std::size_t>(t);
                }
            };
            // Default node properties (modelled after R)
            std::unordered_map<AstOperator, NodeProperties, EnumClassHash> node_properties = {
                // end nodes (highest default priority)
                {AstOperator::SymbRef, {11, NodeAssociativity::None, false}},
                {AstOperator::ScalarInt, {11, NodeAssociativity::None, false}},
                {AstOperator::ScalarReal, {11, NodeAssociativity::None, false}},
                {AstOperator::ScalarBool, {11, NodeAssociativity::None, false}},
                {AstOperator::ScalarString, {11, NodeAssociativity::None, false}},
                {AstOperator::Pi, {11, NodeAssociativity::None, false}},
                {AstOperator::Exponentiale, {11, NodeAssociativity::None, false}},

                // functions (self-enclosed)
                {AstOperator::UniopLog, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopLog2, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopLog10, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopExp, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopAbs, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopSqrt, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopLogistic, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopLogit, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopProbit, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopNormcdf, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopFactorial, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopFactln, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopGamma, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopGammaln, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopSin, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopSinh, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopCos, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopCosh, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopTan, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopTanh, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopCot, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopCoth, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopSec, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopSech, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopCsc, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopCsch, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopArcsin, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopArcsinh, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopArccos, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopArccosh, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopArctan, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopArctanh, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopArccot, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopArccoth, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopArcsec, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopArcsech, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopArccsc, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopArccsch, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopHeaviside, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopSign, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopFloor, {10, NodeAssociativity::None, false}},
                {AstOperator::UniopCeiling, {10, NodeAssociativity::None, false}},
                {AstOperator::BinopLogx, {10, NodeAssociativity::None, false}},
                {AstOperator::BinopRoot, {10, NodeAssociativity::None, false}},
                {AstOperator::BinopMin, {10, NodeAssociativity::None, false}},
                {AstOperator::BinopMax, {10, NodeAssociativity::None, false}},
                {AstOperator::BinopAtan2, {10, NodeAssociativity::None, false}},
                {AstOperator::LogicUniopIsdefined, {10, NodeAssociativity::None, false}},
                {AstOperator::FunctionCall, {10, NodeAssociativity::None, false}},
                {AstOperator::LogicBinopXor, {10, NodeAssociativity::None, false}},

                // exponentiation
                {AstOperator::BinopPower, {9, NodeAssociativity::Right, false}},

                // unary minus (also negative scalars)
                {AstOperator::UniopMinus, {8, NodeAssociativity::Left, false}},

                // modulo
                {AstOperator::BinopRem, {7, NodeAssociativity::Left, false}},

                // multiplication/division
                {AstOperator::BinopDivide, {6, NodeAssociativity::Left, false}},
                {AstOperator::BinopTimes, {6, NodeAssociativity::Left, true}},

                // addition/subtraction
                {AstOperator::BinopPlus, {5, NodeAssociativity::Left, true}},
                {AstOperator::BinopMinus, {5, NodeAssociativity::Left, false}},

                // logical comparisons
                {AstOperator::LogicBinopLt, {4, NodeAssociativity::Both, false}},
                {AstOperator::LogicBinopLeq, {4, NodeAssociativity::Both, false}},
                {AstOperator::LogicBinopGt, {4, NodeAssociativity::Both, false}},
                {AstOperator::LogicBinopGeq, {4, NodeAssociativity::Both, false}},
                {AstOperator::LogicBinopEq, {4, NodeAssociativity::Both, false}},
                {AstOperator::LogicBinopNeq, {4, NodeAssociativity::Both, false}},

                // logical not
                {AstOperator::LogicUniopNot, {3, NodeAssociativity::Left, false}},

                // logical and
                {AstOperator::LogicBinopAnd, {2, NodeAssociativity::Left, false}},

                // logical or
                {AstOperator::LogicBinopOr, {1, NodeAssociativity::Left, false}},

                // uncategorized (lowest priority)
                {AstOperator::SteadyStateParameter, {0, NodeAssociativity::Both, false}},
                {AstOperator::ColumnRef, {0, NodeAssociativity::Both, false}},
                {AstOperator::Vector, {0, NodeAssociativity::Both, false}},
                {AstOperator::Piecewise, {0, NodeAssociativity::Both, false}},
                {AstOperator::Piece, {0, NodeAssociativity::Both, false}},
                {AstOperator::FunctionArgument, {0, NodeAssociativity::Both, false}},
                {AstOperator::Interval, {0, NodeAssociativity::Both, false}},
            };
            bool pretty_minus = true; // e.g. "-a - (-b)", never "-a - -b"

            NodePropertiesStack parents{this};

            void acceptUniop(Uniop *node, AstOperator node_type);
            void acceptBinop(Binop *node, AstOperator node_type);
            void acceptEndNode(AstNode *node, AstOperator node_type);
            bool requiresParentheses();
    };
}

#endif
