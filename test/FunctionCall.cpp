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

    SECTION("clone method") {
        std::unique_ptr<ScalarInt> si = std::make_unique<ScalarInt>(28);
        FunctionArgument fa("argname", std::move(si));
        std::unique_ptr<AstNode> cl = fa.clone();
        
        std::unique_ptr<ScalarInt> si2 = std::make_unique<ScalarInt>(2);
        fa.setArgument(std::move(si2));
        
        ScalarInt *a = static_cast<ScalarInt *>(fa.getArgument());
        REQUIRE(a->toInt() == 2);

        a = static_cast<ScalarInt *>(static_cast<FunctionArgument *>(cl.get())->getArgument());
        REQUIRE(a->toInt() == 28);

        (static_cast<FunctionArgument *>(cl.get()))->setSymbId("changed");
        REQUIRE(fa.getSymbId() == "argname");
    }

    SECTION("Copy construct") {
        std::unique_ptr<ScalarInt> si = std::make_unique<ScalarInt>(28);
        FunctionArgument fa("arg", std::move(si));

        FunctionArgument cp{fa};
        std::unique_ptr<ScalarInt> si2 = std::make_unique<ScalarInt>(2);
        cp.setArgument(std::move(si2));
        
        ScalarInt *a = static_cast<ScalarInt *>(fa.getArgument());
        REQUIRE(a->toInt() == 28);

        a = static_cast<ScalarInt *>(cp.getArgument());
        REQUIRE(a->toInt() == 2);
    }

    SECTION("Assignment") {
        std::unique_ptr<ScalarInt> si = std::make_unique<ScalarInt>(28);
        FunctionArgument fa("arg", std::move(si));

        std::unique_ptr<ScalarInt> si2 = std::make_unique<ScalarInt>(-4);
        FunctionArgument fa2("arg2", std::move(si2));

        fa = fa2;
        
        ScalarInt *a = static_cast<ScalarInt *>(fa.getArgument());
        REQUIRE(a->toInt() == -4);
    }
}
