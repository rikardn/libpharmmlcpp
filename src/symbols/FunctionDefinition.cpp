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

namespace pharmmlcpp
{
    FunctionArgumentDefinition::FunctionArgumentDefinition(PharmMLReader &reader, xml::Node node) {
        this->FunctionArgumentDefinition::parse(reader, node);
    }

    void FunctionArgumentDefinition::parse(PharmMLReader &reader, xml::Node node) {
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

    FunctionDefinition::FunctionDefinition(PharmMLReader &reader, xml::Node node) {
        this->FunctionDefinition::parse(reader, node);
    }

    void FunctionDefinition::parse(PharmMLReader &reader, xml::Node node) {
        this->Symbol::parse(node);

        // Get symbol (return value) type
        this->symbolType = node.getAttribute("symbolType").getValue();

        // Get (non-mandatory) function definition (assignment)
        xml::Node assign = reader.getSingleElement(node, "./ct:Definition/ct:Assign");
        if (assign.exists()) {
            xml::Node tree = assign.getChild();
            this->definition = reader.factory.create(tree);
        }

        // Get (non-mandatory) function argument definitions (symbols with a type)
        std::vector<xml::Node> args = reader.getElements(node, "./ct:FunctionArgument");
        for (xml::Node arg : args) {
            FunctionArgumentDefinition *argument = new FunctionArgumentDefinition(reader, arg);
            this->arguments.push_back(argument);
        }
    }

    std::string FunctionDefinition::getType() {
        return this->symbolType;
    }

    std::vector<FunctionArgumentDefinition *> FunctionDefinition::getArguments() {
        return this->arguments;
    }

    std::shared_ptr<AstNode> FunctionDefinition::getDefinition() {
        return this->definition;
    }

    void FunctionDefinition::setupSymbRefs(SymbolGathering &gathering, std::string blkId) {
        if (this->definition) {
            this->setupAstSymbRefs(this->definition.get(), gathering, blkId);
        }
    }

    void FunctionDefinition::accept(PharmMLVisitor *visitor) {
        visitor->visit(this);
    }

    void FunctionDefinition::accept(SymbolVisitor *visitor) {
        visitor->visit(this);
    }

    // POSTPARSE/CONSOLIDATION
    // Find standard function match and args, store the state and return true if sucessful
    bool FunctionDefinition::isStandardFunction() {
        // TODO: Might want to call this from postParse
        if (this->definition) {
            // Linearize the AST first with the AstAnalyzer
            pharmmlcpp::AstAnalyzer analyzer;
            this->definition->accept(&analyzer);
            std::string linear = analyzer.getValue();

            // Save the linearized function definition, for some reason (debug?)
            this->linear_form = linear;

            // Map the function arguments to generic numbers (and generate anonymous function signature)
            std::string anon_sig = linear;
            std::unordered_map<std::string, pharmmlcpp::FunctionArgumentDefinition *> num_to_arg;
            for (auto it = this->arguments.begin(); it != this->arguments.end(); ++it) {
                std::string name_code = ":" + (*it)->getSymbId() + ":";
                std::string num_code = ":" + std::to_string(std::distance(this->arguments.begin(), it) + 1) + ":";
                num_to_arg[num_code] = *it;
                StringTools::replaceSubstrings(anon_sig, name_code, ":::");

                // Don't know what good transformation below is, but it looks pretty!
                StringTools::replaceSubstrings(linear, name_code, num_code);
            }

            // Find matching standard function and construct argument map
            this->std_func = StandardFunction::NA;
            this->std_arg_map.clear();
            if (anon_sig == ":::") {
                // additiveError: ':1:', where :1:(additive)
                this->std_func = StandardFunction::additiveError;

                // Set the lone parameter
                this->std_arg_map[StandardFunctionArgument::additive] = num_to_arg[":1:"];
            } else if (anon_sig == "times(:::;:::)") {
                // proportionalError: 'times(:1:;:2:)', where :1:(proportional), :2:(f)
                this->std_func = StandardFunction::proportionalError;

                // Set the parameters
                this->std_arg_map[StandardFunctionArgument::proportional] = num_to_arg[":1:"];
                this->std_arg_map[StandardFunctionArgument::prediction]   = num_to_arg[":2:"];
            } else if (anon_sig == "plus(:::;times(:::;:::))" || anon_sig == "plus(times(:::;:::);:::)") {
                // combinedError1: 'plus(:1:;times(:2:;:3:))', where :1:(additive), :2:(proportional), :3:(f)
                this->std_func = StandardFunction::combinedError1;

                // Get the specific permutation of the expression and set them
                if (anon_sig == "plus(:::;times(:::;:::))") {
                    this->std_arg_map[StandardFunctionArgument::additive]     = num_to_arg[":1:"];
                    this->std_arg_map[StandardFunctionArgument::proportional] = num_to_arg[":2:"];
                    this->std_arg_map[StandardFunctionArgument::prediction]   = num_to_arg[":3:"];
                } else { // anon_sig == "plus(times(:::;:::);:::)"
                    this->std_arg_map[StandardFunctionArgument::proportional] = num_to_arg[":1:"];
                    this->std_arg_map[StandardFunctionArgument::prediction]   = num_to_arg[":2:"];
                    this->std_arg_map[StandardFunctionArgument::additive]     = num_to_arg[":3:"];
                }
            } else {
                return false;
            }
            return true;
        } else {
            return false;
        }
    }

    // Get standard function (call after isStandardFunction)
    StandardFunction FunctionDefinition::getStandardFunction() {
        return this->std_func;
    }

    // Get standard function argument map (call after isStandardFunction)
    std::unordered_map<StandardFunctionArgument, FunctionArgumentDefinition *, EnumClassHash> FunctionDefinition::getStandardFunctionArgumentMap() {
        return this->std_arg_map;
    }
}
