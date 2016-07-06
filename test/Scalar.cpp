#include "catch.hpp"

#include <AST/Scalar.h>
#include <PharmML/PharmMLReader.h>

using namespace pharmmlcpp;

TEST_CASE("ScalarInt", "[ScalarInt]") {
    SECTION("Construct from value, toInt() and toString()") {
        ScalarInt si(28);
        REQUIRE(si.toInt() == 28);
        REQUIRE(si.toString() == "28");
    }

    SECTION("Construct from string") {
        ScalarInt si("-578");
        REQUIRE(si.toInt() == -578);
    }

    SECTION("Construct from xml") {
        PharmMLReader reader = PharmMLReader::createTestReader(R"(<ct:Int xmlns:ct="http://www.pharmml.org/pharmml/0.8/CommonTypes">56</ct:Int>)");
        ScalarInt si(reader.getRoot());
        REQUIRE(si.toInt() == 56);
    }
}

TEST_CASE("ScalarReal", "[ScalarReal]") {
    SECTION("Construct from value, toDouble() and toString()") {
        ScalarReal sr(9.0);
        REQUIRE(sr.toDouble() == 9);
        REQUIRE(sr.toString() == "9.000000");
    }
    SECTION("Construct from string") {
        ScalarReal sr("-10.0");
        REQUIRE(sr.toDouble() == -10.0);
    }
    SECTION("Construct from xml") {
        PharmMLReader reader = PharmMLReader::createTestReader("<ct:Real xmlns:ct=\"http://www.pharmml.org/pharmml/0.8/CommonTypes\">790.3</ct:Real>");
        ScalarReal sr(reader.getRoot());
        REQUIRE(Approx(sr.toDouble()) == 790.3);
    }
}
