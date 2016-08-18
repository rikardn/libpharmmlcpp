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
        REQUIRE_THROWS(si = ScalarInt("abc"));
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
        REQUIRE_THROWS(si.set("9999999999"));
    }

    SECTION("Copy construct") {
        ScalarInt si(9);
        ScalarInt x(si);
        x.set(3);
        REQUIRE(si.toInt() == 9);
        REQUIRE(x.toInt() == 3);
    }

    SECTION("clone") {
        ScalarInt si(9);
        std::unique_ptr<AstNode> cl = si.clone();
        REQUIRE(si.toInt() == 9);
        ScalarInt *si2 = static_cast<ScalarInt *>(cl.get());
        REQUIRE(si2->toInt() == 9);
        si2->set(8);
        REQUIRE(si2->toInt() == 8);
        REQUIRE(si.toInt() == 9);
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
        REQUIRE_THROWS(sr = ScalarReal("aa"));
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
        REQUIRE_THROWS(sr = ScalarReal("aa"));
    }

    SECTION("Copy construct") {
        ScalarReal sr(9.0);
        ScalarReal x(sr);
        x.set(3.0);
        REQUIRE(sr.toDouble() == 9.0);
        REQUIRE(x.toDouble() == 3.0);
    }

    SECTION("Getters") {
        ScalarReal sr(89.0);
        // FIXME: Trailing zeros to be removed here! REQUIRE(si.toString() == "89");
        REQUIRE(sr.toDouble() == 89.0);
    }
}

TEST_CASE("ScalarBool", "[ScalarBool]") {
    SECTION("Construct from value") {
        ScalarBool sb(true);
        REQUIRE(sb.toBool());
    }

    SECTION("Setters") {
        ScalarBool sb(true);
        sb.set(false);
        REQUIRE(!sb.toBool());
    }

    SECTION("Copy construct") {
        ScalarBool sb(true);
        ScalarBool x(sb);
        x.set(false);
        REQUIRE(sb.toBool());
        REQUIRE(!x.toBool());
    }
}

TEST_CASE("ScalarString", "[ScalarString]") {
    SECTION("Construct from value") {
        ScalarString ss("MyString");
        REQUIRE(ss.toString() == "MyString");
    }

    SECTION("Construct from xml") {
        PharmMLReader reader = PharmMLReader::createTestReader(R"(<ct:String>AString</ct:String>)");
        ScalarString ss(reader.getRoot());
        REQUIRE(ss.toString() == "AString");
    }

    SECTION("Setters") {
        ScalarString ss("FirstValue");
        ss.set("Ménière");
        REQUIRE(ss.toString() == "Ménière");
    }

    SECTION("Copy construct") {
        ScalarString ss("One");
        ScalarString x(ss);
        x.set("Two");
        REQUIRE(ss.toString() == "One");
        REQUIRE(x.toString() == "Two");
   }
}

TEST_CASE("ScalarId", "[ScalarId]") {
    SECTION("Construct from value") {
        ScalarId sid("MyString");
        REQUIRE(sid.toString() == "MyString");
        REQUIRE_THROWS_AS(sid = ScalarId("1B"), std::invalid_argument);
    }

    SECTION("Construct from xml") {
        PharmMLReader reader = PharmMLReader::createTestReader(R"(<ct:Id>AString</ct:Id>)");
        ScalarId sid(reader.getRoot());
        REQUIRE(sid.toString() == "AString");
    }

    SECTION("Setters") {
        ScalarId sid("FirstValue");
        sid.set("Ménière");
        REQUIRE(sid.toString() == "Ménière");
        REQUIRE_THROWS_AS(sid.set("1A"), std::invalid_argument);
    }

    SECTION("Copy construct") {
        ScalarId sid("One");
        ScalarId x(sid);
        x.set("Two");
        REQUIRE(sid.toString() == "One");
        REQUIRE(x.toString() == "Two");
    }
}
