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

    SECTION("OpenClosed Setters") {
        std::unique_ptr<ScalarInt> si1 = std::make_unique<ScalarInt>(28);
        std::unique_ptr<ScalarInt> si2 = std::make_unique<ScalarInt>(56);
        Interval interval(std::move(si1), std::move(si2));
        interval.setLeftEndpointOpenClosed(true);
        REQUIRE(interval.isLeftEndpointOpenClosed());
        REQUIRE_FALSE(interval.isRightEndpointOpenClosed());
        interval.setRightEndpointOpenClosed(true);
        REQUIRE(interval.isLeftEndpointOpenClosed());
        REQUIRE(interval.isRightEndpointOpenClosed());    
    }

    SECTION("Endpoint setters") {
        std::unique_ptr<ScalarInt> si1 = std::make_unique<ScalarInt>(28);
        std::unique_ptr<ScalarInt> si2 = std::make_unique<ScalarInt>(56);
        Interval interval(std::move(si1), std::move(si2));
        std::unique_ptr<ScalarInt> si3 = std::make_unique<ScalarInt>(99);
        interval.setLeftEndpoint(std::move(si3));
        ScalarInt *a1 = static_cast<ScalarInt *>(interval.getLeftEndpoint());
        ScalarInt *a2 = static_cast<ScalarInt *>(interval.getRightEndpoint());
        REQUIRE(a1->toInt() == 99);
        REQUIRE(a2->toInt() == 56);
        std::unique_ptr<ScalarInt> si4 = std::make_unique<ScalarInt>(-1);
        interval.setRightEndpoint(std::move(si4));
        a1 = static_cast<ScalarInt *>(interval.getLeftEndpoint());
        a2 = static_cast<ScalarInt *>(interval.getRightEndpoint());
        REQUIRE(a1->toInt() == 99);
        REQUIRE(a2->toInt() == -1);
    }

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
}
