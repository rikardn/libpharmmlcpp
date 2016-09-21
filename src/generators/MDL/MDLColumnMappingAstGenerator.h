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
#include <visitors/AstParenthesizer.h>
#include <visitors/AstNodeVisitor.h>
#include <generators/MDL/MDLAstGenerator.h>
#include <AST/symbols.h>
#include <AST/Uniop.h>
#include <AST/Binop.h>
#include <AST/Piecewise.h>
#include <PharmML/ColumnMapping.h>
#include <generators/TextFormatter.h>
#include <helpers/Logger.h>

namespace pharmmlcpp
{
    class MDLColumnMappingAstGenerator : public MDLAstGenerator
    {
        public:
            MDLColumnMappingAstGenerator(std::shared_ptr<Logger> logger);
            bool isPiecewiseMapped();
            std::string getColumnMappingExpression();
            std::string getColumnMappingComment();
            std::vector<std::string> getDeclaredVariables();

            void visit(SymbRef *node); // override
            void visit(LogicBinopEq *node); // override
            void visit(LogicBinopAnd *node); // override
            void visit(Piecewise *node); // override
            void visit(Piece *node); // override

        private:
            std::shared_ptr<Logger> logger;
            AstAnalyzer ast_analyzer;
            AstParenthesizer parenthesizer;
            void reset();
            void resetPiece();

            std::string accept(AstNode *node);
            std::string acceptRoot(AstNode *node);

            // Piecewise mapping state variables
            bool piecewise_mapping;
            bool expression_mapping_mode;
            bool first_condition_node_mode;
            bool contains_and_operator;
            bool contains_eq_operator;
            std::string mapped_target_name;
            std::string mapped_num_code;
            std::string mapped_code_column;

            // Results from full parsing
            std::string mapped_full_expression;
            std::string mapped_comment;
            std::vector<std::string> declared_variables;
    };
}

#endif
