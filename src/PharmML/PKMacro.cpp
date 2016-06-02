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

namespace PharmML
{
    PKMacro::PKMacro(PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
        this->postParse();
    }

    void PKMacro::parse(xml::Node node) {
        // Get type and all values of macro
        this->type = node.getName();
        std::vector<xml::Node> val_nodes = this->context->getElements(node, "./mdef:Value");
        for (xml::Node val_node : val_nodes) {
            // Get (optional) argument name
            MacroValue value;
            value.first = val_node.getAttribute("argument").getValue();

            // Get Assign, Symbref or Scalar
            xml::Node assign_node = this->context->getSingleElement(val_node, "./ct:Assign");
            if (assign_node.exists()) {
                xml::Node tree_node = assign_node.getChild();
                value.second = this->context->factory.create(tree_node);
            } else if (val_node.getChild().exists()) {
                // SymbRef or Scalar
                value.second = this->context->factory.create(val_node.getChild());
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
    AstNode *PKMacro::getAssignment(std::string attribute) {
        for (MacroValue value : this->values) {
            if (value.first == attribute) {
                return value.second;
            }
        }
        return nullptr;
    }

    void PKMacro::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        for (MacroValue value : this->values) {
            if (value.second) { // TODO: See above comment
                std::unordered_set<Symbol *> symbols = this->symbRefsFromAst(value.second, symbolMap);
                this->referencedSymbols.addSymbols(symbols);
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
        if (this->type == "Compartment") {
            this->is_comp = true;
            this->sub_type = MacroType::Compartment;
        } else if (this->type == "Peripheral") {
            this->is_comp = true;
            this->sub_type = MacroType::Peripheral;
        } else if (this->type == "Effect") {
            this->is_comp = true;
            this->sub_type = MacroType::Effect;
        } else if (this->type == "Depot") {
            this->is_abs = true;
            this->sub_type = MacroType::Depot;
        } else if (this->type == "IV") {
            this->is_abs = true;
            this->sub_type = MacroType::IV;
        } else if (this->type == "Absorption") {
            this->is_abs = true;
            this->sub_type = MacroType::Absorption;
        } else if (this->type == "Oral") {
            this->is_abs = true;
            this->sub_type = MacroType::Oral;
        } else if (this->type == "Elimination") {
            this->is_trans = true;
            this->sub_type = MacroType::Elimination;
        } else if (this->type == "Transfer") {
            this->is_trans = true;
            this->sub_type = MacroType::Transfer;
        }
    }

    // Generate a name for this macro via using the symbol attributes
    std::string PKMacro::generateName() {
        PharmML::AstAnalyzer ast_analyzer;
        if (this->sub_type == MacroType::Absorption || this->sub_type == MacroType::Depot) {
            // Try to name in order: ka, Tlag, p
            std::vector<PharmML::AstNode *> nodes;
            nodes.push_back( this->getAssignment("ka") );
            nodes.push_back( this->getAssignment("Tlag") );
            nodes.push_back( this->getAssignment("p") );
            for (PharmML::AstNode *node : nodes) {
                if (node) {
                    ast_analyzer.reset();
                    node->accept(&ast_analyzer);
                    PharmML::SymbRef *ref = ast_analyzer.getPureSymbRef();
                    if (ref) {
                        PharmML::Symbol *symbol = ref->getSymbol();
                        if (symbol) { // FIXME: Shouldn't be necessary
                            return("INPUT_" + symbol->getSymbId());
                        }
                    }
                }
            }
        } else if (this->sub_type == MacroType::Oral) {
            return("INPUT_ORAL");
        } else if (this->sub_type == MacroType::IV) {
            return("INPUT_IV");
        } else if (this->sub_type == MacroType::Compartment || this->sub_type == MacroType::Peripheral) {
            // Try to name in order: amount, cmt
            std::vector<PharmML::AstNode *> nodes;
            nodes.push_back( this->getAssignment("amount") );
            nodes.push_back( this->getAssignment("cmt") );
            for (PharmML::AstNode *node : nodes) {
                if (node) {
                    ast_analyzer.reset();
                    node->accept(&ast_analyzer);
                    PharmML::SymbRef *ref = ast_analyzer.getPureSymbRef();
                    PharmML::ScalarInt *sint = ast_analyzer.getPureScalarInt();
                    if (ref) {
                        PharmML::Symbol *symbol = ref->getSymbol();
                        if (symbol) { // FIXME: Shouldn't be necessary
                            return(symbol->getSymbId());
                        }
                    } else if (sint) {
                        return("CMT" + sint->toString());
                    }
                }
            }
        } else if (this->sub_type == MacroType::Elimination || this->sub_type == MacroType::Transfer) {
            // Try to name in order: from, cmt
            std::vector<PharmML::AstNode *> nodes;
            nodes.push_back( this->getAssignment("from") );
            nodes.push_back( this->getAssignment("cmt") );
            for (PharmML::AstNode *node : nodes) {
                if (node) {
                    ast_analyzer.reset();
                    node->accept(&ast_analyzer);
                    PharmML::SymbRef *ref = ast_analyzer.getPureSymbRef();
                    PharmML::ScalarInt *sint = ast_analyzer.getPureScalarInt();
                    if (ref) {
                        PharmML::Symbol *symbol = ref->getSymbol();
                        if (symbol) { // FIXME: Shouldn't be necessary
                            return("FROM_" + symbol->getSymbId());
                        }
                    } else if (sint) {
                        return("FROM_CMT" + sint->toString());
                    }
                }
            }
        } else if (this->sub_type == MacroType::Effect) {
            // Try to name in order: cmt
            PharmML::AstNode *cmt = this->getAssignment("cmt");
            if (cmt) {
                ast_analyzer.reset();
                cmt->accept(&ast_analyzer);
                PharmML::SymbRef *ref = ast_analyzer.getPureSymbRef();
                PharmML::ScalarInt *sint = ast_analyzer.getPureScalarInt();
                if (ref) {
                    PharmML::Symbol *symbol = ref->getSymbol();
                    if (symbol) { // FIXME: Shouldn't be necessary
                        return("EFF_" + symbol->getSymbId());
                    }
                } else if (sint) {
                    return("CMT" + sint->toString() + "_EFF");
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

    bool PKMacro::isAbsorptionProcess() {
        return this->is_abs;
    }

    bool PKMacro::isMassTransfer() {
        return this->is_trans;
    }

    MacroType PKMacro::getSubType() {
        return this->sub_type;
    }

    // Wrapping layer holding all macros and convenience functions
    PKMacros::PKMacros(PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
        //~ this->postParse(); // FIXME: See comment in Model.cpp!!!
    }

    void PKMacros::parse(xml::Node node) {
        // Get all PK macros (nice that they are homologous, right?)
        std::vector<xml::Node> abs = this->context->getElements(node, "./mdef:Absorption");
        std::vector<xml::Node> cmt = this->context->getElements(node, "./mdef:Compartment");
        std::vector<xml::Node> dpt = this->context->getElements(node, "./mdef:Depot");
        std::vector<xml::Node> eff = this->context->getElements(node, "./mdef:Effect");
        std::vector<xml::Node> el = this->context->getElements(node, "./mdef:Elimination");
        std::vector<xml::Node> iv = this->context->getElements(node, "./mdef:IV");
        std::vector<xml::Node> orl = this->context->getElements(node, "./mdef:Oral");
        std::vector<xml::Node> per = this->context->getElements(node, "./mdef:Peripheral");
        std::vector<xml::Node> tra = this->context->getElements(node, "./mdef:Transfer");

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
            PharmML::PKMacro *macro = new PharmML::PKMacro(this->context, macro_node);
            this->macros.push_back(macro);
        }
    }

    std::vector<PKMacro *> PKMacros::getMacros() {
        return this->macros;
    }

    void PKMacros::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        for (PKMacro *macro : this->macros) {
            macro->gatherSymbRefs(symbolMap);
        }
    }

    // POST PARSE/CONSOLIDATION
    // Perform post-parse functions to enable higher-level abstraction/consolidation
    void PKMacros::postParse() {
        std::unordered_set<std::string> picked_names;
        for (PharmML::PKMacro *macro : this->macros) {
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
        PharmML::AstAnalyzer ast_analyzer;
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
            for (PharmML::MacroValue value : macro->getValues()) {
                // Get attribute type and assignment ("cmt" and 1, etc.)
                std::string attribute = value.first;
                PharmML::AstNode *assignment = value.second;

                if (!assignment) {
                    // Attribute without assignment
                    this->context->logger.error("PK macro '" + type + "' (%a) contains a broken value (no content found)", macro, nullptr);
                } else if (attribute == "") {
                    // Assigment without attribute (type)
                    this->context->logger.warning("PK macro '" + type + "' (%a) contains an anonymous value (no attribute type)", macro, nullptr);
                } else if (attribute == "cmt" || attribute == "adm") {
                    // Check "cmt" and "adm" attributes
                    ast_analyzer.reset();
                    assignment->accept(&ast_analyzer);
                    PharmML::ScalarInt *scalar_int = ast_analyzer.getPureScalarInt();
                    if (!scalar_int) {
                        // No ScalarInt child on "cmt" or "adm" attribute
                        this->context->logger.error("PK macro '" + type + "' (%a) contains attribute '" + attribute + "' but value is not of type 'Int'", macro, nullptr);
                    } else if ( (attribute == "cmt" && macro->isCompartment()) || (attribute == "adm" && macro->isAbsorptionProcess()) ) {
                        // Check if this integer is a duplicate of an earlier such integer found
                        int num = scalar_int->toInt();
                        auto got = int_codes[attribute].find(num);
                        if (got != int_codes[attribute].end()) {
                            PKMacro *dup_macro = int_codes[attribute][num];
                            this->context->logger.error("PK macro '" + type + "' (%a) shares referable attribute '"
                                + attribute + "=" + std::to_string(num) + "' illegally with a preceeding '" + dup_macro->getType() + "' (%b)", macro, dup_macro);
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

    // Get all administration/absorption type macro's
    std::vector<PKMacro *> PKMacros::getAdministrations() {
        std::vector<PKMacro *> adm_macros;
        for (PKMacro *macro : this->macros) {
            // Find administration
            std::string type = macro->getType();
            if (macro->isAbsorptionProcess()) {
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

    // Find and return an administration/absorption from administration number
    PKMacro *PKMacros::getAdministration(int adm_num) {
        PharmML::AstAnalyzer ast_analyzer;
        for (PKMacro *macro : this->getAdministrations()) {
            // Find 'adm' attribute
            for (PharmML::MacroValue value : macro->getValues()) {
                if (value.first == "adm") {
                    // Get 'adm' code and resolve it
                    ast_analyzer.reset();
                    PharmML::AstNode *assignment = value.second;
                    assignment->accept(&ast_analyzer);
                    PharmML::ScalarInt *scalar_int = ast_analyzer.getPureScalarInt();
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
        PharmML::AstAnalyzer ast_analyzer;
        for (PKMacro *macro : this->getCompartments()) {
            // Find 'cmt' attribute
            for (PharmML::MacroValue value : macro->getValues()) {
                if (value.first == "cmt") {
                    // Get 'cmt' code and resolve it
                    ast_analyzer.reset();
                    PharmML::AstNode *assignment = value.second;
                    assignment->accept(&ast_analyzer);
                    PharmML::ScalarInt *scalar_int = ast_analyzer.getPureScalarInt();
                    // Compare and return
                    if (scalar_int->toInt() == cmt_num) {
                        return macro;
                    }
                }
            }
        }
        return nullptr;
    }
}
