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

#include "PKMacro.h"

namespace pharmmlcpp
{
    PKMacro::PKMacro(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->parse(reader, node);
        this->postParse();
    }

    void PKMacro::parse(PharmMLReader &reader, xml::Node node) {
        // Get type and all values of macro
        this->type = node.getName();
        std::vector<xml::Node> val_nodes = reader.getElements(node, "./mdef:Value");
        for (xml::Node val_node : val_nodes) {
            // Get (optional) argument name
            MacroValue value;
            value.first = val_node.getAttribute("argument").getValue();

            // Get Assign, Symbref or Scalar
            xml::Node assign_node = reader.getSingleElement(val_node, "./ct:Assign");
            if (assign_node.exists()) {
                xml::Node tree_node = assign_node.getChild();
                value.second = AstNode::create(reader, tree_node);
            } else if (val_node.getChild().exists()) {
                // SymbRef or Scalar
                value.second = AstNode::create(reader, val_node.getChild());
            } else {
                // TODO: Shouldn't this be schema illegal? Doesn't seem to stop me from crashing the code...
                value.second = nullptr;
            }

            this->values.push_back(value);
        }
    }

    std::string PKMacro::getType() {
        return this->type;
    }

    // Check if macro has a certain attribute
    bool PKMacro::hasAttribute(std::string attribute) {
        for (MacroValue value : this->values) {
            if (value.first == attribute) {
                return true;
            }
        }
        return false;
    }

    // Get pairs of all attributes and values
    std::vector<MacroValue> PKMacro::getValues() {
        return this->values;
    }

    // Find and get a specific attribute's assignment
    std::shared_ptr<AstNode> PKMacro::getAssignment(std::string attribute) {
        for (MacroValue value : this->values) {
            if (value.first == attribute) {
                return value.second;
            }
        }
        return nullptr;
    }

    void PKMacro::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        for (MacroValue value : this->values) {
            if (value.second) { // TODO: See above comment
                this->setupAstSymbRefs(value.second.get(), gathering, blkId);
            }
        }
    }

    void PKMacro::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    // POST PARSE/CONSOLIDATION
    // Perform post-parse functions to enable higher-level abstraction/consolidation
    void PKMacro::postParse() {
        // TODO: Hopefully validation and argument parsing can be done in here instead of in PKMacros/MDLGenerator

        // Get integer attributes
        int from_int, to_int, target_int, cmt_int, adm_int;
        std::unique_ptr<SymbRef> from_symbref, to_symbref, target_symbref;
        pharmmlcpp::AstAnalyzer ast_analyzer;
        for (MacroValue value : this->values) {
            // some values may either be a pure int (for compartment number) or a symbref (e.g. a derivative variable)
            auto get_symbref = [&](AstNode *x) -> std::unique_ptr<SymbRef> {
                ast_analyzer.reset();
                x->accept(&ast_analyzer);
                std::unique_ptr<SymbRef> symbref(ast_analyzer.getPureSymbRef());
                return symbref;
            };
            auto get_int = [&](AstNode *x) -> int {
                ast_analyzer.reset();
                x->accept(&ast_analyzer);
                pharmmlcpp::ScalarInt *scalar_int = ast_analyzer.getPureScalarInt();
                return scalar_int->toInt();
            };
            if (value.first == "from") {
                from_symbref = get_symbref( this->getAssignment(value.first).get() );
                if (!target_symbref) {
                    from_int = get_int( this->getAssignment(value.first).get() );
                }
            } else if (value.first == "to") {
                to_symbref = get_symbref( this->getAssignment(value.first).get() );
                if (!target_symbref) {
                    to_int = get_int( this->getAssignment(value.first).get() );
                }
            } else if (value.first == "target") {
                target_symbref = get_symbref( this->getAssignment(value.first).get() );
                if (!target_symbref) {
                    target_int = get_int( this->getAssignment(value.first).get() );
                }
            } else if (value.first == "cmt") {
                cmt_int = get_int( this->getAssignment(value.first).get() );
            } else if (value.first == "adm" || value.first == "type") {
                adm_int = get_int( this->getAssignment(value.first).get() );
            }
        }

        // Split into types and store symbref/integer attributes more logically
        if (this->type == "Compartment") {
            this->is_comp = true;
            this->sub_type = MacroType::Compartment;

            this->cmt_num = cmt_int;
        } else if (this->type == "Peripheral") {
            this->is_comp = true;
            this->sub_type = MacroType::Peripheral;

            // TODO: Parse kij and k_i_j to determine MacroType::Compartment linked. This is
            //       currently done in the MDL generator (which isn't very pretty at all)!
        } else if (this->type == "Effect") {
            this->is_comp = true;
            this->sub_type = MacroType::Effect;

            // TODO: cmt_int here means what?
        } else if (this->type == "Depot") {
            this->is_abs = true;
            this->sub_type = MacroType::Depot;

            this->adm_num = adm_int;
            if (target_symbref) {
                this->target_symbref = std::move(target_symbref);
            } else {
                this->target_cmt_num = target_int;
            }
        } else if (this->type == "IV") {
            this->is_abs = true;
            this->sub_type = MacroType::IV;

            this->adm_num = adm_int;
            this->target_cmt_num = cmt_int;
        } else if (this->type == "Absorption") {
            this->is_abs = true;
            this->sub_type = MacroType::Absorption;

            this->adm_num = adm_int;
            this->target_cmt_num = cmt_int;
        } else if (this->type == "Oral") {
            this->is_abs = true;
            this->sub_type = MacroType::Oral;

            this->adm_num = adm_int;
            this->target_cmt_num = cmt_int;
        } else if (this->type == "Elimination") {
            this->is_trans = true;
            this->sub_type = MacroType::Elimination;

            this->source_cmt_num = cmt_int;
        } else if (this->type == "Transfer") {
            this->is_trans = true;
            this->sub_type = MacroType::Transfer;

            this->source_cmt_num = from_int;
            if (from_symbref) {
                this->source_symbref = std::move(from_symbref);
            } else {
                this->source_cmt_num = from_int;
            }
            if (to_symbref) {
                this->target_symbref = std::move(to_symbref);
            } else {
                this->target_cmt_num = to_int;
            }
        }
    }

    // Generate a name for this macro via using the symbol attributes
    std::string PKMacro::generateName() {
        pharmmlcpp::AstAnalyzer ast_analyzer;
        if (this->sub_type == MacroType::Absorption || this->sub_type == MacroType::Depot) {
            // Try to get name from ka param
            AstNode *ka_node = this->getAssignment("ka").get();
            std::string ka_name;
            if (ka_node) {
                ast_analyzer.reset();
                ka_node->accept(&ast_analyzer);
                SymbRef *ref = ast_analyzer.getPureSymbRef();
                ka_name = ref ? ref->getSymbol()->getSymbId() : "";
            }

            // Try to get name from target param
            std::string target_name;
            if (this->target_symbref) {
                target_name = this->target_symbref->getSymbol()->getSymbId();
            } else if (this->target_cmt_num != 0) {
                target_name = "CMT" + std::to_string(this->target_cmt_num);
            }

            if (ka_name != "" || target_name != "") {
                std::string name = (ka_name == "") ? "INPUT_" + target_name : "INPUT_" + ka_name + "_" + target_name;
                return name;
            }
        } else if (this->sub_type == MacroType::Oral) {
            return("ORAL_INPUT");
        } else if (this->sub_type == MacroType::IV) {
            return("IV_INPUT");
        } else if (this->sub_type == MacroType::Compartment || this->sub_type == MacroType::Peripheral) {
            // Try to name in order: amount, cmt
            std::vector<AstNode *> nodes;
            nodes.push_back( this->getAssignment("amount").get() );
            nodes.push_back( this->getAssignment("cmt").get() );
            for (AstNode *node : nodes) {
                if (node) {
                    ast_analyzer.reset();
                    node->accept(&ast_analyzer);
                    SymbRef *ref = ast_analyzer.getPureSymbRef();
                    ScalarInt *sint = ast_analyzer.getPureScalarInt();
                    if (ref) {
                        Symbol *symbol = ref->getSymbol();
                        return(symbol->getSymbId());
                    } else if (sint) {
                        return("CMT" + sint->toString());
                    }
                }
            }
        } else if (this->sub_type == MacroType::Elimination || this->sub_type == MacroType::Transfer) {
            // Try to name in order: from, cmt
            std::vector<AstNode *> nodes;
            nodes.push_back( this->getAssignment("from").get() );
            nodes.push_back( this->getAssignment("cmt").get() );
            for (AstNode *node : nodes) {
                if (node) {
                    ast_analyzer.reset();
                    node->accept(&ast_analyzer);
                    SymbRef *ref = ast_analyzer.getPureSymbRef();
                    ScalarInt *sint = ast_analyzer.getPureScalarInt();
                    if (ref) {
                        Symbol *symbol = ref->getSymbol();
                        return("FROM_" + symbol->getSymbId());
                    } else if (sint) {
                        return("FROM_CMT" + sint->toString());
                    }
                }
            }
        } else if (this->sub_type == MacroType::Effect) {
            // Try to name from cmt param
            std::shared_ptr<AstNode> cmt = this->getAssignment("cmt");
            if (cmt) {
                ast_analyzer.reset();
                cmt->accept(&ast_analyzer);
                SymbRef *ref = ast_analyzer.getPureSymbRef();
                ScalarInt *sint = ast_analyzer.getPureScalarInt();
                if (ref) {
                    Symbol *symbol = ref->getSymbol();
                    return("EFF_" + symbol->getSymbId());
                } else if (sint) {
                    return("EFF_CMT" + sint->toString());
                }
            }
        }

        // We failed
        return("NAMELESS");
    }

    void PKMacro::setName(std::string name) {
        this->name = name;
    }

    std::string PKMacro::getName() {
        return this->name;
    }

    bool PKMacro::isCompartment() {
        return this->is_comp;
    }

    bool PKMacro::isAdministration() {
        return this->is_abs;
    }

    bool PKMacro::isMassTransfer() {
        return this->is_trans;
    }

    MacroType PKMacro::getSubType() {
        return this->sub_type;
    }

    // For compartments: Get referable integer number
    int PKMacro::getCmtNum() {
        return this->cmt_num;
    }

    // For compartments: Get referable integer number
    int PKMacro::getAdmNum() {
        return this->adm_num;
    }

    // For mass transfers: Get integer number reference (source), 0 -> use getSourceSymbRef() instead
    int PKMacro::getSourceNum() {
        return this->source_cmt_num;
    }

    // For mass transfers: Get SymbRef (source), nullptr -> use getSourceNum() instead
    SymbRef *PKMacro::getSourceSymbRef() {
        return this->source_symbref.get();
    }

    // For mass transfers/administrations: Get integer number reference (target); 0 -> use getSourceSymbRef() instead
    int PKMacro::getTargetNum() {
        return this->target_cmt_num;
    }

    // For mass transfers/administrations: Get SymbRef (target), nullptr -> use getTargetNum() instead
    SymbRef *PKMacro::getTargetSymbRef() {
        return this->target_symbref.get();
    }

    // Wrapping layer holding all macros and convenience functions
    PKMacros::PKMacros(PharmMLReader &reader, xml::Node node) {
        this->setXMLNode(node);
        this->parse(reader, node);
        //~ this->postParse(); // FIXME: See comment in Model.cpp!!!
    }

    void PKMacros::parse(PharmMLReader &reader, xml::Node node) {
        // Get all PK macros (nice that they are homologous, right?)
        std::vector<xml::Node> abs = reader.getElements(node, "./mdef:Absorption");
        std::vector<xml::Node> cmt = reader.getElements(node, "./mdef:Compartment");
        std::vector<xml::Node> dpt = reader.getElements(node, "./mdef:Depot");
        std::vector<xml::Node> eff = reader.getElements(node, "./mdef:Effect");
        std::vector<xml::Node> el = reader.getElements(node, "./mdef:Elimination");
        std::vector<xml::Node> iv = reader.getElements(node, "./mdef:IV");
        std::vector<xml::Node> orl = reader.getElements(node, "./mdef:Oral");
        std::vector<xml::Node> per = reader.getElements(node, "./mdef:Peripheral");
        std::vector<xml::Node> tra = reader.getElements(node, "./mdef:Transfer");

        // Pre-allocate for premature optimization
        std::vector<xml::Node> macro_nodes;
        macro_nodes.reserve(abs.size() + cmt.size() + dpt.size() + eff.size() + el.size() + iv.size() + orl.size() + per.size() + tra.size());
        macro_nodes.insert(macro_nodes.end(), abs.begin(), abs.end());
        macro_nodes.insert(macro_nodes.end(), cmt.begin(), cmt.end());
        macro_nodes.insert(macro_nodes.end(), dpt.begin(), dpt.end());
        macro_nodes.insert(macro_nodes.end(), eff.begin(), eff.end());
        macro_nodes.insert(macro_nodes.end(), el.begin(), el.end());
        macro_nodes.insert(macro_nodes.end(), iv.begin(), iv.end());
        macro_nodes.insert(macro_nodes.end(), orl.begin(), orl.end());
        macro_nodes.insert(macro_nodes.end(), per.begin(), per.end());
        macro_nodes.insert(macro_nodes.end(), tra.begin(), tra.end());

        // Construct one PKMacro object for each macro
        for (xml::Node macro_node : macro_nodes) {
            pharmmlcpp::PKMacro *macro = new PKMacro(reader, macro_node);
            this->macros.push_back(macro);
        }
    }

    std::vector<PKMacro *> PKMacros::getMacros() {
        return this->macros;
    }

    void PKMacros::setupRefererSymbRefs(SymbolGathering &gathering, std::string blkId) {
        for (PKMacro *macro : this->macros) {
            macro->setupSymbRefs(gathering, blkId);
        }
    }

    // POST PARSE/CONSOLIDATION
    // Perform post-parse functions to enable higher-level abstraction/consolidation
    void PKMacros::postParse() {
        std::unordered_set<std::string> picked_names;
        for (pharmmlcpp::PKMacro *macro : this->macros) {
            // Auto-generate a fitting name (hopefully useful for many end-tools but in particular, MDL)
            std::string name = macro->generateName();
            std::string uniq_name = name;
            size_t count = 2; // FIXME: Remove when perfect naming
            while (picked_names.count(uniq_name) > 0) {
                std::string suffix = "_" + std::to_string(count);
                uniq_name = name + suffix;
                count++;
            }
            macro->setName(uniq_name);
            picked_names.insert(uniq_name);
        }
    }

    // Validate the macros (where the schema won't help)
    void PKMacros::validate() { // FIXME: Who should call this now that consolidator is gone?
        AstAnalyzer ast_analyzer;
        // Map from compartment/administration to (referable) integer codes (and the referees themselves)
        std::unordered_map<std::string, std::unordered_map<int, PKMacro *>> int_codes = {
            { "cmt", std::unordered_map<int, PKMacro *>() },
            { "adm", std::unordered_map<int, PKMacro *>() },
        };

        // Check all macros
        for (PKMacro *macro : this->macros) {
            // Get type of macro ("Absorption", "Compartment", etc.)
            std::string type = macro->getType();

            // Check all attributes in macro
            for (MacroValue value : macro->getValues()) {
                // Get attribute type and assignment ("cmt" and 1, etc.)
                std::string attribute = value.first;
                std::shared_ptr<AstNode> assignment = value.second;

                if (!assignment) {
                    // Attribute without assignment
                    // FIXME: How to log in validate?
                    //this->context->logger.error("PK macro '" + type + "' (%a) contains a broken value (no content found)", macro, nullptr);
                } else if (attribute == "") {
                    // Assigment without attribute (type)
                    // FIXME: How to log?
                    //this->context->logger.warning("PK macro '" + type + "' (%a) contains an anonymous value (no attribute type)", macro, nullptr);
                } else if (attribute == "cmt" || attribute == "adm") {
                    // Check "cmt" and "adm" attributes
                    ast_analyzer.reset();
                    assignment->accept(&ast_analyzer);
                    ScalarInt *scalar_int = ast_analyzer.getPureScalarInt();
                    if (!scalar_int) {
                        // No ScalarInt child on "cmt" or "adm" attribute
                        //this->context->logger.error("PK macro '" + type + "' (%a) contains attribute '" + attribute + "' but value is not of type 'Int'", macro, nullptr);
                    } else if ( (attribute == "cmt" && macro->isCompartment()) || (attribute == "adm" && macro->isAdministration()) ) {
                        // Check if this integer is a duplicate of an earlier such integer found
                        int num = scalar_int->toInt();
                        auto got = int_codes[attribute].find(num);
                        if (got != int_codes[attribute].end()) {
                            //PKMacro *dup_macro = int_codes[attribute][num];
                            //this->context->logger.error("PK macro '" + type + "' (%a) shares referable attribute '"
                            //    + attribute + "=" + std::to_string(num) + "' illegally with a preceeding '" + dup_macro->getType() + "' (%b)", macro, dup_macro);
                        }
                        // Link attribute, integer and macro for above check
                        int_codes[attribute][num] = macro;
                    }
                }
            }
        }
    }

    // Get all compartment type macro's
    std::vector<PKMacro *> PKMacros::getCompartments() {
        std::vector<PKMacro *> cmt_macros;
        for (PKMacro *macro : this->macros) {
            // Find compartment
            std::string type = macro->getType();
            if (macro->isCompartment()) {
                cmt_macros.push_back(macro);
            }
        }
        return cmt_macros;
    }

    // Get all administration type macro's
    std::vector<PKMacro *> PKMacros::getAdministrations() {
        std::vector<PKMacro *> adm_macros;
        for (PKMacro *macro : this->macros) {
            // Find administration
            std::string type = macro->getType();
            if (macro->isAdministration()) {
                adm_macros.push_back(macro);
            }
        }
        return adm_macros;
    }

    // Get all mass transfer type macro's
    std::vector<PKMacro *> PKMacros::getTransfers() {
        std::vector<PKMacro *> trans_macros;
        for (PKMacro *macro : this->macros) {
            // Find mass transfers
            std::string type = macro->getType();
            if (macro->isMassTransfer()) {
                trans_macros.push_back(macro);
            }
        }
        return trans_macros;
    }

    // Find and return an administration from administration number
    PKMacro *PKMacros::getAdministration(int adm_num) {
        pharmmlcpp::AstAnalyzer ast_analyzer;
        for (PKMacro *macro : this->getAdministrations()) {
            // Find 'adm' attribute
            for (pharmmlcpp::MacroValue value : macro->getValues()) {
                if (value.first == "adm") {
                    // Get 'adm' code and resolve it
                    ast_analyzer.reset();
                    std::shared_ptr<AstNode> assignment = value.second;
                    assignment->accept(&ast_analyzer);
                    ScalarInt *scalar_int = ast_analyzer.getPureScalarInt();
                    // Compare and return
                    if (scalar_int->toInt() == adm_num) {
                        return macro;
                    }
                }
            }
        }
        return nullptr;
    }

    // Find and return a compartment from compartment number
    PKMacro *PKMacros::getCompartment(int cmt_num) {
        AstAnalyzer ast_analyzer;
        for (PKMacro *macro : this->getCompartments()) {
            // Find 'cmt' attribute
            for (MacroValue value : macro->getValues()) {
                if (value.first == "cmt") {
                    // Get 'cmt' code and resolve it
                    ast_analyzer.reset();
                    std::shared_ptr<AstNode> assignment = value.second;
                    assignment->accept(&ast_analyzer);
                    ScalarInt *scalar_int = ast_analyzer.getPureScalarInt();
                    // Compare and return
                    if (scalar_int->toInt() == cmt_num) {
                        return macro;
                    }
                }
            }
        }
        return nullptr;
    }

    PKMacro *PKMacros::getElimination(int cmt_num) {
        AstAnalyzer ast_analyzer;
        for (PKMacro *macro : this->getTransfers()) {
            if (macro->getSubType() == MacroType::Elimination) {
                for (MacroValue value : macro->getValues()) {
                    if (value.first == "cmt") {
                        ast_analyzer.reset();
                        std::shared_ptr<AstNode> assignment = value.second;
                        assignment->accept(&ast_analyzer);
                        ScalarInt *scalar_int = ast_analyzer.getPureScalarInt();
                        if (scalar_int->toInt() == cmt_num) {
                            return macro;
                        }
                    }
                }
            } 
        }
        return nullptr;
    }

    PKMacro *PKMacros::getAbsorption(int cmt_num) {
        AstAnalyzer ast_analyzer;
        for (PKMacro *macro : this->macros) {
            if (macro->getSubType() == MacroType::Absorption) {
                for (MacroValue value : macro->getValues()) {
                    if (value.first == "cmt") {
                        ast_analyzer.reset();
                        std::shared_ptr<AstNode> assignment = value.second;
                        assignment->accept(&ast_analyzer);
                        ScalarInt *scalar_int = ast_analyzer.getPureScalarInt();
                        if (scalar_int->toInt() == cmt_num) {
                            return macro;
                        }
                    }
                }
            } 
        }
        return nullptr;
    }

}
