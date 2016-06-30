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
    // Stack of node properties with visitor to traverse
    NodePropertiesStack::NodePropertiesStack(AstNodeVisitor *visitor) {
        this->visitor = visitor;
    }

    // Get current size of stack
    int NodePropertiesStack::size() {
        return this->properties.size();
    }

    // Set current node properties (pushed to stack only when node is accepted)
    void NodePropertiesStack::setProperties(NodeProperties properties) {
        this->set_properties = properties;
    }

    // Set current node type (pushed to stack only when node is accepted)
    void NodePropertiesStack::setNodeType(AstOperator node_type) {
        // TODO: Not very pretty, but required to remove double minus and resolve commutativity
        this->set_properties.node_type = node_type;
    }

    // Get current node properties
    const NodeProperties &NodePropertiesStack::getCurrentProperties() {
        return this->set_properties;
    }

    // Push set properties, push child direction and accept uniop node (traverse); pop node from stack when exiting
    void NodePropertiesStack::acceptUniop(Uniop *node) {
        this->setParenthesesProperty(node);

        this->properties.push_back(this->set_properties);
        this->directions.push_back(AcceptDirection::OnlyChild);
        node->getChild()->accept(this->visitor);
        this->popStacks();
    }

    // Push set properties, push child direction and accept binop node (traverse); pop node from stack when exiting
    void NodePropertiesStack::acceptBinop(Binop *node) {
        this->setParenthesesProperty(node);

        this->properties.push_back(this->set_properties);
        this->directions.push_back(AcceptDirection::LeftChild);
        node->getLeft()->accept(this->visitor);

        this->directions.pop_back();
        this->directions.push_back(AcceptDirection::RightChild);
        node->getRight()->accept(this->visitor);
        this->popStacks();
    }

    // Fetch properties of the first parent to the left (in flattened AST)
    // (i.e., first parent whom not yet pushed node is a right child of)
    NodeProperties *NodePropertiesStack::getParentLeft() {
        for (auto it = this->directions.rbegin(); it != this->directions.rend(); ++it) {
            if ((*it) == AcceptDirection::RightChild || (*it) == AcceptDirection::OnlyChild) {
                auto dir_index = std::distance(this->directions.rbegin(), it);
                auto prop_it = this->properties.rbegin() + dir_index;
                return &(*prop_it);
            }
        }
        return nullptr;
    }

    // Fetch properties of the first parent to the right (in flattened AST)
    // (i.e., first parent whom not yet pushed node is a left child of)
    NodeProperties *NodePropertiesStack::getParentRight() {
        for (auto it = this->directions.rbegin(); it != this->directions.rend(); ++it) {
            if ((*it) == AcceptDirection::LeftChild || (*it) == AcceptDirection::OnlyChild) {
                auto dir_index = std::distance(this->directions.rbegin(), it);
                auto prop_it = this->properties.rbegin() + dir_index;
                return &(*prop_it);
            }
        }
        return nullptr;
    }

    // Check if parent node left-parenthesizes (not yet pushed node)
    bool NodePropertiesStack::adjacentLeftParenthesis() {
        for (auto it = this->directions.rbegin(); it != this->directions.rend(); ++it) {
            auto dir_index = std::distance(this->directions.rbegin(), it);
            auto prop_it = this->properties.rbegin() + dir_index;
            if ((*prop_it).associativity == NodeAssociativity::None) return true; // e.g. log(x), not log((x))
            if ((*prop_it).position == OperatorPosition::Prefix) break;
            if ((*it) == AcceptDirection::RightChild && (*prop_it).position == OperatorPosition::Infix) break;
            if ((*it) == AcceptDirection::MiddleChild && (*prop_it).position == OperatorPosition::Infix) break;
            if ((*prop_it).parenthesized == true) return true;
        }
        return false;
    }

    // Check if parent node right-parenthesizes (not yet pushed node)
    bool NodePropertiesStack::adjacentRightParenthesis() {
        for (auto it = this->directions.rbegin(); it != this->directions.rend(); ++it) {
            auto dir_index = std::distance(this->directions.rbegin(), it);
            auto prop_it = this->properties.rbegin() + dir_index;
            if ((*prop_it).associativity == NodeAssociativity::None) return true; // e.g. log(x), not log((x))
            if ((*prop_it).position == OperatorPosition::Postfix) break;
            if ((*it) == AcceptDirection::LeftChild && (*prop_it).position == OperatorPosition::Infix) break;
            if ((*it) == AcceptDirection::MiddleChild && (*prop_it).position == OperatorPosition::Infix) break;
            if ((*prop_it).parenthesized == true) return true;
        }
        return false;
    }

    // Pop internal stacks (of last pushed node)
    void NodePropertiesStack::popStacks() {
        this->properties.pop_back();
        this->directions.pop_back();
    }

    // Set parentheses property from node (by internal accept before pushing and visiting)
    void NodePropertiesStack::setParenthesesProperty(AstNode *node) {
        this->set_properties.parenthesized = (node->hasParentheses() ? true : false);
    }

    // Visitor to traverse and determine if node requires parentheses
    // (with regards to operator priority, associativity, commutativity and parent node parentheses)
    AstParenthesizer::AstParenthesizer() { }

    void AstParenthesizer::acceptUniop(Uniop *node, AstOperator node_type) {
        this->parents.setProperties(this->node_properties[node_type]);
        this->parents.setNodeType(node_type);
        if (!this->requiresParentheses()) node->elideParentheses();
        this->parents.acceptUniop(node);
    }

    void AstParenthesizer::acceptBinop(Binop *node, AstOperator node_type) {
        this->parents.setProperties(this->node_properties[node_type]);
        this->parents.setNodeType(node_type);
        if (!this->requiresParentheses()) node->elideParentheses();
        this->parents.acceptBinop(node);
    }

    void AstParenthesizer::acceptEndNode(AstNode *node, AstOperator node_type) {
        this->parents.setProperties(this->node_properties[node_type]);
        this->parents.setNodeType(node_type);
        if (!this->requiresParentheses()) node->elideParentheses();
    }

    // Determine if current node (not yet accepted) requires parentheses (using internal parent stack)
    bool AstParenthesizer::requiresParentheses() {
        // Root nodes never require parentheses
        if (this->parents.size() == 0) {
            return false;
        }

        // Properties of current (not yet pushed) node
        const NodeProperties &cur_props = parents.getCurrentProperties();
        if (cur_props.associativity == NodeAssociativity::None) return false;

        // Check parent node to the "left" (which this node is RIGHT child of)
        NodeProperties *left_prop = this->parents.getParentLeft();
        if (left_prop) {
            // Only check rules if no parent node already protects with left "("
            if (!this->parents.adjacentLeftParenthesis()) {
                if (left_prop->priority > cur_props.priority) { // Lower priority requires ()
                    return true;
                } else if (left_prop->priority == cur_props.priority) { // Same priority might require ()
                    if (cur_props.associativity == NodeAssociativity::Left || cur_props.associativity == NodeAssociativity::Both) {
                        if (!left_prop->commutative) return true;
                    }
                } else if (this->pretty_minus) {
                    if (cur_props.node_type == AstOperator::UniopMinus) {
                        return true;
                    }
                }
            }
        }

        // Check parent node to the "right" (which this node is LEFT child of)
        NodeProperties *right_prop = this->parents.getParentRight();
        if (right_prop) {
            // Only check rules if no parent node already protects with right ")"
            if (!this->parents.adjacentRightParenthesis()) {
                if (right_prop->priority > cur_props.priority) { // Lower priority requires ()
                    return true;
                } else if (right_prop->priority == cur_props.priority) { // Same priority might require ()
                    if (cur_props.associativity == NodeAssociativity::Right || cur_props.associativity == NodeAssociativity::Both) {
                        if (!right_prop->commutative) return true;
                    }
                }
            }
        }

        // No rule triggered: () must not be required
        return false;
    }

    // Visit methods; Procedure:
    // 1. Fetch properties of node (operator) and load into stack
    // 2. Determine if node requires () considering parents visited (and set AstNode flag)
    // 3. Let stack push node properties and accept node (via this visitor)
    void AstParenthesizer::visit(SymbRef *node) {
        this->acceptEndNode(node, AstOperator::SymbRef);
    }

    void AstParenthesizer::visit(SteadyStateParameter *node) { }

    void AstParenthesizer::visit(ColumnRef *node) { }

    void AstParenthesizer::visit(UniopLog *node) {
        this->acceptUniop(node, AstOperator::UniopLog);
    }

    void AstParenthesizer::visit(UniopLog2 *node) {
        this->acceptUniop(node, AstOperator::UniopLog2);
    }

    void AstParenthesizer::visit(UniopLog10 *node) {
        this->acceptUniop(node, AstOperator::UniopLog10);
    }

    void AstParenthesizer::visit(UniopExp *node) {
        this->acceptUniop(node, AstOperator::UniopExp);
    }

    void AstParenthesizer::visit(UniopMinus *node) {
        this->acceptUniop(node, AstOperator::UniopMinus);
    }

    void AstParenthesizer::visit(UniopAbs *node) {
        this->acceptUniop(node, AstOperator::UniopAbs);
    }

    void AstParenthesizer::visit(UniopSqrt *node) {
        this->acceptUniop(node, AstOperator::UniopSqrt);
    }

    void AstParenthesizer::visit(UniopLogistic *node) {
        this->acceptUniop(node, AstOperator::UniopLogistic);
    }

    void AstParenthesizer::visit(UniopLogit *node) {
        this->acceptUniop(node, AstOperator::UniopLogit);
    }

    void AstParenthesizer::visit(UniopProbit *node) {
        this->acceptUniop(node, AstOperator::UniopProbit);
    }

    void AstParenthesizer::visit(UniopNormcdf *node) {
        this->acceptUniop(node, AstOperator::UniopNormcdf);
    }

    void AstParenthesizer::visit(UniopFactorial *node) {
        this->acceptUniop(node, AstOperator::UniopFactorial);
    }

    void AstParenthesizer::visit(UniopFactln *node) {
        this->acceptUniop(node, AstOperator::UniopFactln);
    }

    void AstParenthesizer::visit(UniopGamma *node) {
        this->acceptUniop(node, AstOperator::UniopGamma);
    }

    void AstParenthesizer::visit(UniopGammaln *node) {
        this->acceptUniop(node, AstOperator::UniopGammaln);
    }

    void AstParenthesizer::visit(UniopSin *node) {
        this->acceptUniop(node, AstOperator::UniopSin);
    }

    void AstParenthesizer::visit(UniopSinh *node) {
        this->acceptUniop(node, AstOperator::UniopSinh);
    }

    void AstParenthesizer::visit(UniopCos *node) {
        this->acceptUniop(node, AstOperator::UniopCos);
    }

    void AstParenthesizer::visit(UniopCosh *node) {
        this->acceptUniop(node, AstOperator::UniopCosh);
    }

    void AstParenthesizer::visit(UniopTan *node) {
        this->acceptUniop(node, AstOperator::UniopTan);
    }

    void AstParenthesizer::visit(UniopTanh *node) {
        this->acceptUniop(node, AstOperator::UniopTanh);
    }

    void AstParenthesizer::visit(UniopCot *node) {
        this->acceptUniop(node, AstOperator::UniopCot);
    }

    void AstParenthesizer::visit(UniopCoth *node) {
        this->acceptUniop(node, AstOperator::UniopCoth);
    }

    void AstParenthesizer::visit(UniopSec *node) {
        this->acceptUniop(node, AstOperator::UniopSec);
    }

    void AstParenthesizer::visit(UniopSech *node) {
        this->acceptUniop(node, AstOperator::UniopSech);
    }

    void AstParenthesizer::visit(UniopCsc *node) {
        this->acceptUniop(node, AstOperator::UniopCsc);
    }

    void AstParenthesizer::visit(UniopCsch *node) {
        this->acceptUniop(node, AstOperator::UniopCsch);
    }

    void AstParenthesizer::visit(UniopArcsin *node) {
        this->acceptUniop(node, AstOperator::UniopArcsin);
    }

    void AstParenthesizer::visit(UniopArcsinh *node) {
        this->acceptUniop(node, AstOperator::UniopArcsinh);
    }

    void AstParenthesizer::visit(UniopArccos *node) {
        this->acceptUniop(node, AstOperator::UniopArccos);
    }

    void AstParenthesizer::visit(UniopArccosh *node) {
        this->acceptUniop(node, AstOperator::UniopArccosh);
    }

    void AstParenthesizer::visit(UniopArctan *node) {
        this->acceptUniop(node, AstOperator::UniopArctan);
    }

    void AstParenthesizer::visit(UniopArctanh *node) {
        this->acceptUniop(node, AstOperator::UniopArctanh);
    }

    void AstParenthesizer::visit(UniopArccot *node) {
        this->acceptUniop(node, AstOperator::UniopArccot);
    }

    void AstParenthesizer::visit(UniopArccoth *node) {
        this->acceptUniop(node, AstOperator::UniopArccoth);
    }

    void AstParenthesizer::visit(UniopArcsec *node) {
        this->acceptUniop(node, AstOperator::UniopArcsec);
    }

    void AstParenthesizer::visit(UniopArcsech *node) {
        this->acceptUniop(node, AstOperator::UniopArcsech);
    }

    void AstParenthesizer::visit(UniopArccsc *node) {
        this->acceptUniop(node, AstOperator::UniopArccsc);
    }

    void AstParenthesizer::visit(UniopArccsch *node) {
        this->acceptUniop(node, AstOperator::UniopArccsch);
    }

    void AstParenthesizer::visit(UniopHeaviside *node) {
        this->acceptUniop(node, AstOperator::UniopHeaviside);
    }

    void AstParenthesizer::visit(UniopSign *node) {
        this->acceptUniop(node, AstOperator::UniopSign);
    }

    void AstParenthesizer::visit(UniopFloor *node) {
        this->acceptUniop(node, AstOperator::UniopFloor);
    }

    void AstParenthesizer::visit(UniopCeiling *node) {
        this->acceptUniop(node, AstOperator::UniopCeiling);
    }

    void AstParenthesizer::visit(ScalarInt *node) {
        if (node->toInt() >= 0) {
            this->acceptEndNode(node, AstOperator::ScalarInt);
        } else {
            this->acceptEndNode(node, AstOperator::UniopMinus);
        }
    }

    void AstParenthesizer::visit(ScalarReal *node) {
        if (node->toDouble() >= 0) {
            this->acceptEndNode(node, AstOperator::ScalarReal);
        } else {
            this->acceptEndNode(node, AstOperator::UniopMinus);
        }
    }

    void AstParenthesizer::visit(ScalarBool *node) {
        this->acceptEndNode(node, AstOperator::ScalarBool);
    }

    void AstParenthesizer::visit(ScalarString *node) {
        this->acceptEndNode(node, AstOperator::ScalarString);
    }

    void AstParenthesizer::visit(BinopPlus *node) {
        this->acceptBinop(node, AstOperator::BinopPlus);
    }

    void AstParenthesizer::visit(BinopMinus *node) {
        this->acceptBinop(node, AstOperator::BinopMinus);
    }

    void AstParenthesizer::visit(BinopDivide *node) {
        this->acceptBinop(node, AstOperator::BinopDivide);
    }

    void AstParenthesizer::visit(BinopTimes *node) {
        this->acceptBinop(node, AstOperator::BinopTimes);
    }

    void AstParenthesizer::visit(BinopPower *node) {
        this->acceptBinop(node, AstOperator::BinopPower);
    }

    void AstParenthesizer::visit(BinopLogx *node) {
        this->acceptBinop(node, AstOperator::BinopLogx);
    }

    void AstParenthesizer::visit(BinopRoot *node) {
        this->acceptBinop(node, AstOperator::BinopRoot);
    }

    void AstParenthesizer::visit(BinopMin *node) {
        this->acceptBinop(node, AstOperator::BinopMin);
    }

    void AstParenthesizer::visit(BinopMax *node) {
        this->acceptBinop(node, AstOperator::BinopMax);
    }

    void AstParenthesizer::visit(BinopRem *node) {
        this->acceptBinop(node, AstOperator::BinopRem);
    }

    void AstParenthesizer::visit(BinopAtan2 *node) {
        this->acceptBinop(node, AstOperator::BinopAtan2);
    }

    void AstParenthesizer::visit(Pi *node) {
        this->acceptEndNode(node, AstOperator::Pi);
    }

    void AstParenthesizer::visit(Exponentiale *node) {
        this->acceptEndNode(node, AstOperator::Exponentiale);
    }

    void AstParenthesizer::visit(NullValue *node) {
        this->acceptEndNode(node, AstOperator::NullValue);
    }

    void AstParenthesizer::visit(LogicUniopIsdefined *node) {
        this->acceptUniop(node, AstOperator::LogicUniopIsdefined);
    }

    void AstParenthesizer::visit(LogicUniopNot *node) {
        this->acceptUniop(node, AstOperator::LogicUniopNot);
    }

    void AstParenthesizer::visit(LogicBinopLt *node) {
        this->acceptBinop(node, AstOperator::LogicBinopLt);
    }

    void AstParenthesizer::visit(LogicBinopLeq *node) {
        this->acceptBinop(node, AstOperator::LogicBinopLeq);
    }

    void AstParenthesizer::visit(LogicBinopGt *node) {
        this->acceptBinop(node, AstOperator::LogicBinopGt);
    }

    void AstParenthesizer::visit(LogicBinopGeq *node) {
        this->acceptBinop(node, AstOperator::LogicBinopGeq);
    }

    void AstParenthesizer::visit(LogicBinopEq *node) {
        this->acceptBinop(node, AstOperator::LogicBinopEq);
    }

    void AstParenthesizer::visit(LogicBinopNeq *node) {
        this->acceptBinop(node, AstOperator::LogicBinopNeq);
    }

    void AstParenthesizer::visit(LogicBinopAnd *node) {
        this->acceptBinop(node, AstOperator::LogicBinopAnd);
    }

    void AstParenthesizer::visit(LogicBinopOr *node) {
        this->acceptBinop(node, AstOperator::LogicBinopOr);
    }

    void AstParenthesizer::visit(LogicBinopXor *node) {
        this->acceptBinop(node, AstOperator::LogicBinopXor);
    }

    void AstParenthesizer::visit(Vector *node) {

    }

    void AstParenthesizer::visit(Piecewise *node) {
        this->parents.setProperties(this->node_properties[AstOperator::Piecewise]);
        this->parents.setNodeType(AstOperator::Piecewise);
        if (!this->requiresParentheses()) node->elideParentheses();
        for (Piece *piece : node->getPieces()) {
            piece->accept(this);
        }
    }

    void AstParenthesizer::visit(Piece *node) {
        this->parents.setProperties(this->node_properties[AstOperator::Piece]);
        this->parents.setNodeType(AstOperator::Piece);
        if (!this->requiresParentheses()) node->elideParentheses();
        if (!node->isOtherwise()) {
            node->getCondition()->accept(this);
        }
        node->getExpression()->accept(this);
    }

    void AstParenthesizer::visit(FunctionCall *node) {
        node->elideParentheses();
        SymbRef *func_ref = node->getFunction();
        func_ref->elideParentheses();
        for (FunctionArgument *farg : node->getFunctionArguments()) {
            farg->elideParentheses();
        }
    }

    void AstParenthesizer::visit(FunctionArgument *node) {

    }

    void AstParenthesizer::visit(Interval *node) {

    }
}
