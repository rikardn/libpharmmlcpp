#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../Dependencies.h"
#include "../Model.h"

TEST_CASE("Dependencies class", "[Dependencies]") {
    PharmML::Dependencies d;

    REQUIRE(!d.hasDependency("CL"));
    REQUIRE(!d.hasDependency("V"));
    REQUIRE(d.numDependencies() == 0);

    SECTION("Add one dependency") {
        d.addDependency("CL");

        REQUIRE(d.hasDependency("CL"));
        REQUIRE(!d.hasDependency("V"));
        REQUIRE(d.numDependencies() == 1);
    }
    SECTION("Re-add same dependency") {
        d.addDependency("CL");
        d.addDependency("CL");

        REQUIRE(d.hasDependency("CL"));
        REQUIRE(!d.hasDependency("V"));
        REQUIRE(d.numDependencies() == 1);
    }
    SECTION("Add two dependencies") {
        d.addDependency("V");
        d.addDependency("CL");

        REQUIRE(d.hasDependency("CL"));
        REQUIRE(d.hasDependency("V"));
        REQUIRE(d.numDependencies() == 2);
    }
}

TEST_CASE("Dependencies of variable", "[Dependencies]") {
    auto *model = new PharmML::Model("testfiles/UseCase2.xml");
    
    REQUIRE(model);
    REQUIRE(model->getModelDefinition());
    REQUIRE(model->getModelDefinition()->getStructuralModel());
    REQUIRE(model->getModelDefinition()->getStructuralModel()->getVariables().size() == 3);
    PharmML::Dependencies& deps1 = model->getModelDefinition()->getStructuralModel()->getVariables()[1]->getDependencies();
    PharmML::Dependencies& deps2 = model->getModelDefinition()->getStructuralModel()->getVariables()[2]->getDependencies();

    SECTION("Check dependencies of UseCase2") {
        REQUIRE(deps1.numDependencies() == 2);
        REQUIRE(deps1.hasDependency("CL"));
        REQUIRE(deps1.hasDependency("V"));

        REQUIRE(deps2.numDependencies() == 6);
        REQUIRE(deps2.hasDependency("T"));
        REQUIRE(deps2.hasDependency("TLAG"));
        REQUIRE(deps2.hasDependency("D"));
        REQUIRE(deps2.hasDependency("V"));
        REQUIRE(deps2.hasDependency("KA"));
        REQUIRE(deps2.hasDependency("k"));
    }
}
