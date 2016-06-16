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

    // Get current node properties
    const NodeProperties &NodePropertiesStack::getCurrentProperties() {
        return this->set_properties;
    }

    // Push set properties, push child direction and accept uniop node (traverse); pop node from stack when exiting
    void NodePropertiesStack::acceptUniop(Uniop *node) {
        this->setParenthesesProperty(node);

        this->properties.push_back(this->set_properties);
        this->directions.push_back(AcceptDirection::LeftChild);
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
        for (auto it = this->directions.begin(); it != this->directions.end(); ++it) {
            if ((*it) == AcceptDirection::RightChild) {
                auto dir_index = std::distance(this->directions.begin(), it);
                auto prop_it = this->properties.begin() + dir_index;
                return &(*prop_it);
            }
        }
        return nullptr;
    }

    // Fetch properties of the first parent to the right (in flattened AST)
    // (i.e., first parent whom not yet pushed node is a left child of)
    NodeProperties *NodePropertiesStack::getParentRight() {
        for (auto it = this->directions.begin(); it != this->directions.end(); ++it) {
            if ((*it) == AcceptDirection::LeftChild) {
                auto dir_index = std::distance(this->directions.begin(), it);
                auto prop_it = this->properties.begin() + dir_index;
                return &(*prop_it);
            }
        }
        return nullptr;
    }

    // Check if parent node left-parenthesizes last pushed node
    bool NodePropertiesStack::adjacentLeftParenthesis() {
        for (auto it = this->directions.begin()+1; it != this->directions.end(); ++it) {
            if ((*it) != AcceptDirection::LeftChild) break;
            auto dir_index = std::distance(this->directions.begin(), it);
            auto prop_it = this->properties.begin() + dir_index;
            if ((*prop_it).parenthesized == true) return true;
        }
        return false;
    }

    // Check if parent node right-parenthesizes last pushed node
    bool NodePropertiesStack::adjacentRightParenthesis() {
        for (auto it = this->directions.begin()+1; it != this->directions.end(); ++it) {
            if ((*it) != AcceptDirection::RightChild) break;
            auto dir_index = std::distance(this->directions.begin(), it);
            auto prop_it = this->properties.begin() + dir_index;
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
    AstParenthesizer::AstParenthesizer() {

    }

    // Determine if current node (not yet accepted) requires parentheses (using internal parent stack)
    bool AstParenthesizer::requiresParentheses() {
        // Root nodes never require parentheses
        if (this->parents.size() == 0) {
            return false;
        }

        // Properties of current (not yet pushed) node
        const NodeProperties &cur_props = parents.getCurrentProperties();

        // Check parent node to the "left" (which this node is RIGHT child of)
        NodeProperties *left_prop = this->parents.getParentLeft();
        if (left_prop) {
            // Only check rules if no parent node already protects with left "("
            if (!this->parents.adjacentLeftParenthesis()) {
                if (left_prop->priority > cur_props.priority) { // Lower priority requires ()
                    return true;
                } else if (left_prop->priority == cur_props.priority) { // Same priority might require ()
                    if (cur_props.associativity == NodeAssociativity::Left &&
                        left_prop->commutative == false) {
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
                    if (cur_props.associativity == NodeAssociativity::Right &&
                        right_prop->commutative == false) {
                        return true;
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
        this->parents.setProperties(node_properties[AstOperator::SymbRef]);
        if (!this->requiresParentheses()) node->elideParentheses();
    }

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
        if (!this->requiresParentheses()) node->elideParentheses();
    }

    void AstParenthesizer::visit(ScalarReal *node) {
        if (node->toDouble() >= 0) {
            this->parents.setProperties(node_properties[AstOperator::ScalarReal]);
        } else {
            this->parents.setProperties(node_properties[AstOperator::UniopMinus]);
        }
        if (!this->requiresParentheses()) node->elideParentheses();
    }

    void AstParenthesizer::visit(ScalarBool *node) { }

    void AstParenthesizer::visit(ScalarString *node) { }

    void AstParenthesizer::visit(BinopPlus *node) {
        const NodeProperties &props = node_properties[AstOperator::BinopPlus];
        this->parents.setProperties(props);
        if (!this->requiresParentheses()) node->elideParentheses();
        this->parents.acceptBinop(node);
    }

    void AstParenthesizer::visit(BinopMinus *node) {
        const NodeProperties &props = node_properties[AstOperator::BinopMinus];
        this->parents.setProperties(props);
        if (!this->requiresParentheses()) node->elideParentheses();
        this->parents.acceptBinop(node);
    }

    void AstParenthesizer::visit(BinopDivide *node) {
        const NodeProperties &props = node_properties[AstOperator::BinopDivide];
        this->parents.setProperties(props);
        if (!this->requiresParentheses()) node->elideParentheses();
        this->parents.acceptBinop(node);
    }

    void AstParenthesizer::visit(BinopTimes *node) {
        const NodeProperties &props = node_properties[AstOperator::BinopTimes];
        this->parents.setProperties(props);
        if (!this->requiresParentheses()) node->elideParentheses();
        this->parents.acceptBinop(node);
    }

    void AstParenthesizer::visit(BinopPower *node) {
        const NodeProperties &props = node_properties[AstOperator::BinopPower];
        this->parents.setProperties(props);
        if (!this->requiresParentheses()) node->elideParentheses();
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
