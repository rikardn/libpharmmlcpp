#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <string>
#include <vector>
#include <unordered_set>
#include <codecvt>
#include <locale>
#include <symbols/Symbol.h>
#include <helpers/SymbolNamer.h>
#include <symbols/PopulationParameter.h>

TEST_CASE("SymbolNamer class", "[SymbolNamer]") {
    // Construct a collection of Symbol's from a test set of strings
    std::vector<std::string> test_names = {"CL", "CL", "cl", "cl", "Cl", "cL", "_CL", ".CL", " CL", "CLÅ", "CLå"};
    std::vector<PharmML::Symbol *> test_symbols;
    for (std::string test_name : test_names) {
        PharmML::Symbol *param = new PharmML::PopulationParameter(test_name);
        test_symbols.push_back(param);
    }

    // Define sets of allowed symbols
    std::unordered_set<char32_t> latin_english(PharmML::LatinChars::ALL.begin(), PharmML::LatinChars::ALL.end());
    std::unordered_set<char32_t> latin_swedish = {u'\uC3A5',u'\uC385',u'\uC3A4',u'\uC384',u'\uC3B6',u'\uC396'}; // å,Å,ä,Ä,ö,Ö
    latin_swedish.insert(latin_english.begin(), latin_english.end());

    SECTION("English name collision") {
        // Setup without reserved words and with latin english charset
        PharmML::SymbolNamer namer;
        namer.addCharSet(latin_english);
        namer.addInitialCharSet(latin_english);

        // Test for collisions
        std::unordered_set<std::u32string> names;
        for (PharmML::Symbol *symbol : test_symbols) {
            std::u32string name = namer.getName(symbol);
            names.insert(name);
            REQUIRE(names.count(name) == 1);
        }
    }
}
