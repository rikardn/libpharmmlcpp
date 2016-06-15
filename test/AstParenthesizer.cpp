#include "catch.hpp"

#include <PharmML/PharmML.h>
#include <visitors/AstParenthesizer.h>

TEST_CASE("AstParenthesizer", "[AstParenthesizer]") {
    pharmmlcpp::PharmML model("testfiles/ast_parenthesizer.xml");
    pharmmlcpp::AstParenthesizer ap;
    SECTION("WIP") {
       
    }
}
