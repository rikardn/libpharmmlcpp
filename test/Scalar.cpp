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
        PharmMLReader reader = PharmMLReader::createTestReader(R"(<ct:Int>56</ct:Int>)");
        ScalarInt si(reader.getRoot());
        REQUIRE(si.toInt() == 56);
    }

    SECTION("Setters") {
        ScalarInt si(0);
        si.set(23);
        REQUIRE(si.toInt() == 23);
        si.set("-451");
        REQUIRE(si.toInt() == -451);
    }

    SECTION("Copy construct") {
        ScalarInt si(9);
        ScalarInt x = si;
        x.set(3);
        REQUIRE(si.toInt() == 9);
        REQUIRE(x.toInt() == 3);
    }

    SECTION("Getters") {
        ScalarInt si(89);
        REQUIRE(si.toString() == "89");
        REQUIRE(si.toInt() == 89);
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
        PharmMLReader reader = PharmMLReader::createTestReader("<ct:Real>790.3</ct:Real>");
        ScalarReal sr(reader.getRoot());
        REQUIRE(Approx(sr.toDouble()) == 790.3);
    }

    SECTION("Setters") {
        ScalarReal sr(0);
        sr.set(29.0);
        REQUIRE(sr.toDouble() == 29.0);
        sr.set("-7.123");
        REQUIRE(sr.toString() == "-7.123");
    }

    SECTION("Copy construct") {
        ScalarReal sr(9.0);
        ScalarReal x = sr;
        x.set(3.0);
        REQUIRE(sr.toDouble() == 9.0);
        REQUIRE(x.toDouble() == 3.0);
    }

    SECTION("Getters") {
        ScalarReal si(89.0);
        // FIXME: Trailing zeros to be removed here! REQUIRE(si.toString() == "89");
        REQUIRE(si.toDouble() == 89.0);
    }
}
