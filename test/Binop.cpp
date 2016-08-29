#include "catch.hpp"

#include <AST/Binop.h>
#include <AST/Scalar.h>
#include <PharmML/PharmMLReader.h>

using namespace pharmmlcpp;

TEST_CASE("Binop", "[Binop]") {
    SECTION("Construct, get and set") {
        std::unique_ptr<ScalarInt> si1 = std::make_unique<ScalarInt>(28);
        std::unique_ptr<ScalarInt> si2 = std::make_unique<ScalarInt>(56);
        ScalarInt *myadd = si1.get();
        BinopPlus ex{std::move(si1), std::move(si2)};
        REQUIRE(myadd == ex.getLeft());    // The correct child pointer was inserted
    }

    SECTION("clone method") {
        std::unique_ptr<ScalarInt> si1 = std::make_unique<ScalarInt>(28);
        std::unique_ptr<ScalarInt> si2 = std::make_unique<ScalarInt>(56);
        BinopMinus *ex = new BinopMinus(std::move(si1), std::move(si2));
        std::unique_ptr<AstNode> cl = ex->clone();
        BinopMinus *cl_cast = static_cast<BinopMinus *>(cl.get());
        ScalarInt *si1_o = static_cast<ScalarInt *>(ex->getLeft()); 
        ScalarInt *si2_o = static_cast<ScalarInt *>(cl_cast->getLeft());
        REQUIRE(si1_o->toInt() == 28);
        REQUIRE(si2_o->toInt() == 28);
        si1_o->set(99);
        REQUIRE(si1_o->toInt() == 99);
        REQUIRE(si2_o->toInt() == 28);
    }

    SECTION("Copy construct") {
        std::unique_ptr<ScalarInt> si1 = std::make_unique<ScalarInt>(28);
        std::unique_ptr<ScalarInt> si2 = std::make_unique<ScalarInt>(56);
        BinopTimes lg{std::move(si1), std::move(si2)};
        BinopTimes x(lg);
        ScalarInt *si1_o = static_cast<ScalarInt *>(lg.getRight());
        ScalarInt *si2_o = static_cast<ScalarInt *>(x.getRight());
        REQUIRE(si1_o->toInt() == 56);
        REQUIRE(si2_o->toInt() == 56);
        si1_o->set(98);
        REQUIRE(si1_o->toInt() == 98);
        REQUIRE(si2_o->toInt() == 56);
    }
}
