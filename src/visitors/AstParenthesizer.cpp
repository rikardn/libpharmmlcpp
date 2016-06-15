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

#include "AstParenthesizer.h"

namespace pharmmlcpp
{
    // private
    NodePropertiesStack::NodePropertiesStack(AstNodeVisitor *visitor) {
        this->visitor = visitor;
    }

    int NodePropertiesStack::size() {
        return this->properties.size();
    }

    void NodePropertiesStack::setProperties(NodeProperties properties) {
        this->set_properties = properties;
    }
    
    void NodePropertiesStack::acceptUniop(Uniop *node) {
        this->properties.push_back(this->set_properties);
        this->directions.push_back(AcceptDirection::LeftChild);
        node->getChild()->accept(this->visitor);
        this->popStacks();
    }

    void NodePropertiesStack::acceptBinop(Binop *node) {
        this->properties.push_back(this->set_properties);
        this->directions.push_back(AcceptDirection::LeftChild);
        node->getLeft()->accept(this->visitor);
        this->popStacks();

        this->properties.push_back(this->set_properties);
        this->directions.push_back(AcceptDirection::RightChild);
        node->getRight()->accept(this->visitor);
        this->popStacks();
    }

    void NodePropertiesStack::popStacks() {
        this->properties.pop_back();
        this->directions.pop_back();
    }

    // public
    AstParenthesizer::AstParenthesizer() {

    }

    // private
    bool AstParenthesizer::requiresParentheses(const NodeProperties &properties) {
        // Root nodes never require parentheses
        if (this->parents.size() == 0) {
            return false;
        }
        return true;
    }

    // visitor methods
    void AstParenthesizer::visit(SymbRef *node) { }

    void AstParenthesizer::visit(SteadyStateParameter *node) { }

    void AstParenthesizer::visit(ColumnRef *node) { }

    void AstParenthesizer::visit(UniopLog *node) { }

    void AstParenthesizer::visit(UniopLog2 *node) { }

    void AstParenthesizer::visit(UniopLog10 *node) { }

    void AstParenthesizer::visit(UniopExp *node) { }

    void AstParenthesizer::visit(UniopMinus *node) {
        this->parents.setProperties(node_properties[AstOperator::UniopMinus]);

        this->parents.acceptUniop(node);
    }

    void AstParenthesizer::visit(UniopAbs *node) { }

    void AstParenthesizer::visit(UniopSqrt *node) { }

    void AstParenthesizer::visit(UniopLogistic *node) { }

    void AstParenthesizer::visit(UniopLogit *node) { }

    void AstParenthesizer::visit(UniopProbit *node) { }

    void AstParenthesizer::visit(UniopNormcdf *node) { }

    void AstParenthesizer::visit(UniopFactorial *node) { }

    void AstParenthesizer::visit(UniopFactln *node) { }

    void AstParenthesizer::visit(UniopGamma *node) { }

    void AstParenthesizer::visit(UniopGammaln *node) { }

    void AstParenthesizer::visit(UniopSin *node) { }

    void AstParenthesizer::visit(UniopSinh *node) { }

    void AstParenthesizer::visit(UniopCos *node) { }

    void AstParenthesizer::visit(UniopCosh *node) { }

    void AstParenthesizer::visit(UniopTan *node) { }

    void AstParenthesizer::visit(UniopTanh *node) { }

    void AstParenthesizer::visit(UniopCot *node) { }

    void AstParenthesizer::visit(UniopCoth *node) { }

    void AstParenthesizer::visit(UniopSec *node) { }

    void AstParenthesizer::visit(UniopSech *node) { }

    void AstParenthesizer::visit(UniopCsc *node) { }

    void AstParenthesizer::visit(UniopCsch *node) { }

    void AstParenthesizer::visit(UniopArcsin *node) { }

    void AstParenthesizer::visit(UniopArcsinh *node) { }

    void AstParenthesizer::visit(UniopArccos *node) { }

    void AstParenthesizer::visit(UniopArccosh *node) { }

    void AstParenthesizer::visit(UniopArctan *node) { }

    void AstParenthesizer::visit(UniopArctanh *node) { }

    void AstParenthesizer::visit(UniopArccot *node) { }

    void AstParenthesizer::visit(UniopArccoth *node) { }

    void AstParenthesizer::visit(UniopArcsec *node) { }

    void AstParenthesizer::visit(UniopArcsech *node) { }

    void AstParenthesizer::visit(UniopArccsc *node) { }

    void AstParenthesizer::visit(UniopArccsch *node) { }

    void AstParenthesizer::visit(UniopHeaviside *node) { }

    void AstParenthesizer::visit(UniopSign *node) { }

    void AstParenthesizer::visit(UniopFloor *node) { }

    void AstParenthesizer::visit(UniopCeiling *node) { }

    void AstParenthesizer::visit(ScalarInt *node) {
        if (node->toInt() >= 0) {
            this->parents.setProperties(node_properties[AstOperator::ScalarInt]);
        } else {
            this->parents.setProperties(node_properties[AstOperator::UniopMinus]);
        }
    }

    void AstParenthesizer::visit(ScalarReal *node) {
        if (node->toDouble() >= 0) {
            this->parents.setProperties(node_properties[AstOperator::ScalarReal]);
        } else {
            this->parents.setProperties(node_properties[AstOperator::UniopMinus]);
        }
    }

    void AstParenthesizer::visit(ScalarBool *node) { }

    void AstParenthesizer::visit(ScalarString *node) { }

    void AstParenthesizer::visit(BinopPlus *node) {
        const NodeProperties &props = node_properties[AstOperator::BinopPlus];
        this->parents.setProperties(props);
        if (!this->requiresParentheses(props)) node->elideParentheses();
        this->parents.acceptBinop(node);
    }

    void AstParenthesizer::visit(BinopMinus *node) {
        this->parents.setProperties(node_properties[AstOperator::BinopMinus]);

        this->parents.acceptBinop(node);
    }

    void AstParenthesizer::visit(BinopDivide *node) {
        this->parents.setProperties(node_properties[AstOperator::BinopDivide]);

        this->parents.acceptBinop(node);
    }

    void AstParenthesizer::visit(BinopTimes *node) {
        this->parents.setProperties(node_properties[AstOperator::BinopTimes]);

        this->parents.acceptBinop(node);
    }

    void AstParenthesizer::visit(BinopPower *node) {
        this->parents.setProperties(node_properties[AstOperator::BinopPower]);

        this->parents.acceptBinop(node);
    }

    void AstParenthesizer::visit(BinopLogx *node) { }

    void AstParenthesizer::visit(BinopRoot *node) { }

    void AstParenthesizer::visit(BinopMin *node) { }

    void AstParenthesizer::visit(BinopMax *node) { }

    void AstParenthesizer::visit(BinopRem *node) { }

    void AstParenthesizer::visit(BinopAtan2 *node) { }

    void AstParenthesizer::visit(Pi *node) { }

    void AstParenthesizer::visit(Exponentiale *node) { }

    void AstParenthesizer::visit(NullValue *node) { }

    void AstParenthesizer::visit(LogicUniopIsdefined *node) { }

    void AstParenthesizer::visit(LogicUniopNot *node) { }

    void AstParenthesizer::visit(LogicBinopLt *node) { }

    void AstParenthesizer::visit(LogicBinopLeq *node) { }

    void AstParenthesizer::visit(LogicBinopGt *node) { }

    void AstParenthesizer::visit(LogicBinopGeq *node) { }

    void AstParenthesizer::visit(LogicBinopEq *node) { }

    void AstParenthesizer::visit(LogicBinopNeq *node) { }

    void AstParenthesizer::visit(LogicBinopAnd *node) { }

    void AstParenthesizer::visit(LogicBinopOr *node) { }

    void AstParenthesizer::visit(LogicBinopXor *node) { }

    void AstParenthesizer::visit(Vector *node) { }

    void AstParenthesizer::visit(Piecewise *node) { }

    void AstParenthesizer::visit(Piece *node) { }

    void AstParenthesizer::visit(FunctionCall *node) { }

    void AstParenthesizer::visit(FunctionArgument *node) { }

    void AstParenthesizer::visit(Interval *node) { }
}
