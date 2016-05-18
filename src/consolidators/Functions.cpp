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

#include <consolidators/Functions.h>

namespace CPharmML
{
    // Add a function
    void Functions::addFunctionDefinition(PharmML::FunctionDefinition *functionDefinition) {
        this->functionDefinitions.push_back(functionDefinition);
        
        // Try to identify a standard function from the definition
        this->identifyStandardFunction(functionDefinition);
    }
    
    std::vector<PharmML::FunctionDefinition *> Functions::getFunctionDefinitions() {
        return this->functionDefinitions;
    }
    
    PharmML::FunctionDefinition *Functions::resolveFunctionCall(PharmML::FunctionCall *functionCall) {
        for (PharmML::FunctionDefinition *functionDefinition : this->functionDefinitions) {
            if (functionCall->referencedSymbols.dependsOn(functionDefinition)) {
                return functionDefinition;
            }
        }
        return nullptr;
    }
    
    std::string Functions::getLinearizedFunctionDefinition(PharmML::FunctionDefinition *functionDefinition) {
        return this->linearizedFunctionDefinitions[functionDefinition];
    }
    
    bool Functions::isStandardFunction(PharmML::FunctionDefinition *functionDefinition) {
        auto got = this->standardFunctions.find(functionDefinition);
        if (got == this->standardFunctions.end()) {
            return false;
        }
        return true;
    }
    
    std::string Functions::getStandardFunctionName(PharmML::FunctionDefinition *functionDefinition) {
        return this->standardFunctions[functionDefinition];
    }
    
    std::unordered_map<std::string, PharmML::FunctionArgumentDefinition *> Functions::getStandardArgumentMap(PharmML::FunctionDefinition *functionDefinition) {
        return this->standardArgumentMaps[functionDefinition];
    }
    
    void Functions::identifyStandardFunction(PharmML::FunctionDefinition *functionDefinition) {
        PharmML::AstNode *definition = functionDefinition->getDefinition();
        std::vector<PharmML::FunctionArgumentDefinition *> arguments = functionDefinition->getArguments();
        if (definition) {
            // Linearize the AST first with the AstAnalyzer
            PharmML::AstAnalyzer analyzer;
            definition->accept(&analyzer);
            std::string linear = analyzer.getValue();
            
            // Save the linearized function definition, for some reason (debug?)
            this->linearizedFunctionDefinitions[functionDefinition] = linear;
            
            // Map the function arguments to generic numbers (and generate anonymous function signature)
            std::string anon_sig = linear;
            std::unordered_map<std::string, PharmML::FunctionArgumentDefinition *> num_to_arg;
            for (auto it = arguments.begin(); it != arguments.end(); ++it) {
                std::string name_code = ":" + (*it)->getSymbId() + ":";
                std::string num_code = ":" + std::to_string(std::distance(arguments.begin(), it) + 1) + ":";
                num_to_arg[num_code] = *it;
                this->replaceString(anon_sig, name_code, ":::");
                
                // Don't know what good transformation below is, but it looks pretty!
                this->replaceString(linear, name_code, num_code);
            }
            
            // Find matching standard function and construct argument map
            std::unordered_map<std::string, PharmML::FunctionArgumentDefinition *> arg_map;
            if (anon_sig == ":::") {
                // additiveError: ':1:', where :1:(additive)
                this->standardFunctions[functionDefinition] = "additiveError";
                this->standardArgumentMaps.emplace(functionDefinition, arg_map);
                
                // Set the lone parameter
                arg_map["additive"] = num_to_arg[":1:"];
                this->standardArgumentMaps[functionDefinition] = arg_map;
            } else if (anon_sig == "times(:::;:::)") {
                // proportionalError: 'times(:1:;:2:)', where :1:(proportional), :2:(f)
                this->standardFunctions[functionDefinition] = "proportionalError";
                this->standardArgumentMaps.emplace(functionDefinition, arg_map);
                
                // Set the parameters
                arg_map["proportional"] = num_to_arg[":1:"];
                arg_map["f"]            = num_to_arg[":2:"];
                this->standardArgumentMaps[functionDefinition] = arg_map;
            } else if (anon_sig == "plus(:::;times(:::;:::))" || anon_sig == "plus(times(:::;:::);:::)") {
                // combinedError1: 'plus(:1:;times(:2:;:3:))', where :1:(additive), :2:(proportional), :3:(f)
                this->standardFunctions[functionDefinition] = "combinedError1";
                this->standardArgumentMaps.emplace(functionDefinition, arg_map);
                
                // Get the specific permutation of the expression and set them
                if (anon_sig == "plus(:::;times(:::;:::))") {
                    arg_map["additive"]     = num_to_arg[":1:"];
                    arg_map["proportional"] = num_to_arg[":2:"];
                    arg_map["f"]            = num_to_arg[":3:"];
                } else { // anon_sig == "plus(times(:::;:::);:::)"
                    arg_map["proportional"] = num_to_arg[":1:"];
                    arg_map["f"]            = num_to_arg[":2:"];
                    arg_map["additive"]     = num_to_arg[":3:"];
                }
                this->standardArgumentMaps[functionDefinition] = arg_map;
            }
        }
    }
    
    void Functions::replaceString(std::string &original, const std::string &replace, const std::string &replacement) {
        size_t pos = 0;
        while((pos = original.find(replace, pos)) != std::string::npos){
            original.replace(pos, replace.length(), replacement);
            pos += replacement.length();
        }
    }
}
