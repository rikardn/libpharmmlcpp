#include "catch.hpp"

#include <AST/Scalar.h>

TEST_CASE("ScalarInt", "[ScalarInt]") {
    SECTION("Construct from value, toInt() and toString()") {
        PharmML::ScalarInt si(28);
        REQUIRE(si.toInt() == 28);
        REQUIRE(si.toString() == "28");
    }

    SECTION("Construct from string") {
        PharmML::ScalarInt si("-578");
        REQUIRE(si.toInt() == -578);
    }

    SECTION("Construct from xml") {
        std::string xml = "<ct:ScalarInt>56</ct:ScalarInt>";
        xml::Node node = xml::nodeFromString(xml);
        PharmML::ScalarInt si(node);
        REQUIRE(si.toInt() == 56);
    }
}
