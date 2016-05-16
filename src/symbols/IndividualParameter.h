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

#ifndef PHARMML_INDIVIDUALPARAMETER_H_
#define PHARMML_INDIVIDUALPARAMETER_H_

#include <PharmML/PharmMLContext.h>
#include <xml/xml.h>
#include <AST/AstNode.h>
#include <visitors/PharmMLVisitor.h>
#include <PharmML/PharmMLSection.h>
#include <PharmML/Dependencies.h>
#include <symbols/Symbol.h>
#include <AST/Scalar.h>

namespace PharmML
{
    class FixedEffect : public Referer
    {
        public:
            FixedEffect(PharmMLContext *context, xml::Node node);
            
            SymbRef *getReference();
            PharmML::AstNode *getScalar();
            std::string getCategory();
            
            Symbol *gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap);
        
        private:
            PharmMLContext *context;
            SymbRef *symbRef = nullptr;
            PharmML::AstNode *scalar = nullptr; // TODO: Is really a Scalar (but general Scalar has no constructor)
            std::string catId;
            
            void parse(xml::Node node);
    };
    
    class IndividualParameter : public PharmMLSection, public Symbol
    {
        public:
            IndividualParameter(PharmMLContext *context, xml::Node node);
            void parse(xml::Node node);
            
            bool isStructured(); // Type 2/3 (linear/generic)
            bool isLinear(); // Type 2
            bool isGeneral(); // Type 3
            bool isExplicit(); // Type 1 (fully explicit
            bool isGeneric(); // Type 4 (distributional)
            
            std::string getTransformation();
            PharmML::AstNode *getPopulationValue();
            
            std::vector<SymbRef *> getCovariates();
            std::vector<FixedEffect *> getFixedEffects(SymbRef *covariate);
            std::vector<FixedEffect *> getFixedEffects(Symbol *covariate);
            
            std::vector<SymbRef *> getRandomEffects();
            
            PharmML::AstNode *getAssignment();
            
            void gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap);
            void accept(PharmMLVisitor *visitor);
            void accept(SymbolVisitor *visitor);
        
        private:
            PharmMLContext *context;
            
            bool is_structured;
            bool is_linear_cov = false;
            bool is_general_cov = false;
            bool is_explicit_cov = false;
            bool is_generic_cov = false;
            
            std::string transformation;
            std::vector<PharmML::AstNode *> transformationParameters;
            PharmML::AstNode *populationValue = nullptr;
            
            std::vector<SymbRef *> covariates;
            std::unordered_map<SymbRef *, std::vector<FixedEffect *>> fixedEffects;
            
            std::vector<SymbRef *> randomEffects;
            
            PharmML::AstNode *generalAssignment = nullptr;
            PharmML::AstNode *explicitAssignment = nullptr;
        
    };
}

#endif
