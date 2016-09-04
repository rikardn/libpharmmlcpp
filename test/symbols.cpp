#include "catch.hpp"

#include <AST/symbols.h>
#include <PharmML/PharmMLReader.h>

using namespace pharmmlcpp;

TEST_CASE("SymbRef", "[SymbRef]") {
    SECTION("Construct from value and getters") {
        SymbRef sr("REF");
        REQUIRE(sr.getSymbIdRef() == "REF");
        REQUIRE(sr.getBlkIdRef().empty());
        REQUIRE(sr.getSymbol() == nullptr);
    }

    SECTION("Construct from xml") {
        PharmMLReader reader = PharmMLReader::createTestReader(R"(<ct:SymbRef symbIdRef="POP" blkIdRef="MYBLK"/>)");
        SymbRef sr(reader, reader.getRoot());
        REQUIRE(sr.getSymbIdRef() == "POP");
        REQUIRE(sr.getBlkIdRef() == "MYBLK");
        REQUIRE(sr.getSymbol() == nullptr);
    }

    SECTION("Setters") {
        SymbRef sr("REF");
        sr.setSymbIdRef("NEW");
        REQUIRE(sr.getSymbIdRef() == "NEW");
        sr.setBlkIdRef("BLOP");
        REQUIRE(sr.getBlkIdRef() == "BLOP");

        Symbol *symb = (Symbol *) 0xFACE;
        sr.setSymbol(symb);
        REQUIRE(sr.getSymbol() == symb);
    }
    
    SECTION("Copy construct") {
        SymbRef sr("REF");
        sr.setBlkIdRef("BLK");
        SymbRef x(sr);
        x.setSymbIdRef("CHANGED");
        REQUIRE(sr.getSymbIdRef() == "REF");
        REQUIRE(x.getSymbIdRef() == "CHANGED");
        REQUIRE(x.getBlkIdRef() == "BLK");
        REQUIRE(x.getSymbol() == nullptr);
    }

    SECTION("clone") {
        SymbRef sr("REFFY");
        std::unique_ptr<AstNode> cl = sr.clone();
        SymbRef *sr2 = static_cast<SymbRef *>(cl.get());
        REQUIRE(sr2->getSymbIdRef() == "REFFY");
        sr2->setSymbIdRef("CACHE");
        REQUIRE(sr2->getSymbIdRef() == "CACHE");
        REQUIRE(sr.getSymbIdRef() == "REFFY");
    }
}
