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
    // Generate a name for this macro via using the symbol attributes
    std::string PKMacro::generateName() {
        PharmML::AstAnalyzer ast_analyzer;
        if (this->type == "Absorption" || this->type == "Depot") {
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
        } else if (this->type == "Oral") {
            return("INPUT_ORAL");
        } else if (this->type == "IV") {
            return("INPUT_IV");
        } else if (this->type == "Compartment" || this->type == "Peripheral") {
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
        } else if (this->type == "Elimination" || this->type == "Transfer") {
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
        } else if (this->type == "Effect") {
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

    // Wrapping layer holding all macros and convenience functions
    PKMacros::PKMacros(PharmMLContext *context, xml::Node node) {
        this->setXMLNode(node);
        this->context = context;
        this->parse(node);
        this->postParse();
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

    std::vector<PKMacro *> PKMacros::getAllMacros() {
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
        // Name the macros (hopefully useful for many end-tools but in particular, MDL)
        std::unordered_set<std::string> picked_names;
        for (PharmML::PKMacro *macro : this->macros) {
            // Auto-generate a fitting name
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
}
