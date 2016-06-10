#include "catch.hpp"

#include <PharmML/Model.h>
#include <PharmML/ModellingSteps.h>
#include <visitors/AstAnalyzer.h>

TEST_CASE("ModellingSteps class", "[ModellingSteps]") {
    PharmML::Model model("testfiles/minimal.xml");
    PharmML::AstAnalyzer aa;
    SECTION("Construct Operation") {
        SECTION("simple PopED example") {
            // Construct test node
            const char *xml = R"(\
                <Operation xmlns="http://www.pharmml.org/pharmml/0.8/ModellingSteps" order="1" opType="optimization">
                    <Algorithm definition="PopED">
                        <Property name="criterion">
                            <ct:Assign>
                                <ct:String>explicit</ct:String>
                            </ct:Assign>
                        </Property>
                        <!-- EXCLUDED PROPERTIES -->
                        <Property name="E_family_edsampling">
                            <ct:Assign>
                                <ct:Int>0</ct:Int>
                            </ct:Assign>
                        </Property>
                    </Algorithm>
                </Operation>\
            )";
            xml::Node node = xml::nodeFromString(std::string(xml));

            // Root data
            PharmML::Operation op(model.getContext(), node);
            REQUIRE(op.getOrder() == 1);
            REQUIRE(op.getType() == "optimization");
            REQUIRE(op.getName().empty());

            // Properties
            REQUIRE(op.getProperties().empty());

            // Algorithm
            PharmML::Algorithm *algo = op.getAlgorithm();
            REQUIRE(algo != nullptr);
            REQUIRE(algo->getName().empty());
            REQUIRE(algo->getDefinition() == "PopED");
            std::vector<PharmML::OperationProperty *> properties = algo->getProperties();
            REQUIRE(properties.size() == 2);

            REQUIRE(properties.at(0)->getName() == "criterion");
            REQUIRE(properties.at(0)->getAssignment() != nullptr);
            properties.at(0)->getAssignment()->accept(&aa);
            //~ REQUIRE(aa.getPureString() != nullptr);

            REQUIRE(properties.at(1)->getName() == "E_family_edsampling");
            REQUIRE(properties.at(1)->getAssignment() != nullptr);
            aa.reset();
            properties.at(1)->getAssignment()->accept(&aa);
            REQUIRE(aa.getPureScalarInt() != nullptr);
        }
    }
}
