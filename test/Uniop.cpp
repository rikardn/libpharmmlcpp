#include "catch.hpp"

#include <AST/Uniop.h>
#include <AST/Scalar.h>
#include <PharmML/PharmMLReader.h>

using namespace pharmmlcpp;

TEST_CASE("Uniop", "[Uniop]") {
    SECTION("Construct, get and set") {
        std::unique_ptr<ScalarInt> si = std::make_unique<ScalarInt>(28);
        ScalarInt *myadd = si.get();
        UniopExp ex{std::move(si)};
        REQUIRE(myadd == ex.getChild());    // The correct child pointer was inserted
    }

    SECTION("clone method") {
        std::unique_ptr<ScalarInt> si = std::make_unique<ScalarInt>(28);
        UniopMinus *ex = new UniopMinus(std::move(si));
        std::unique_ptr<AstNode> cl = ex->clone();
        UniopMinus *cl_cast = static_cast<UniopMinus *>(cl.get());
        ScalarInt *si1 = static_cast<ScalarInt *>(ex->getChild()); 
        ScalarInt *si2 = static_cast<ScalarInt *>(cl_cast->getChild());
        REQUIRE(si1->toInt() == 28);
        REQUIRE(si2->toInt() == 28);
        si1->set(56);
        REQUIRE(si1->toInt() == 56);
        REQUIRE(si2->toInt() == 28);
    }

    SECTION("Copy construct") {
        std::unique_ptr<ScalarInt> si = std::make_unique<ScalarInt>(28);
        UniopLog lg{std::move(si)};
        UniopLog x(lg);
        ScalarInt *si1 = static_cast<ScalarInt *>(lg.getChild());
        ScalarInt *si2 = static_cast<ScalarInt *>(x.getChild());
        REQUIRE(si1->toInt() == 28);
        REQUIRE(si2->toInt() == 28);
        si1->set(56);
        REQUIRE(si1->toInt() == 56);
        REQUIRE(si2->toInt() == 28);
    }
}
