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

#include "MultipleDVMapping.h"

namespace pharmmlcpp
{
    MultipleDVMapping::MultipleDVMapping(PharmMLReader &reader, xml::Node node) {
        this->parse(reader, node);
        setXMLNode(node);
    }

    void MultipleDVMapping::parse(PharmMLReader &reader, xml::Node node) {
        xml::Node ref_node = reader.getSingleElement(node, "ds:ColumnRef");
        this->column_id = ref_node.getAttribute("columnIdRef").getValue();
        xml::Node piecewise_node = reader.getSingleElement(node, "design:Piecewise");

        // Store and parse the piecewise to gather the mapping scheme
        this->piecewise = AstNode::create(reader, piecewise_node);
        this->parsePiecewiseNode(reader, piecewise_node);
    }

    void MultipleDVMapping::parsePiecewiseNode(PharmMLReader &reader, xml::Node node) {
        std::vector<xml::Node> pieces = node.getChildren();
        for (xml::Node piece : pieces) {
            xml::Node symbref_node = reader.getSingleElement(piece, "ct:SymbRef");
            xml::Node cond_node = reader.getSingleElement(piece, "math:Condition");

            if (symbref_node.exists()) {
                // Store this SymbRef
                std::unique_ptr<SymbRef> om_symbref = std::make_unique<SymbRef>(reader, symbref_node);

                // Get the logic operator
                xml::Node logic_node = cond_node.getChild();
                std::unique_ptr<AstNode> logic_ast_node = AstNode::create(reader, logic_node); // for logging
                std::string logic_node_type = logic_node.getName();
                std::string op_type;
                if (logic_node_type == "LogicUniop" || logic_node_type == "LogicBinop") {
                    op_type = logic_node.getAttribute("op").getValue();
                } else if (logic_node_type == "True") {
                    op_type = "True";
                } else if (logic_node_type == "False") {
                    op_type = "False";
                } else if (logic_node_type == "Otherwise") {
                    op_type = "Otherwise";
                }

                // Differentiate on the logic operator type
                if (op_type == "eq") {
                    // Equivalence operator found
                    xml::Node col_ref_node = reader.getSingleElement(logic_node, "ds:ColumnRef");
                    xml::Node int_node = reader.getSingleElement(logic_node, "ct:Int");
                    if (col_ref_node.exists() && int_node.exists()) {
                        // (dvid) column  and integer mapping code found; Piece is parseable
                        std::string dvid_col = ColumnRef(reader, col_ref_node).getColumnIdRef();
                        int code = ScalarInt(reader, int_node).toInt();

                        // map (dvid) column  to a list containing this code
                        if (auto got = dvid_column_codes.find(dvid_col) == dvid_column_codes.end()) {
                            this->dvid_column_codes.emplace(dvid_col, std::vector<int>());
                        }
                        this->dvid_column_codes[dvid_col].push_back(code);

                        // store observation model symbol as mapped by both (dvid) column  and the code
                        if (auto got = symbref_by_dvid_and_code.find(dvid_col) == symbref_by_dvid_and_code.end()) {
                            this->symbref_by_dvid_and_code.emplace(dvid_col, std::unordered_map<int, SymbRef *>());
                        }
                        if (auto got = symbref_by_dvid_and_code[dvid_col].find(code) == symbref_by_dvid_and_code[dvid_col].end()) {
                            this->symbref_by_dvid_and_code[dvid_col][code] = om_symbref.get();

                            // store the piece's observation model symbol and the (dvid) column
                            bool duplicate = false;
                            for (auto const &symbref : this->observation_model_symbrefs) {
                                if (symbref->getBlkIdRef() == om_symbref->getBlkIdRef() && symbref->getSymbIdRef() == om_symbref->getSymbIdRef()) {
                                    duplicate = true;
                                }
                            }
                            if (!duplicate) {
                                // don't pretend that different SymbRef elements are different observation model references...
                                this->observation_model_symbrefs.push_back(std::move(om_symbref));
                            }
                            this->dvid_columns.insert(dvid_col);
                        } else {
                            reader.logger.error("Piece in Piecewise in MultipleDVMapping maps multiple observations with same column and code; Second map ignored", logic_ast_node.get());
                        }
                    } else {
                        if (!col_ref_node.exists()) {
                            reader.logger.error("Equivalence operator in Piece in Piecewise in MultipleDVMapping: ColumnRef not found; Piece ignored", logic_ast_node.get());
                        } else {
                            reader.logger.error("Equivalence operator in Piece in Piecewise in MultipleDVMapping: Int not found; Piece ignored", logic_ast_node.get());
                        }
                    }
                } else {
                    reader.logger.error("Piece in Piecewise in MultipleDVMapping contains unsupported operator ('" + op_type + "'); Piece ignored", logic_ast_node.get());
                }
            } else {
                // Piece's allow a lot more elements than a SymbRef as the expression but only a SymbRef makes sense as an observation model reference
                reader.logger.error("Piece in Piecewise in MultipleDVMapping does not contain SymbRef (refering observation model symbol); This is unsupported", this->piecewise.get());
                continue;
            }

        }
    }

    std::vector<SymbRef *> MultipleDVMapping::getAllObservationSymbRefs() {
        std::vector<SymbRef *> symbrefs;
        for (auto const &symbref : this->observation_model_symbrefs) {
            symbrefs.push_back(symbref.get());
        }
        return symbrefs;
    }

    std::unordered_set<std::string> MultipleDVMapping::getAllMappingColumns() {
        return this->dvid_columns;
    }

    std::vector<int> MultipleDVMapping::getCodesInColumn(std::string col_id) {
        if (auto got = this->dvid_column_codes.find(col_id) == this->dvid_column_codes.end()) {
            return std::vector<int>();
        }
        return this->dvid_column_codes[col_id];
    }

    SymbRef *MultipleDVMapping::getObservationSymbRef(int code, std::string col_id) {
        if (auto got = this->symbref_by_dvid_and_code.find(col_id) != this->symbref_by_dvid_and_code.end()) {
            if (auto got = this->symbref_by_dvid_and_code[col_id].find(code) != this->symbref_by_dvid_and_code[col_id].end()) {
                return this->symbref_by_dvid_and_code[col_id][code];
            }
        }
        return nullptr;
    }

    xml::Node MultipleDVMapping::xml() {
        xml::Node mdm("MultipleDVMapping");
        xml::Node idref("ColumnRef", xml::Namespace::ds);
        idref.setAttribute("columnIdRef", this->column_id);
        mdm.addChild(idref);
        XMLAstVisitor xml;
        this->piecewise->accept(&xml);
        mdm.addChild(xml.getValue());
        return mdm;
    }

    void MultipleDVMapping::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        this->setupAstSymbRefs(this->piecewise.get(), gathering, blkId);
        for (auto const &om_symbref : this->observation_model_symbrefs) {
            this->addSymbRef(om_symbref.get(), gathering, blkId);
        }
    }

    void MultipleDVMapping::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
}
