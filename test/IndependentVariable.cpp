#include "catch.hpp"

#include <xml/xml.h>
#include <xml/Document.h>
#include <PharmML/PharmMLReader.h>
#include <symbols/IndependentVariable.h>

using namespace pharmmlcpp;

TEST_CASE("IndependentVariable", "[IndependentVariable]") {
    SECTION("Construction") {
        PharmMLReader reader = PharmMLReader::createTestReader(R"(<IndependentVariable symbId="T"/>)");
        IndependentVariable iv(reader, reader.getRoot());
        REQUIRE(iv.getSymbId() == "T");
    }
}
