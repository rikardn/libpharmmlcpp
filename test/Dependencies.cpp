#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../Dependencies.h"

TEST_CASE("Dependencies", "[Dependencies]") {
    PharmML::Dependencies d;

    REQUIRE(!d.hasDependency("CL"));
    REQUIRE(!d.hasDependency("V"));

    SECTION("Add one depencendy") {
        d.addDependency("CL");

        REQUIRE(d.hasDependency("CL"));
        REQUIRE(!d.hasDependency("V"));
    }
    SECTION("Re-add same dependency") {
        d.addDependency("CL");

        REQUIRE(d.hasDependency("CL"));
        REQUIRE(!d.hasDependency("V"));
    }
    SECTION("Add two dependencies") {
        d.addDependency("V");
        d.addDependency("CL");

        REQUIRE(d.hasDependency("CL"));
        REQUIRE(d.hasDependency("V"));
    }
}
