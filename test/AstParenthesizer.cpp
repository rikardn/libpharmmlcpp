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

    SECTION("Simple binop tests") {
        {
            auto var = vars["mul_add"]; // (1*2)+(3/4)
            var->accept(&ap);
            Binop *add = static_cast<Binop *>( var );
            Binop *mul = static_cast<Binop *>( add->getLeft() );
            Binop *div = static_cast<Binop *>( add->getRight() );
            REQUIRE(add->hasParentheses() == false);
            REQUIRE(mul->hasParentheses() == false);
            REQUIRE(div->hasParentheses() == false);
        }
    }
}
