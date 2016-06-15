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
    enum class NodeAssociativity {
        Left, Right, None
    };
    enum class AcceptDirection {
        LeftChild, RightChild
    };

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

    struct NodeProperties {
        int priority;
        NodeAssociativity associativity;
        bool commutative;
    };

    class NodePropertiesStack
    {
        public:
            NodePropertiesStack(AstNodeVisitor *visitor);
            int size();
            void setProperties(NodeProperties properties);
            void acceptUniop(Uniop *node);
            void acceptBinop(Binop *node);
            NodeProperties *getParentLeft();
            NodeProperties *getParentRight();
            
        private:
            AstNodeVisitor *visitor;
            NodeProperties set_properties;
            std::vector<NodeProperties> properties;
            std::vector<AcceptDirection> directions;

            void popStacks();
    };

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
            std::unordered_map<AstOperator, NodeProperties, EnumClassHash> node_properties = {
                {AstOperator::UniopMinus, {4, NodeAssociativity::Left, false}},
                {AstOperator::ScalarInt, {6, NodeAssociativity::None, false}},
                {AstOperator::ScalarReal, {6, NodeAssociativity::None, false}},
                {AstOperator::BinopPlus, {2, NodeAssociativity::Left, true}},
                {AstOperator::BinopMinus, {2, NodeAssociativity::Left, false}},
                {AstOperator::BinopDivide, {3, NodeAssociativity::Left, false}},
                {AstOperator::BinopTimes, {3, NodeAssociativity::Left, false}},
                {AstOperator::BinopPower, {5, NodeAssociativity::Right, true}},
            };

            NodePropertiesStack parents{this};
            bool requiresParentheses(const NodeProperties &properties);
    };
}

#endif
