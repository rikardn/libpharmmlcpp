#include "catch.hpp"

#include <AST/Interval.h>
#include <AST/Scalar.h>
#include <PharmML/PharmMLReader.h>

using namespace pharmmlcpp;

TEST_CASE("Interval", "[Interval]") {
    SECTION("Constructor and getters") {
        std::unique_ptr<ScalarInt> si1 = std::make_unique<ScalarInt>(28);
        std::unique_ptr<ScalarInt> si2 = std::make_unique<ScalarInt>(56);
        Interval interval(std::move(si1), std::move(si2));
        REQUIRE_FALSE(interval.isLeftEndpointOpenClosed());
        REQUIRE_FALSE(interval.isRightEndpointOpenClosed());
        ScalarInt *a1 = static_cast<ScalarInt *>(interval.getLeftEndpoint());
        ScalarInt *a2 = static_cast<ScalarInt *>(interval.getRightEndpoint());
        REQUIRE(a1->toInt() == 28);
        REQUIRE(a2->toInt() == 56);
    }

    SECTION("Construct from XML default endpoints") {
        PharmMLReader reader = PharmMLReader::createTestReader(R"(
					<ct:Interval>
						<ct:LeftEndpoint>
							<ct:Assign>
								<ct:Int>0</ct:Int>
							</ct:Assign>
						</ct:LeftEndpoint>
						<ct:RightEndpoint>
							<ct:Assign>
								<ct:Int>10</ct:Int>
							</ct:Assign>
						</ct:RightEndpoint>
					</ct:Interval>
        )");
        Interval interval(reader.getRoot());
        REQUIRE_FALSE(interval.isLeftEndpointOpenClosed());
        REQUIRE_FALSE(interval.isRightEndpointOpenClosed());
        ScalarInt *a1 = static_cast<ScalarInt *>(interval.getLeftEndpoint());
        ScalarInt *a2 = static_cast<ScalarInt *>(interval.getRightEndpoint());
        REQUIRE(a1->toInt() == 0);
        REQUIRE(a2->toInt() == 10);
    }

    SECTION("Construct from XML mixed explicit endpoints") {
        PharmMLReader reader = PharmMLReader::createTestReader(R"(
					<ct:Interval>
						<ct:LeftEndpoint type="open">
							<ct:Assign>
								<ct:Int>15</ct:Int>
							</ct:Assign>
						</ct:LeftEndpoint>
						<ct:RightEndpoint type="closed">
							<ct:Assign>
								<ct:Int>16</ct:Int>
							</ct:Assign>
						</ct:RightEndpoint>
					</ct:Interval>
        )");
        Interval interval(reader.getRoot());
        REQUIRE(interval.isLeftEndpointOpenClosed());
        REQUIRE_FALSE(interval.isRightEndpointOpenClosed());
        ScalarInt *a1 = static_cast<ScalarInt *>(interval.getLeftEndpoint());
        ScalarInt *a2 = static_cast<ScalarInt *>(interval.getRightEndpoint());
        REQUIRE(a1->toInt() == 15);
        REQUIRE(a2->toInt() == 16);
    }


/*
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
*/
}
