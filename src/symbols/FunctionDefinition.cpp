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

#include "FunctionDefinition.h"

namespace PharmML
{
    FunctionArgumentDefinition::FunctionArgumentDefinition(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->FunctionArgumentDefinition::parse(node);
    }

    void FunctionArgumentDefinition::parse(xml::Node node) {
        this->Symbol::parse(node);
        
        // Get symbol (argument) type
        this->symbolType = node.getAttribute("symbolType").getValue();
    }

    std::string FunctionArgumentDefinition::getType() {
        return this->symbolType;
    }

    void FunctionArgumentDefinition::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
    
    void FunctionArgumentDefinition::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }
    
    FunctionDefinition::FunctionDefinition(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->FunctionDefinition::parse(node);
    }

    void FunctionDefinition::parse(xml::Node node) {
        this->Symbol::parse(node);
        
        // Get symbol (return value) type
        this->symbolType = node.getAttribute("symbolType").getValue();
        
        // Get (non-mandatory) function definition (assignment)
        xml::Node assign = this->context->getSingleElement(node, "./ct:Definition/ct:Assign");
        if (assign.exists()) {
            xml::Node tree = assign.getChild();
            this->definition = this->context->factory.create(tree);
        }
        
        // Get (non-mandatory) function argument definitions (symbols with a type)
        std::vector<xml::Node> args = this->context->getElements(node, "./ct:FunctionArgument");
        for (xml::Node arg : args) {
            FunctionArgumentDefinition *argument = new FunctionArgumentDefinition(this->context, arg);
            this->arguments.push_back(argument);
        }
    }
    
    std::string FunctionDefinition::getType() {
        return this->symbolType;
    }
    
    std::vector<FunctionArgumentDefinition *> FunctionDefinition::getArguments() {
        return this->arguments;
    }

    AstNode *FunctionDefinition::getDefinition() {
        return this->definition;
    }
    
    void FunctionDefinition::gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) {
        if (this->definition) {
            std::unordered_set<PharmML::Symbol *> found_symbols = this->symbRefsFromAst(this->definition, symbolMap);
            this->addReferences(found_symbols);
        }
    }
    
    void FunctionDefinition::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }
    
    void FunctionDefinition::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }
}
