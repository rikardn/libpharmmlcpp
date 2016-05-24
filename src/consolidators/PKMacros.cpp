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

#include "PKMacros.h"

namespace CPharmML
{
    // Wrapping class for PharmML::PKMacro's (mostly because PharmML is terribly non-namey here)
    PKMacro::PKMacro(PharmML::PKMacro *macro) {
        this->macro = macro;
        //~ for (macro->hasAttribute(
    }
    
    PharmML::PKMacro *PKMacro::getMacro() {
        return this->macro;
    }
    
    bool PKMacro::hasAttribute(std::string attribute) {
        return this->macro->hasAttribute(attribute);
    }

    PharmML::AstNode *PKMacro::getAttribute(std::string attribute) {
        return this->macro->getAssignment(attribute);
    }

    std::string PKMacro::getName() {
        return this->name;
    }
    
    void PKMacro::setName(std::string name) {
        this->name = name;
    }

    // Generate a name for this macro via using the symbol attributes
    std::string PKMacro::generateName(PharmML::AstAnalyzer &ast_analyzer) {
        std::string n = macro->getName();
        if (n == "Absorption" || n == "Depot") {
            // Try to name in order: ka, Tlag, p
            std::vector<PharmML::AstNode *> nodes;
            nodes.push_back( this->macro->getAssignment("ka") );
            nodes.push_back( this->macro->getAssignment("Tlag") );
            nodes.push_back( this->macro->getAssignment("p") );
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
        } else if (n == "Oral") {
            return("INPUT_ORAL");
        } else if (n == "IV") {
            return("INPUT_IV");
        } else if (n == "Compartment" || n == "Peripheral") {
            // Try to name in order: amount, cmt
            std::vector<PharmML::AstNode *> nodes;
            nodes.push_back( this->macro->getAssignment("amount") );
            nodes.push_back( this->macro->getAssignment("cmt") );
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
        } else if (n == "Elimination" || n == "Transfer") {
            // Try to name in order: from, cmt
            std::vector<PharmML::AstNode *> nodes;
            nodes.push_back( this->macro->getAssignment("from") );
            nodes.push_back( this->macro->getAssignment("cmt") );
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
        } else if (name == "Effect") {
            // Try to name in order: cmt
            PharmML::AstNode *cmt = this->macro->getAssignment("cmt");
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
    
    // Convenience function; try to parse an attribute into int
    // FIXME: Depreceate somehow or put (general for AstNode's) into src/helpers (or AstAnalyzer)
    bool PKMacro::tryParseInt(std::string attribute, int &result, PharmML::AstAnalyzer &ast_analyzer) {
        ast_analyzer.reset();
        PharmML::AstNode *node = this->getMacro()->getAssignment(attribute);
        if (!node) {
            return false;
        }
        node->accept(&ast_analyzer);
        PharmML::ScalarInt *sint_adm = ast_analyzer.getPureScalarInt();
        if (!sint_adm) {
            return false;
        }
        result = sint_adm->toInt();
        return true;
    }
    
    // Construct with PKMacro's as base
    PKMacros::PKMacros(std::vector<PharmML::PKMacro *> macros, std::shared_ptr<PharmML::Logger> logger) {
        this->logger = logger;
        
        // Collect CPharmML wrapping objects
        this->cmacros.reserve(macros.size());
        std::unordered_set<std::string> picked_names;
        for (PharmML::PKMacro *macro : macros) {
            PKMacro *cmacro = new PKMacro(macro);
            
            // Auto-generate a fitting name
            std::string name = cmacro->generateName(this->ast_analyzer);
            std::string uniq_name = name;
            size_t count = 2; // FIXME: Remove when perfect naming
            while (picked_names.count(uniq_name) > 0) {
                std::string suffix = "_" + std::to_string(count);
                uniq_name = name + suffix;
                count++;
            }
            cmacro->setName(uniq_name);
            picked_names.insert(uniq_name);
            
            // Store the new wrapping object
            this->cmacros.push_back(cmacro);
        }
    }
    
    // Validate the internals
    void PKMacros::validate() {
        // Map from name of attribute to (referable) integer codes (and the referables)
        std::unordered_map<std::string, std::unordered_map<int, PKMacro *>> int_codes = {
            { "cmt", std::unordered_map<int, PKMacro *>() },
            { "adm", std::unordered_map<int, PKMacro *>() },
        };
        
        // Check all macros
        for (PKMacro *cmacro : this->cmacros) {
            // Get name of macro ("Absorption", "Compartment", etc.)
            PharmML::PKMacro *macro = cmacro->getMacro();
            std::string name = macro->getName();
            
            // Check all values in macro
            for (PharmML::MacroValue value : macro->getValues()) {
                // Get attribute name and assignment ("cmt" and 1, etc.)
                std::string attribute = value.first;
                PharmML::AstNode *assignment = value.second;
                
                if (!assignment) {
                    // TODO: See comment in PKMacros.cpp; Schema doesn't seem to block this!
                    this->logger->error("PK macro '" + name + "' (%a) contains a broken value: No content found", macro, nullptr);
                } else if (attribute == "") {
                    // Anonymous attribute
                    this->logger->warning("PK macro '" + name + "' (%a) contains an anonymous value (no attribute type)", macro, nullptr);
                } else if (attribute == "cmt" || attribute == "adm") {
                    // Check if "cmt" and "adm" attributes contain an integer code
                    this->ast_analyzer.reset();
                    assignment->accept(&ast_analyzer);
                    PharmML::ScalarInt *scalar_int = ast_analyzer.getPureScalarInt();
                    if (!scalar_int) {
                        this->logger->error("PK macro '" + name + "' (%a) contains attribute '" + attribute + "' but value is not of type 'Int'", macro, nullptr);
                    } else if ((attribute == "cmt" && name == "Compartment") || ( (attribute == "adm") && (name == "Absorption" || name == "IV" || name == "Depot" || name == "Oral") )) {
                        // Check if there's a (duplicate) referable integer code registred
                        int num = scalar_int->toInt();
                        auto got = int_codes[attribute].find(num);
                        if (got != int_codes[attribute].end()) {
                            PKMacro *dup_cmacro = int_codes[attribute][num];
                            PharmML::PKMacro *dup_macro = dup_cmacro->getMacro();
                            this->logger->error("PK macro '" + name + "' (%a) shares identifier '"
                                + attribute + "=" + std::to_string(num) + "' illegally with a preceeding '" + dup_macro->getName() + "' (%b)", macro, dup_macro);
                        }
                        
                        // Link attribute, code and cmacro for above check
                        int_codes[attribute][num] = cmacro;
                    }
                }
            }
        }
    }
    
    // Check if this model has PK macros
    bool PKMacros::exists() {
        return !this->cmacros.empty();
    }
    
    // Get wrapping objects (CPharmML::PKMacro*)
    std::vector<PKMacro *> PKMacros::getMacros() {
        return this->cmacros;
    }
    
    // Get all compartment type macro's
    std::vector<PKMacro *> PKMacros::getCompartments() {
        std::vector<PKMacro *> cmts;
        for (PKMacro *cmacro : this->cmacros) {
            // Find compartment
            PharmML::PKMacro *macro = cmacro->getMacro();
            std::string name = macro->getName();
            if (name == "Compartment" || name == "Peripheral" || name == "Effect") {
                cmts.push_back(cmacro);
            }
        }
        return cmts;
    }
    
    // Find and return a compartment from compartment number
    PKMacro *PKMacros::getCompartment(int cmt_num) {
        for (PKMacro *cmacro : this->getCompartments()) {
            PharmML::PKMacro *macro = cmacro->getMacro();
            // Find 'cmt' attribute
            for (PharmML::MacroValue value : macro->getValues()) {
                if (value.first == "cmt") {
                    // Get 'cmt' code and resolve it
                    this->ast_analyzer.reset();
                    PharmML::AstNode *assignment = value.second;
                    assignment->accept(&ast_analyzer);
                    PharmML::ScalarInt *scalar_int = ast_analyzer.getPureScalarInt();
                    // Compare and return
                    if (scalar_int->toInt() == cmt_num) {
                        return cmacro;
                    }
                }
            }
        }
        return nullptr;
    }
    
    // Get all administration/absorption type macro's
    std::vector<PKMacro *> PKMacros::getAdministrations() {
        std::vector<PKMacro *> adms;
        for (PKMacro *cmacro : this->cmacros) {
            // Find administration
            PharmML::PKMacro *macro = cmacro->getMacro();
            std::string name = macro->getName();
            if (name == "Absorption" || name == "IV" || name == "Depot" || name == "Oral") {
                adms.push_back(cmacro);
            }
        }
        return adms;
    }
    
    // Find and return an administration/absorption from administration number
    PKMacro *PKMacros::getAdministration(int adm_num) {
        for (PKMacro *cmacro : this->getAdministrations()) {
            PharmML::PKMacro *macro = cmacro->getMacro();
            // Find 'adm' attribute
            for (PharmML::MacroValue value : macro->getValues()) {
                if (value.first == "adm") {
                    // Get 'adm' code and resolve it
                    this->ast_analyzer.reset();
                    PharmML::AstNode *assignment = value.second;
                    assignment->accept(&ast_analyzer);
                    PharmML::ScalarInt *scalar_int = ast_analyzer.getPureScalarInt();
                    // Compare and return
                    if (scalar_int->toInt() == adm_num) {
                        return cmacro;
                    }
                }
            }
        }
        return nullptr;
    }
    
    // Get all mass transfer type macro's
    std::vector<PKMacro *> PKMacros::getTransfers() {
        std::vector<PKMacro *> trans;
        for (PKMacro *cmacro : this->cmacros) {
            // Find mass transfers
            PharmML::PKMacro *macro = cmacro->getMacro();
            std::string name = macro->getName();
            if (name == "Elimination" || name == "Transfer") {
                trans.push_back(cmacro);
            }
        }
        return trans;
    }
}
