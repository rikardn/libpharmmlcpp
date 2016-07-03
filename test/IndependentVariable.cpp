#include "catch.hpp"

#include <xml/xml.h>
#include <xml/Document.h>
#include <PharmML/PharmMLReader.h>
#include <symbols/IndependentVariable.h>

TEST_CASE("IndependentVariable", "[IndependentVariable]") {
    SECTION("Construction") {
        std::shared_ptr<xml::Document> doc(new xml::Document());
        doc->parseString(R"(<IndependentVariable symbId="T"/>)");
        auto reader = pharmmlcpp::PharmMLReader(doc);
        pharmmlcpp::IndependentVariable iv(reader, doc->getRoot());
        REQUIRE(iv.getSymbId() == "T");
    }
}
