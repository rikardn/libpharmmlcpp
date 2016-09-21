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

#include "MDLColumnMappingAstGenerator.h"
#include <generators/TextFormatter.h>

namespace pharmmlcpp
{
    // constructor
    MDLColumnMappingAstGenerator::MDLColumnMappingAstGenerator(std::shared_ptr<Logger> logger) : MDLAstGenerator(logger) {
        this->reset();
    }

    // public methods
    bool MDLColumnMappingAstGenerator::isPiecewiseMapped() {
        return this->piecewise_mapping;
    }

    // Get MDL formatted full mapping expression (e.g. "define = {1 in CMT as GUT, 2 in CMT as CENTRAL}" or "variable = GUT");
    std::string MDLColumnMappingAstGenerator::getColumnMappingExpression() {
        return this->mapped_full_expression;
    }

    // Get mapping comment (extra non-translateable expression, e.g. "# Additional conditions: (AMT > 0), (AMT > 0)"
    std::string MDLColumnMappingAstGenerator::getColumnMappingComment() {
        return this->mapped_comment;
    }

    // Get declared variables (e.g. <"GUT", "CENTRAL">)
    std::vector<std::string> MDLColumnMappingAstGenerator::getDeclaredVariables() {
        return this->declared_variables;
    }

    std::string MDLColumnMappingAstGenerator::accept(AstNode *node) {
        node->accept(this);
        return this->getValue();
    }

    // Parenthesize from a (root) node, accept it and return string
    std::string MDLColumnMappingAstGenerator::acceptRoot(AstNode *node) {
        node->accept(&this->parenthesizer);
        return this->accept(node);
    }

    // private methods
    void MDLColumnMappingAstGenerator::reset() {
        // Reset the internal state to a fully clean state
        this->value = "";
        this->piecewise_mapping = false;
        this->expression_mapping_mode = false;
        this->mapped_full_expression = "";
        this->mapped_comment = "";
        this->resetPiece();
    }

    void MDLColumnMappingAstGenerator::resetPiece() {
        // Reset the internal state for a fresh Piece accept
        this->mapped_target_name = "";
        this->mapped_num_code = "";
        this->mapped_code_column = "";
        this->first_condition_node_mode = true;
        this->contains_and_operator = false;
        this->contains_eq_operator = false;
        this->declared_variables.clear();
    }

    // visitor methods
    void MDLColumnMappingAstGenerator::visit(SymbRef *node) {
        std::string name;
        Symbol *symbol = node->getSymbol();
        if (symbol != nullptr) {
            name = symbol->getName();
        }
        if (this->piecewise_mapping && this->expression_mapping_mode) {
            this->mapped_target_name = name;
            this->declared_variables.push_back(name);
        } else {
            this->setValue(name);
            this->logger->warning("SymbRef encountered outside of Piece expression in Piecewise ColumnMapping", node);
        }
    }

    void MDLColumnMappingAstGenerator::visit(LogicBinopEq *node) {
        if (this->piecewise_mapping) {
            if (this->contains_and_operator && this->contains_eq_operator) {
                this->logger->error("Second 'eq' op detected in 'and' op in ColumnMapping Piecewise; First 'eq' interpreted as code column mapping", node);
                this->setValue(this->infix(node, " == "));
                return;
            }
            this->first_condition_node_mode = false;
            this->contains_eq_operator = true;

            bool left_node_is_code_column, right_node_is_code_column;
            // Accept left element of equality test
            std::string left_node = this->acceptRoot(node->getLeft());
            this->ast_analyzer.reset();
            node->getLeft()->accept(&this->ast_analyzer);
            left_node_is_code_column = ast_analyzer.getPureScalarInt() ? false : true;

            // Accept right element of equality test
            std::string right_node = this->acceptRoot(node->getRight());
            this->ast_analyzer.reset();
            node->getRight()->accept(&this->ast_analyzer);
            right_node_is_code_column = ast_analyzer.getPureScalarInt() ? false : true;

            // Determine which is code column and which is numerical code
            if (left_node_is_code_column == right_node_is_code_column) {
                this->logger->error("Integer code of 'eq' operator within ColumnMapping Piecewise could not be determined; Assumed to be second (right) element", node);
                left_node_is_code_column = true;
                right_node_is_code_column = false;
            }
            this->mapped_code_column = left_node_is_code_column ? left_node : right_node;
            this->mapped_num_code = right_node_is_code_column ? left_node : right_node;
        } else {
            this->setValue(this->infix(node, " && "));
        }
    }

    void MDLColumnMappingAstGenerator::visit(LogicBinopAnd *node) {
        if (this->piecewise_mapping) {
            if (this->first_condition_node_mode) {
                this->first_condition_node_mode = false;
                this->contains_and_operator = true;

                this->acceptRoot(node->getLeft());
                // Save extra condition for comment (not the 'eq' condition used for code_column and num_code)
                if (!this->contains_eq_operator) {
                    std::string peripheral_condition = this->getValue();
                    this->acceptRoot(node->getRight());
                    this->setValue(peripheral_condition);
                } else {
                    this->acceptRoot(node->getRight());
                }
            }
        } else {
            this->setValue(this->infix(node, " && "));
        }
    }

    void MDLColumnMappingAstGenerator::visit(Piecewise *node) {
        // Piecewise mapping detected, initialize parsing of this special (cumbersome) form
        this->piecewise_mapping = true;

        // Get and accept all pieces (one per code column, code and target)
        std::vector<Piece *> pieces = node->getPieces();
        if (pieces.size() == 1) {
            // If one piece there might not be a conditional mapping (e.g. AMT->D when AMT>0)
            this->resetPiece();
            pieces[0]->accept(this);
            if (this->mapped_num_code == "" && this->mapped_code_column == "") {
                // There's no mapping condition (except as a comment)
                this->mapped_full_expression = "variable = " + (this->mapped_target_name == "" ? "UNDEF" : this->mapped_target_name);
                this->mapped_comment = this->getValue() == "" ? "" : ("# " + this->getValue());
                return; // Return early (no define list needs to be created)
            }
        }
        std::vector<std::string> target_names, num_codes, code_columns, comments;
        bool has_comments = false;
        for (Piece *piece : pieces) {
            if (piece->isOtherwise()) {
                this->logger->error("Piecewise in ColumnMapping can't contain 'otherwise' condition (no MDL support); Piece ignored", piece);
            } else {
                this->resetPiece();
                piece->accept(this);
                target_names.push_back(this->mapped_target_name == "" ? "UNDEF" : this->mapped_target_name);
                num_codes.push_back(this->mapped_num_code == "" ? "UNDEF" : this->mapped_num_code);
                code_columns.push_back(this->mapped_code_column == "" ? "UNDEF" : this->mapped_code_column);

                // Add comments containing the additional logic to list
                if (this->getValue() != "") {
                    has_comments = true;
                    comments.push_back(this->getValue());
                } else {
                    comments.push_back("NONE");
                }
            }
        }

        // Create string for 'define' attribute (e.g. "1 in CMT as GUT")
        TextFormatter define_form, comment_form;
        define_form.openVector("{}", 0, ", ");
        comment_form.openVector("", 0, ", ");
        for (std::vector<int>::size_type i = 0; i != target_names.size(); i++) {
            define_form.add(num_codes[i] + " in " + code_columns[i] + " as " + target_names[i]);
            comment_form.add(comments[i]);
        }
        define_form.closeVector();
        comment_form.closeVector();

        define_form.noFinalNewline();
        comment_form.noFinalNewline();
        this->mapped_full_expression = "define = " + define_form.createString();
        this->mapped_comment = "# Additional conditions: " + comment_form.createString();

        this->setValue(""); // It should not be used, so flush it
    }

    void MDLColumnMappingAstGenerator::visit(Piece *node) {
        std::string target_name, num_code, code_column;

        // Check for (unsupported) nested Piecewise node
        this->ast_analyzer.reset();
        node->getExpression()->accept(&this->ast_analyzer);
        if (ast_analyzer.getPurePiecewise()) {
            this->logger->error("Piecewise in ColumnMapping can't contain nested Piecewise objects (no MDL support)", node->getExpression());
        } else {
            // Visit expression to fetch target name
            this->expression_mapping_mode = true;
            this->acceptRoot(node->getExpression());
            if (this->mapped_target_name == "") {
                this->logger->error("Piece in Piecewise in ColumnMapping did not contain any target", node);
            }
            this->expression_mapping_mode = false;

            // Visit condition to fetch code column and code
            this->acceptRoot(node->getCondition());
            this->mapped_comment = this->getValue();
        }

    }
}

