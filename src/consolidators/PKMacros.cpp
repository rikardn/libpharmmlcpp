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
    // Construct with PKMacro's as base
    PKMacros::PKMacros(std::vector<PharmML::PKMacro *> pk_macros, std::shared_ptr<PharmML::Logger> logger) {
        this->logger = logger;
        this->pk_macros.reserve(pk_macros.size());
        this->pk_macros.insert(this->pk_macros.begin(), pk_macros.begin(), pk_macros.end());
    }
    
    // Validate the internals
    void PKMacros::validate() {
        // Map from name of attribute to (referable) integer codes (and their PKMacro objects)
        std::unordered_map<std::string, std::unordered_map<int, PharmML::PKMacro *>> int_codes = {
            { "cmt", std::unordered_map<int, PharmML::PKMacro *>() },
            { "adm", std::unordered_map<int, PharmML::PKMacro *>() },
        };
        
        // Check all macros
        for (PharmML::PKMacro *macro : this->pk_macros) {
            // Get name of macro ("Absorption", "Compartment", etc.)
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
                    } else if ((attribute == "cmt" && name == "Compartment") || ( (attribute == "adm") && (name == "Absorption" || name == "IV" || name == "Depot") )) {
                        // Check if there's a (duplicate) referable integer code registred
                        int num = scalar_int->toInt();
                        auto got = int_codes[attribute].find(num);
                        if (got != int_codes[attribute].end()) {
                            PharmML::PKMacro *m2 = int_codes[attribute][num];
                            this->logger->error("PK macro '" + name + "' (%a) shares identifier '"
                                + attribute + "=" + std::to_string(num) + "' illegally with a preceeding '" + m2->getName() + "' (%b)", macro, m2);
                        }
                        
                        // Link attribute, code and macro for above check
                        int_codes[attribute][num] = macro;
                    }
                }
            }
        }
    }
    
    // Get PharmML objects used to consolidate
    std::vector<PharmML::PKMacro *> PKMacros::getMacros() {
        return this->pk_macros;
    }
    
    // Find and return a compartment from compartment number
    PharmML::PKMacro *PKMacros::getCompartment(int cmt_num) {
        for (PharmML::PKMacro *macro : this->pk_macros) {
            // Find compartment
            std::string name = macro->getName();
            if (name == "Compartment") {
                // Find 'cmt' attribute
                for (PharmML::MacroValue value : macro->getValues()) {
                    if (value.first == "cmt") {
                        // Get 'cmt' code and resolve it
                        this->ast_analyzer.reset();
                        PharmML::AstNode *assignment = value.second;
                        assignment->accept(&ast_analyzer);
                        PharmML::ScalarInt *scalar_int = ast_analyzer.getPureScalarInt();
                        if (scalar_int->toInt() == cmt_num) {
                            return macro;
                        }
                    }
                }
            }
        }
        return nullptr;
    }
    
    // Find and return an administration process from administration number
    PharmML::PKMacro *PKMacros::getAdministration(int adm_num) {
        for (PharmML::PKMacro *macro : this->pk_macros) {
            // Find administration
            std::string name = macro->getName();
            if (name == "Absorption" || name == "IV" || name == "Depot") {
                // Find 'adm' attribute
                for (PharmML::MacroValue value : macro->getValues()) {
                    if (value.first == "adm") {
                        // Get 'adm' code and resolve it
                        this->ast_analyzer.reset();
                        PharmML::AstNode *assignment = value.second;
                        assignment->accept(&ast_analyzer);
                        PharmML::ScalarInt *scalar_int = ast_analyzer.getPureScalarInt();
                        if (scalar_int->toInt() == adm_num) {
                            return macro;
                        }
                    }
                }
            }
        }
        return nullptr;
    }
}
