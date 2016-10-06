#include "catch.hpp"

#include <AST/Sequence.h>
#include <AST/Scalar.h>
#include <PharmML/PharmMLReader.h>

using namespace pharmmlcpp;

TEST_CASE("Sequence", "[Sequence]") {

    SECTION("Constructor and getters") {
        std::unique_ptr<ScalarInt> begin = std::make_unique<ScalarInt>(28);
        std::unique_ptr<ScalarInt> stepSize = std::make_unique<ScalarInt>(2);
        std::unique_ptr<ScalarInt> end = std::make_unique<ScalarInt>(56);
        Sequence seq(std::move(begin), std::move(stepSize), nullptr, std::move(end));
        ScalarInt *a1 = static_cast<ScalarInt *>(seq.getBegin());
        ScalarInt *a2 = static_cast<ScalarInt *>(seq.getStepSize());
        ScalarInt *a3 = static_cast<ScalarInt *>(seq.getEnd());
        CHECK(a1->toInt() == 28);
        CHECK(a2->toInt() == 2);
        CHECK(a3->toInt() == 56);
    }

// FIXME: This test is broken. The namespaces cannot be found by the XPath lookups
/*    SECTION("Construct from XML") {
        PharmMLReader reader = PharmMLReader::createTestReader(R"(
					<ct:Sequence>
						<ct:Begin>
						    <ct:Int>56</ct:Int>
						</ct:Begin>
						<ct:StepSize>
							<ct:Int>1</ct:Int>
						</ct:StepSize>
						<ct:End>
							<ct:Int>100</ct:Int>
						</ct:End>
					</ct:Sequence>
        )");
        Sequence s(reader, reader.getRoot());
        ScalarInt *a1 = static_cast<ScalarInt *>(s.getBegin());
        ScalarInt *a2 = static_cast<ScalarInt *>(s.getStepSize());
        ScalarInt *a3 = static_cast<ScalarInt *>(s.getEnd());
        REQUIRE(a1->toInt() == 56);
        REQUIRE(a2->toInt() == 1);
        REQUIRE(a3->toInt() == 100);
    }*/

    SECTION("Setters") {
        std::unique_ptr<ScalarInt> begin = std::make_unique<ScalarInt>(28);
        std::unique_ptr<ScalarInt> stepSize = std::make_unique<ScalarInt>(2);
        std::unique_ptr<ScalarInt> end = std::make_unique<ScalarInt>(56);
        Sequence seq(std::move(begin), std::move(stepSize), nullptr, std::move(end));
        std::unique_ptr<ScalarInt> si1 = std::make_unique<ScalarInt>(23);
        seq.setBegin(std::move(si1));
        ScalarInt *a1 = static_cast<ScalarInt *>(seq.getBegin());
        CHECK(a1->toInt() == 23);

        std::unique_ptr<ScalarInt> si2 = std::make_unique<ScalarInt>(23);
        CHECK_THROWS(seq.setStepNumber(std::move(si2)));
    }

    SECTION("Getters") {
        std::unique_ptr<ScalarInt> begin = std::make_unique<ScalarInt>(28);
        std::unique_ptr<ScalarInt> stepSize = std::make_unique<ScalarInt>(2);
        std::unique_ptr<ScalarInt> end = std::make_unique<ScalarInt>(56);
        Sequence s(std::move(begin), std::move(stepSize), nullptr, std::move(end));

        BinopDivide *div = static_cast<BinopDivide *>(s.getStepNumber());
        BinopMinus *num = static_cast<BinopMinus *>(div->getLeft());
        ScalarInt *e = static_cast<ScalarInt *>(num->getLeft());
        ScalarInt *b = static_cast<ScalarInt *>(num->getRight());
        ScalarInt *ss = static_cast<ScalarInt *>(div->getRight());
        CHECK(e->toInt() == 56);
        CHECK(b->toInt() == 28);
        CHECK(ss->toInt() == 2);
    }

    SECTION("clone method") {
        std::unique_ptr<ScalarInt> begin = std::make_unique<ScalarInt>(28);
        std::unique_ptr<ScalarInt> stepNumber = std::make_unique<ScalarInt>(10);
        std::unique_ptr<ScalarInt> stepSize = std::make_unique<ScalarInt>(1);
        Sequence seq(std::move(begin), std::move(stepSize), std::move(stepNumber), nullptr);
        std::unique_ptr<AstNode> cl = seq.clone();
        std::unique_ptr<ScalarInt> si3 = std::make_unique<ScalarInt>(2);
        seq.setStepSize(std::move(si3));

        ScalarInt *a1 = static_cast<ScalarInt *>(seq.getStepSize());
        ScalarInt *a2 = static_cast<ScalarInt *>(seq.getBegin());
        REQUIRE(a1->toInt() == 2);
        REQUIRE(a2->toInt() == 28);

        a1 = static_cast<ScalarInt *>(static_cast<Sequence *>(cl.get())->getStepSize());
        a2 = static_cast<ScalarInt *>(static_cast<Sequence *>(cl.get())->getBegin());
        REQUIRE(a1->toInt() == 1);
        REQUIRE(a2->toInt() == 28);
    }
    
    SECTION("Copy construct") {
        std::unique_ptr<ScalarInt> begin = std::make_unique<ScalarInt>(28);
        std::unique_ptr<ScalarInt> stepNumber = std::make_unique<ScalarInt>(10);
        std::unique_ptr<ScalarInt> stepSize = std::make_unique<ScalarInt>(1);
        Sequence seq(std::move(begin), std::move(stepSize), std::move(stepNumber), nullptr);
        Sequence seq2{seq};
        std::unique_ptr<ScalarInt> si3 = std::make_unique<ScalarInt>(2);
        seq.setStepSize(std::move(si3));

        ScalarInt *a1 = static_cast<ScalarInt *>(seq.getStepSize());
        ScalarInt *a2 = static_cast<ScalarInt *>(seq.getBegin());
        REQUIRE(a1->toInt() == 2);
        REQUIRE(a2->toInt() == 28);

        a1 = static_cast<ScalarInt *>(seq2.getStepSize());
        a2 = static_cast<ScalarInt *>(seq2.getBegin());
        REQUIRE(a1->toInt() == 1);
        REQUIRE(a2->toInt() == 28);
    }
}
