#include "catch.hpp"

#include <AST/Scalar.h>

TEST_CASE("ScalarInt", "[ScalarInt]") {
    SECTION("Construct from value, toInt() and toString()") {
        pharmmlcpp::ScalarInt si(28);
        REQUIRE(si.toInt() == 28);
        REQUIRE(si.toString() == "28");
    }

    SECTION("Construct from string") {
        pharmmlcpp::ScalarInt si("-578");
        REQUIRE(si.toInt() == -578);
    }

    SECTION("Construct from xml") {
        std::string xml = "<ct:Int>56</ct:Int>";
        xml::Node node = xml::nodeFromString(xml);
        pharmmlcpp::ScalarInt si(node);
        REQUIRE(si.toInt() == 56);
    }
}

TEST_CASE("ScalarReal", "[ScalarReal]") {
    SECTION("Construct from value, toDouble() and toString()") {
        pharmmlcpp::ScalarReal sr(9.0);
        REQUIRE(sr.toDouble() == 9);
        REQUIRE(sr.toString() == "9.000000");
    }
    SECTION("Construct from string") {
        pharmmlcpp::ScalarReal sr("-10.0");
        REQUIRE(sr.toDouble() == -10.0);
    }
    SECTION("Construct from xml") {
        std::string xml = "<ct:Real xmlns:ct=\"http://www.pharmml.org/pharmml/0.8.1/CommonTypes\">790.3</ct:Real>";
        xml::Node node = xml::nodeFromString(xml);
        pharmmlcpp::ScalarReal sr(node);
        REQUIRE(Approx(sr.toDouble()) == 790.3);
    }
}
