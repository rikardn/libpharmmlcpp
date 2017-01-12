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

#ifndef PHARMMLCPP_MDLCOLUMNMAPPINGASTGENERATOR_H_
#define PHARMMLCPP_MDLCOLUMNMAPPINGASTGENERATOR_H_

#include <string>
#include <exception>
#include <visitors/AstParenthesizer.h>
#include <visitors/AstNodeVisitor.h>
#include "MDLAstGenerator.h"
#include <AST/symbols.h>
#include <AST/Uniop.h>
#include <AST/Binop.h>
#include <AST/Piecewise.h>
#include <PharmML/ColumnMapping.h>
#include <generators/TextFormatter.h>
#include <helpers/Logger.h>

namespace pharmmlcpp
{
    /// Visitor to parse Piecewise object syntax found in ColumnMapping for MDL translation. Extends MDLAstGenerator visitor.
    class MDLColumnMappingAstGenerator : public MDLAstGenerator
    {
        public:
            MDLColumnMappingAstGenerator(std::shared_ptr<Logger> logger);
            void setColumnId(std::string id);
            void setColumnType(std::string type);

            bool isPiecewiseMapped(std::string id);
            std::string getColumnMappingAttribute(std::string id);
            std::string getColumnMappingComment(std::string id);
            std::vector<std::string> getDataDerivedVariables(std::string id);
            std::vector<std::string> getDeclaredVariables(std::string id);
            std::vector<std::string> getDeclaredDosingTargets(std::string id);

            void visit(SymbRef *node); ///< overrides MDLAstGenerator::visit(SymbRef *node)
            void visit(LogicBinopEq *node); ///< overrides MDLAstGenerator::visit(LogicBinopEq *node)
            void visit(LogicBinopAnd *node); ///< overrides MDLAstGenerator::visit(LogicBinopAnd *node)
            void visit(Piecewise *node); ///< overrides MDLAstGenerator::visit(Piecewise *node)
            void visit(Piece *node); ///< overrides MDLAstGenerator::visit(Piece *node)

        private:
            std::shared_ptr<Logger> logger;
            AstAnalyzer ast_analyzer;
            AstParenthesizer parenthesizer;
            void reset();
            void resetPiece();

            std::string accept(AstNode *node);
            std::string acceptRoot(AstNode *node);


            //@{
            /** Piecewise mapping current column variables */
            std::string current_col_id;
            std::string current_col_type;
            //@}

            //@{
            /** Piecewise mapping state variables */
            bool piecewise_mapping;
            bool expression_mapping_mode;
            bool first_condition_node_mode;
            bool contains_and_operator;
            bool contains_eq_operator;
            std::string mapped_target_name;
            std::string mapped_code;
            std::string mapped_code_column;
            //@}

            //@{
            /** Results from full parsing of a ColumnMapping */
            struct MappedColumn {
                std::string type;
                std::string single_target, single_extra_condition; ///< single target/additional condition if no code column/code exists
                std::vector<std::string> code_columns; ///< set of all code columns
                std::unordered_map<std::string, std::vector<std::string>> codes; ///< all codes, mapped by code column
                std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> code_target_pairs; ///< all code-target pairs, mapped by code column
                std::unordered_map<std::string, std::unordered_map<std::string, std::string>> extra_conditions; ///< additional conditions, mapped by code column and then code
                std::vector<std::string> declared_variables;
                std::vector<std::string> dosing_targets;
            };
            //@}
            std::unordered_map<std::string, MappedColumn> mapped_columns; ///< all columns parsed, mapped by column id

            MDLColumnMappingAstGenerator::MappedColumn *getMappedColumnOfType(std::string type);
            std::string getTargetMappedBy(MappedColumn &column, std::string code_column = "", std::string code = "");
    };
}

#endif
