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
}

TEST_CASE("FunctionCall", "[FunctionCall]") {
    SECTION("Constructor") {
        std::unique_ptr<SymbRef> sr = std::make_unique<SymbRef>("SWAN");
        FunctionCall fc(std::move(sr));
        REQUIRE(fc.getFunction()->getSymbIdRef() == "SWAN");
        REQUIRE(fc.getFunctionArguments().empty());
    }

    SECTION("Construct from xml") {
        PharmMLReader reader = PharmMLReader::createTestReader(R"(
            <math:FunctionCall>
                <ct:SymbRef symbIdRef="proportionalError"/>
                <math:FunctionArgument symbId="proportional">
                    <ct:SymbRef blkIdRef="pm" symbIdRef="CV"/>
                </math:FunctionArgument>
                <math:FunctionArgument symbId="f">
                    <ct:SymbRef blkIdRef="sm" symbIdRef="WTOT"/>
                </math:FunctionArgument>
            </math:FunctionCall>
        )");
        FunctionCall fc(reader.getRoot());
        REQUIRE(fc.getFunction()->getSymbIdRef() == "proportionalError");
        REQUIRE(fc.getFunctionArguments().size() == 2);
    }

    SECTION("Setters") {
        std::unique_ptr<SymbRef> sr = std::make_unique<SymbRef>("SWAN");
        FunctionCall fc(std::move(sr));
        std::unique_ptr<SymbRef> another = std::make_unique<SymbRef>("NAMED");
        fc.setFunction(std::move(another));
        REQUIRE(fc.getFunction()->getSymbIdRef() == "NAMED");

        auto &args = fc.getFunctionArguments();

        std::unique_ptr<ScalarInt> si = std::make_unique<ScalarInt>(28);
        std::unique_ptr<FunctionArgument> fa = std::make_unique<FunctionArgument>("MyID", std::move(si));
        args.push_back(std::move(fa));
        REQUIRE(fc.getFunctionArguments().size() == 1);
    }

    SECTION("Copy construct") {
        std::unique_ptr<SymbRef> sr = std::make_unique<SymbRef>("SWAN");
        FunctionCall fc(std::move(sr));

        std::unique_ptr<ScalarInt> si = std::make_unique<ScalarInt>(28);
        std::unique_ptr<FunctionArgument> fa = std::make_unique<FunctionArgument>("arg", std::move(si));
        auto &args = fc.getFunctionArguments();
        args.push_back(std::move(fa));
        
        FunctionCall cp{fc};

        REQUIRE(cp.getFunction()->getSymbIdRef() == "SWAN");
        std::unique_ptr<SymbRef> sr2 = std::make_unique<SymbRef>("GOLDEN");
        cp.setFunction(std::move(sr2));
        REQUIRE(cp.getFunction()->getSymbIdRef() == "GOLDEN");
        REQUIRE(fc.getFunction()->getSymbIdRef() == "SWAN");
       
        auto &cp_args = cp.getFunctionArguments();
        REQUIRE(cp_args[0]->getSymbId() == "arg");
        std::unique_ptr<ScalarInt> si2 = std::make_unique<ScalarInt>(99);
        std::unique_ptr<FunctionArgument> fa2 = std::make_unique<FunctionArgument>("fixed", std::move(si2));
        cp_args[0] = std::move(fa2);
        REQUIRE(cp_args[0]->getSymbId() == "fixed");
        auto &fc_args = fc.getFunctionArguments();
        REQUIRE(fc_args[0]->getSymbId() == "arg");
    }

}
