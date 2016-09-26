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
    /// Construct a visitor to parse Piecewise object syntax found in ColumnMapping for MDL translation. Extends MDLAstGenerator visitor.
    MDLColumnMappingAstGenerator::MDLColumnMappingAstGenerator(std::shared_ptr<Logger> logger) : MDLAstGenerator(logger) {
        this->reset();
    }

    /// Set the current column id before accepting Piecewise in ColumnMapping (as a reference for fetching results)
    void MDLColumnMappingAstGenerator::setColumnId(std::string id) {
        this->current_col_id = id;
    }

    /// Set the current column type before accepting Piecewise in ColumnMapping, i.e. from ColumnDefinition in DataSet (necessary for MDL logic to resolve fully)
    void MDLColumnMappingAstGenerator::setColumnType(std::string type) {
        if (type != "dose" && type != "covariate" && type != "reg" && type != "idv") {
            throw std::invalid_argument("type not 'dose', 'covariate', 'reg' or 'idv'");
        }
        if (type == "reg") {
            // TODO: Any difference between 'reg' and 'covariate'?
            this->current_col_type = "covariate";
        } else {
            this->current_col_type = type;
        }
    }

    /// Check if the specified column id has been sucessfully parsed as a piecewise style column mapping
    bool MDLColumnMappingAstGenerator::isPiecewiseMapped(std::string id) {
        auto got = this->mapped_columns.find(id);
        return (got != this->mapped_columns.end());
    }

    /**
     *  Get MDL formatted full mapping expression for a column id (e.g. "define = {1 in CMT as GUT, 2 in CMT as CENTRAL}" or "variable = GUT").
     *  Empty string returned if no mapping parsed or target shares name in "variable" form for covariates.
     */
    std::string MDLColumnMappingAstGenerator::getColumnMappingAttribute(std::string id) {
        auto got = this->mapped_columns.find(id);
        if (got == this->mapped_columns.end()) {
            return "";
        }

        std::string full_attribute;
        MappedColumn &column = this->mapped_columns[id];
        if (column.type == "dose" || column.type == "covariate") {
            // Mapping attribute only defined for dose and covariate columns
            if (!column.code_columns.empty()) {
                // Mapping codes means that the 'define' list attribute must be used
                TextFormatter form;
                form.openVector("define = {}", 0, ", ");
                for (std::string code_column : column.code_columns) {
                    for (auto code_target_pair : column.code_target_pairs[code_column]) {
                        form.add(code_target_pair.first + " in " + code_column + " as " + code_target_pair.second);
                        column.declared_variables.push_back(code_target_pair.second + "::dosingTarget");
                        column.dosing_targets.push_back(code_target_pair.second);
                    }
                }
                form.noFinalNewline();
                full_attribute = form.createString();
            } else {
                // No mapping codes means that the single 'variable' attribute must be used
                if (!(column.type == "covariate" && column.single_target == id)) {
                    std::string var = (column.single_target == "") ? "UNDEF" : column.single_target;
                    full_attribute = "variable = " + var;
                    column.declared_variables.push_back(var + "::dosingTarget");
                    column.dosing_targets.push_back(var);
                }
            }
        }
        return full_attribute;
    }

    /// Get mapping comment (extra non-translateable expression, e.g. "# (AMT > 0), (AMT > 0)"
    std::string MDLColumnMappingAstGenerator::getColumnMappingComment(std::string id) {
        auto got = this->mapped_columns.find(id);
        if (got == this->mapped_columns.end()) {
            return "";
        }

        std::string comment;
        MappedColumn column = this->mapped_columns[id];
        if (!column.extra_conditions.empty()) {
            if (!column.code_columns.empty()) {
                bool has_extra_conditions = false;
                TextFormatter form;
                form.openVector("", 0, ", ");
                for (std::string code_column : column.code_columns) {
                    for (std::string code : column.codes[code_column]) {
                        std::string cond = "NONE";
                        if (column.extra_conditions[code_column][code] != "") {
                            cond = column.extra_conditions[code_column][code];
                            has_extra_conditions = true;
                        }
                        form.add(cond);
                    }
                }
                form.noFinalNewline();
                comment = has_extra_conditions ? "# " + form.createString() : "";
            } else {
                std::string cond = column.single_extra_condition;
                comment = cond == "" ? "" : "# " + column.single_extra_condition;
            }
        }
        return comment;
    }

    /**
     *  Get data derived variables for columns that support it (idv). These are entries which referers
     *  backwards to another column's mapping definition, e.g. "DT : { use is doseTime, idvColumn=TIME, dosingVar=GUT }".
     */
    std::vector<std::string> MDLColumnMappingAstGenerator::getDataDerivedVariables(std::string id) {
        std::vector<std::string> derived_vars;
        auto got = this->mapped_columns.find(id);
        if (got == this->mapped_columns.end()) {
            return derived_vars;
        }

        MappedColumn &idv_column = this->mapped_columns[id];
        MappedColumn *dose_column = getMappedColumnOfType("dose");
        if (idv_column.type == "idv") {
            // Data derived variables only defined for idv columns
            if (!idv_column.codes.empty() && dose_column) {
                // Requires codes as well as a dose column (which is being implicitly refered)
                for (std::string code_column : idv_column.code_columns) {
                    for (auto code_target_pair : idv_column.code_target_pairs[code_column]) {
                        // Create a data derived variable statement for each code-target pair of idv column mapping
                        TextFormatter form;
                        form.noFinalNewline();
                        std::string code = code_target_pair.first;
                        std::string variable = code_target_pair.second;

                        form.openVector(variable + " : {}", 0, ", ");
                        form.add("use is doseTime");
                        form.add("idvColumn = " + id);
                        std::string dosing_var = getTargetMappedBy(*dose_column, code_column, code);
                        dosing_var = dosing_var == "" ? "UNDEF" : dosing_var;
                        form.add("dosingVar = " + dosing_var);
                        idv_column.declared_variables.push_back(dosing_var + "::dosingTarget");
                        idv_column.dosing_targets.push_back(dosing_var);
                        form.closeVector();
                        // form.append(" " + getColumnMappingComment(id));

                        derived_vars.push_back(form.createString());
                    }
                }
            } else {
                this->logger->error("Derived variable '" + idv_column.single_target + "' in Piecewise ColumnMapping 'idv' column, but no mapping resolved");
            }
        }
        return derived_vars;
    }

    /// Get declared variables for a column, including the type prefix (e.g. <"dosingTarget::GUT", "dosingTarget::CENTRAL">)
    std::vector<std::string> MDLColumnMappingAstGenerator::getDeclaredVariables(std::string id) {
        auto got = this->mapped_columns.find(id);
        if (got == this->mapped_columns.end()) {
            return std::vector<std::string>();
        } else {
            return got->second.declared_variables;
        }
    }

    /// Get declared dosing targets for a column (e.g. <"GUT", "CENTRAL">)
    std::vector<std::string> MDLColumnMappingAstGenerator::getDeclaredDosingTargets(std::string id) {
        auto got = this->mapped_columns.find(id);
        if (got == this->mapped_columns.end()) {
            return std::vector<std::string>();
        } else {
            return got->second.dosing_targets;
        }
    }

    /// General accept of an AstNode
    std::string MDLColumnMappingAstGenerator::accept(AstNode *node) {
        node->accept(this);
        return this->getValue();
    }

    /// Parenthesize from a (root) node, accept it and return string
    std::string MDLColumnMappingAstGenerator::acceptRoot(AstNode *node) {
        node->accept(&this->parenthesizer);
        return this->accept(node);
    }

    /// Get a specific parsed column by its type (useful for 'idv' and 'dose')
    MDLColumnMappingAstGenerator::MappedColumn *MDLColumnMappingAstGenerator::getMappedColumnOfType(std::string type) {
        for (auto mapped_column : this->mapped_columns) {
            if (mapped_column.second.type == type) {
                return &(mapped_columns[mapped_column.first]);
            }
        }
        return nullptr;
    }

    /// Get a target string (e.g. "GUT") from a parsed column, a code column and a code. Useful for data derived variables generation.
    std::string MDLColumnMappingAstGenerator::getTargetMappedBy(MappedColumn &column, std::string code_column, std::string code) {
        auto got = std::find(column.code_columns.begin(), column.code_columns.end(), code_column);
        if (got != column.code_columns.end()) {
            for (auto code_target_pair : column.code_target_pairs[code_column]) {
                if (code_target_pair.first == code) {
                    return code_target_pair.second;
                }
            }
        } else if (code_column == "" && code == "") {
            return column.single_target;
        }
        return "";
    }

    /// Reset the state of this visitor to a fully clean state as created by constructor
    void MDLColumnMappingAstGenerator::reset() {
        this->value = "";
        this->piecewise_mapping = false;
        this->current_col_id = "";
        this->current_col_type = "";
        this->mapped_columns.clear();
        this->resetPiece();
    }

    /// Reset the state of this visitor to be ready to accept a new Piece
    void MDLColumnMappingAstGenerator::resetPiece() {
        this->mapped_target_name = "";
        this->mapped_code = "";
        this->mapped_code_column = "";
        this->expression_mapping_mode = false;
        this->first_condition_node_mode = true;
        this->contains_and_operator = false;
        this->contains_eq_operator = false;
    }

    /// Accept a SymbRef (used to pull the target name)
    void MDLColumnMappingAstGenerator::visit(SymbRef *node) {
        std::string name;
        Symbol *symbol = node->getSymbol();
        if (symbol != nullptr) {
            name = symbol->getName();
        }
        if (this->piecewise_mapping && this->expression_mapping_mode) {
            this->mapped_target_name = name;
        } else {
            this->setValue(name);
            this->logger->warning("SymbRef encountered outside of Piece expression in Piecewise ColumnMapping", node);
        }
    }

    /// Accept a LogicBinopEq (used to map code column to code in a target Piece)
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
            this->mapped_code = right_node_is_code_column ? left_node : right_node;
        } else {
            this->setValue(this->infix(node, " == "));
        }
    }

    /// Accept a LogicBinopAnd (often preludes LogicBinopEq in conjunction with "(AMT > 0)", which is not parsed
    void MDLColumnMappingAstGenerator::visit(LogicBinopAnd *node) {
        if (this->piecewise_mapping) {
            if (this->first_condition_node_mode) {
                this->first_condition_node_mode = false;
                this->contains_and_operator = true;

                this->acceptRoot(node->getLeft());
                // Save extra condition for comment (not the 'eq' condition used for code_column and code)
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

    /// Accept a Piecewise (contains the whole mapping tree for a ColumnMapping)
    void MDLColumnMappingAstGenerator::visit(Piecewise *node) {
        // Type check (no duplicate dose or idv for example)
        std::string type = this->current_col_type;
        if (type == "") {
            return;
        } else {
            if ((type == "dose" || type == "idv") && getMappedColumnOfType(type)) {
                this->logger->error("Multiple Piecewise ColumnMapping for type '" + type + "' is not supported; Ignored", node);
                return;
            }
        }

        // Prepare data structure for result of parsing
        this->mapped_columns.emplace(this->current_col_id, MappedColumn{});
        MappedColumn &column = this->mapped_columns[this->current_col_id];
        column.type = this->current_col_type;

        // Piecewise mapping detected, initialize parsing of this special (cumbersome) form
        this->piecewise_mapping = true;

        // Get and accept all pieces (one per code column, code and target)
        std::vector<Piece *> pieces = node->getPieces();
        if (pieces.size() == 1) {
            // If one piece there might not be a conditional mapping (e.g. AMT->D when AMT>0)
            this->resetPiece();
            pieces[0]->accept(this);
            if (this->mapped_code == "") {
                // There's no mapping condition (except as a comment)
                column.single_target = this->mapped_target_name;
                column.single_extra_condition = this->getValue();
                return;
            }
        }
        std::vector<std::string> target_names, codes, code_columns, comments;
        for (Piece *piece : pieces) {
            if (piece->isOtherwise()) {
                this->logger->error("Piecewise in ColumnMapping can't contain 'otherwise' condition (no MDL support); Piece ignored", piece);
            } else {
                this->resetPiece();
                piece->accept(this);
                if (this->mapped_code != "") {
                    auto got = std::find(column.code_columns.begin(), column.code_columns.end(), this->mapped_code_column);
                    if (got == column.code_columns.end()) {
                        // Initialize with empty T in members which maps via this code column
                        column.codes.emplace(this->mapped_code_column, std::vector<std::string>());
                        column.code_target_pairs.emplace(this->mapped_code_column, std::vector<std::pair<std::string, std::string>>());
                        column.extra_conditions.emplace(this->mapped_code_column, std::unordered_map<std::string, std::string>());
                        column.code_columns.push_back(this->mapped_code_column);
                    }

                    // Add codes, code-target pairs and extra conditions if present
                    column.codes[this->mapped_code_column].push_back(this->mapped_code);
                    column.code_target_pairs[this->mapped_code_column].push_back(std::make_pair(this->mapped_code, this->mapped_target_name));
                    column.extra_conditions[this->mapped_code_column][this->mapped_code] = this->getValue();
                }
            }
        }

        this->setValue(""); // It should not be used, so flush it
    }

    /// Accept a Piece (contains the per-target mapping binding code column and code to it)
    void MDLColumnMappingAstGenerator::visit(Piece *node) {
        std::string target_name, code, code_column;

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
                this->logger->error("Piece in Piecewise in ColumnMapping does not contain any target", node);
            }
            this->expression_mapping_mode = false;

            // Visit condition to fetch code column and code
            this->acceptRoot(node->getCondition());
        }

    }
}

