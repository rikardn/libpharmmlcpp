#include "catch.hpp"

#include <symbols/Symbol.h>
#include <symbols/SymbolSet.h>
#include <symbols/PopulationParameter.h>

TEST_CASE("SymbolSet class", "[SymbolSet]") {
    PharmML::SymbolSet ss;

    PharmML::PopulationParameter cl("CL");
    PharmML::PopulationParameter v("V");

    REQUIRE(!ss.hasSymbol(&cl));
    REQUIRE(!ss.hasSymbol(&v));
    REQUIRE(ss.isEmpty());

    SECTION("Add one symbol") {
        ss.addSymbol(&cl);

        REQUIRE(ss.hasSymbol(&cl));
        REQUIRE(!ss.hasSymbol(&v));
        REQUIRE(ss.numSymbols() == 1);
    }
    SECTION("Re-add same symbol") {
        ss.addSymbol(&cl);
        ss.addSymbol(&cl);

        REQUIRE(ss.hasSymbol(&cl));
        REQUIRE(!ss.hasSymbol(&v));
        REQUIRE(ss.numSymbols() == 1);
    }
    SECTION("Add two symbols") {
        ss.addSymbol(&v);
        ss.addSymbol(&cl);

        REQUIRE(ss.hasSymbol(&cl));
        REQUIRE(ss.hasSymbol(&v));
        REQUIRE(ss.numSymbols() == 2);
    }
}
