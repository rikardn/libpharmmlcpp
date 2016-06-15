#include "catch.hpp"

#include <unordered_map>

#include <PharmML/PharmML.h>
#include <visitors/AstParenthesizer.h>

using namespace pharmmlcpp;

TEST_CASE("AstParenthesizer", "[AstParenthesizer]") {
    PharmML model("testfiles/ast_parenthesizer.xml");
    std::unordered_map<std::string, AstNode *> vars;
    for (auto var : model.getModelDefinition()->getStructuralModel()->getVariables()) {
       vars[var->getSymbId()] = var->getAssignment();
    }
    AstParenthesizer ap;

    SECTION ("Simple binop tests") {
        // Binop's named after the numbers they contain
        SECTION ("((1+2)+(3+4))+(5+6)") {
            auto var = vars["add"]; // ((1+2)+(3+4))+(5+6) = 1+2+3+4+5+6 = 21
            var->accept(&ap);
            Binop *e123456 = static_cast<Binop *>( var );
            Binop *e1234 = static_cast<Binop *>( e123456->getLeft() );
            Binop *e12 = static_cast<Binop *>( e1234->getLeft() );
            Binop *e34 = static_cast<Binop *>( e1234->getRight() );
            Binop *e56 = static_cast<Binop *>( e123456->getRight() );
            
            CHECK(e123456->hasParentheses() == false);
            CHECK(e1234->hasParentheses() == false);
            CHECK(e12->hasParentheses() == false);
            CHECK(e34->hasParentheses() == false);
            CHECK(e56->hasParentheses() == false);
        }
        SECTION ("((1-2)+(3-4))-((5+6)-(7+8))") {
            auto var = vars["add_min"]; // ((1-2)+(3-4))-((5+6)-(7+8)) = 1-2+3-4-(5+6-(7+8)) = 2
            var->accept(&ap);
            Binop *e12345678 = static_cast<Binop *>( var );
            Binop *e1234 = static_cast<Binop *>( e12345678->getLeft() );
            Binop *e12 = static_cast<Binop *>( e1234->getLeft() );
            Binop *e34 = static_cast<Binop *>( e1234->getRight() );
            Binop *e5678 = static_cast<Binop *>( e12345678->getRight() );
            Binop *e56 = static_cast<Binop *>( e5678->getLeft() );
            Binop *e78 = static_cast<Binop *>( e5678->getRight() );
                        
            CHECK(e12345678->hasParentheses() == false);
            CHECK(e1234->hasParentheses() == false);
            CHECK(e12->hasParentheses() == false);
            CHECK(e34->hasParentheses() == false);
            CHECK(e5678->hasParentheses() == true);
            CHECK(e56->hasParentheses() == false); // Non-fatal bug: (5+6) doesn't realize outer parenthesis protects it from minus (feature request!)
            CHECK(e78->hasParentheses() == true);
        }
        SECTION ("(1*2)+(3/4)") {
            auto var = vars["mul_add_div"]; // (1*2)+(3/4) = 2.75
            var->accept(&ap);
            Binop *e1234 = static_cast<Binop *>( var );
            Binop *e12 = static_cast<Binop *>( e1234->getLeft() );
            Binop *e34 = static_cast<Binop *>( e1234->getRight() );
            CHECK(e1234->hasParentheses() == false);
            CHECK(e12->hasParentheses() == false);
            CHECK(e34->hasParentheses() == false);
        }
        SECTION ("((1.1^1.2)^1.3)^(1.4^1.5)") {
            auto var = vars["exp"]; // ((1.1^1.2)^1.3)^(1.4^1.5) = ((1.1^1.2)^1.3)^1.4^1.5 ~= 1.28
            var->accept(&ap);
            Binop *e12345 = static_cast<Binop *>( var );
            Binop *e123 = static_cast<Binop *>( e12345->getLeft() );
            Binop *e12 = static_cast<Binop *>( e123->getLeft() );
            Binop *e45 = static_cast<Binop *>( e12345->getRight() );
            
            CHECK(e12345->hasParentheses() == false);
            CHECK(e123->hasParentheses() == true);
            CHECK(e12->hasParentheses() == true);
            CHECK(e45->hasParentheses() == false);
        }
    }
}
