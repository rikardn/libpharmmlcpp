#include "catch.hpp"

#include <AST/FunctionCall.h>
#include <AST/Scalar.h>
#include <PharmML/PharmMLReader.h>

using namespace pharmmlcpp;

TEST_CASE("FunctionArgument", "[FunctionArgument]") {

    SECTION("Constructor and getters") {
        std::unique_ptr<ScalarInt> si = std::make_unique<ScalarInt>(28);
        FunctionArgument fa("MyID", std::move(si));
        REQUIRE(fa.getSymbId() == "MyID");

        ScalarInt *a = static_cast<ScalarInt *>(fa.getArgument());
        REQUIRE(a->toInt() == 28);
    }

    SECTION("Construct from XML with SymbRefs") {
        PharmMLReader reader = PharmMLReader::createTestReader(R"(
            <math:FunctionArgument symbId="proportional">
                <ct:SymbRef blkIdRef="pm" symbIdRef="CV"/>
            </math:FunctionArgument>
        )");
        FunctionArgument fa(reader.getRoot());
        REQUIRE(fa.getSymbId() == "proportional");
        SymbRef *a = static_cast<SymbRef *>(fa.getArgument());
        REQUIRE(a->getSymbIdRef() == "CV");
    }

    SECTION("Construct from XML with Assign") {
        PharmMLReader reader = PharmMLReader::createTestReader(R"(
            <math:FunctionArgument symbId="proportional">
                <ct:Assign>
                    <ct:Int>99</ct:Int>
                </ct:Assign>
            </math:FunctionArgument>
        )");
        FunctionArgument fa(reader.getRoot());
        REQUIRE(fa.getSymbId() == "proportional");
        ScalarInt *a = static_cast<ScalarInt *>(fa.getArgument());
        REQUIRE(a->toInt() == 99);
    }

    SECTION("Setters") {
        std::unique_ptr<ScalarInt> si = std::make_unique<ScalarInt>(28);
        FunctionArgument fa("name", std::move(si));
        fa.setSymbId("new");
        REQUIRE(fa.getSymbId() == "new");
        std::unique_ptr<ScalarInt> si2 = std::make_unique<ScalarInt>(-23);
        fa.setArgument(std::move(si2));
        ScalarInt *a = static_cast<ScalarInt *>(fa.getArgument());
        REQUIRE(a->toInt() == -23);
    }

    /*
    SECTION("clone method") {
        std::unique_ptr<ScalarInt> si1 = std::make_unique<ScalarInt>(28);
        std::unique_ptr<ScalarInt> si2 = std::make_unique<ScalarInt>(56);
        Interval interval(std::move(si1), std::move(si2));
        std::unique_ptr<AstNode> cl = interval.clone();
        
        std::unique_ptr<ScalarInt> si3 = std::make_unique<ScalarInt>(2);
        interval.setLeftEndpoint(std::move(si3));
        
        ScalarInt *a1 = static_cast<ScalarInt *>(interval.getLeftEndpoint());
        ScalarInt *a2 = static_cast<ScalarInt *>(interval.getRightEndpoint());
        REQUIRE(a1->toInt() == 2);
        REQUIRE(a2->toInt() == 56);

        a1 = static_cast<ScalarInt *>(static_cast<Interval *>(cl.get())->getLeftEndpoint());
        a2 = static_cast<ScalarInt *>(static_cast<Interval *>(cl.get())->getRightEndpoint());
        REQUIRE(a1->toInt() == 28);
        REQUIRE(a2->toInt() == 56);
    }
    
    SECTION("Copy construct") {
        std::unique_ptr<ScalarInt> si1 = std::make_unique<ScalarInt>(28);
        std::unique_ptr<ScalarInt> si2 = std::make_unique<ScalarInt>(56);
        Interval interval(std::move(si1), std::move(si2));

        Interval cp{interval};
        std::unique_ptr<ScalarInt> si3 = std::make_unique<ScalarInt>(2);
        cp.setLeftEndpoint(std::move(si3));
        
        ScalarInt *a1 = static_cast<ScalarInt *>(interval.getLeftEndpoint());
        ScalarInt *a2 = static_cast<ScalarInt *>(interval.getRightEndpoint());
        REQUIRE(a1->toInt() == 28);
        REQUIRE(a2->toInt() == 56);

        a1 = static_cast<ScalarInt *>(cp.getLeftEndpoint());
        a2 = static_cast<ScalarInt *>(cp.getRightEndpoint());
        REQUIRE(a1->toInt() == 2);
        REQUIRE(a2->toInt() == 56);

    }

    SECTION("Assignment") {
        std::unique_ptr<ScalarInt> si1 = std::make_unique<ScalarInt>(28);
        std::unique_ptr<ScalarInt> si2 = std::make_unique<ScalarInt>(56);
        Interval interval1(std::move(si1), std::move(si2));

        std::unique_ptr<ScalarInt> si3 = std::make_unique<ScalarInt>(-2);
        std::unique_ptr<ScalarInt> si4 = std::make_unique<ScalarInt>(-4);
        Interval interval2(std::move(si3), std::move(si4));

        interval1 = interval2;
        
        ScalarInt *a1 = static_cast<ScalarInt *>(interval1.getLeftEndpoint());
        ScalarInt *a2 = static_cast<ScalarInt *>(interval1.getRightEndpoint());
        REQUIRE(a1->toInt() == -2);
        REQUIRE(a2->toInt() == -4);
    }

*/
}
