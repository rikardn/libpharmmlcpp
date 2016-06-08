#include "catch.hpp"

#include <cstdlib>
#include <string>
#include <vector>
#include <unordered_set>
#include <codecvt>
#include <locale>
#include <symbols/Symbol.h>
#include <helpers/SymbolNamer.h>
#include <symbols/PopulationParameter.h>

PharmML::Symbol *symbolFromString(std::string name) {
    return new PharmML::PopulationParameter(name);
}

std::vector<PharmML::Symbol *> symbolsFromStrings(std::vector<std::string> names) {
    std::vector<PharmML::Symbol *> symbols;
    for (std::string name : names) {
        PharmML::Symbol *param = new PharmML::PopulationParameter(name);
        symbols.push_back(param);
    }
    return symbols;
}

std::string genRandomString(uint length, std::vector<char32_t> &legal_chars) {
    std::u32string u32str;
    for (uint i = 0; i < length; i++) {
        char32_t rand_char32 = legal_chars.at(rand() % legal_chars.size());
        u32str.push_back(rand_char32);
    }
    std::string str = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.to_bytes(u32str);
    return str;
}

TEST_CASE("SymbolNamer class", "[SymbolNamer]") {
    // Construct a collection of Symbol's from a test set of strings
    std::vector<std::string> test_names = {"CL", "CL", "cl", "cl", "Cl", "cL", "_CL", ".CL", " CL", "CLÅ", "CLå"};
    std::vector<PharmML::Symbol *> test_symbols = symbolsFromStrings(test_names);

    // Define sets of allowed symbols
    std::unordered_set<char32_t> latin_upper(PharmML::LatinChars::UPPER_ALPHA.begin(), PharmML::LatinChars::UPPER_ALPHA.end());
    std::unordered_set<char32_t> latin_lower(PharmML::LatinChars::LOWER_ALPHA.begin(), PharmML::LatinChars::LOWER_ALPHA.end());
    std::unordered_set<char32_t> latin_english(PharmML::LatinChars::ALL_EN.begin(), PharmML::LatinChars::ALL_EN.end());
    std::unordered_set<char32_t> latin_swedish_ext(PharmML::LatinChars::SE_EXT.begin(), PharmML::LatinChars::SE_EXT.end());

    SECTION("Collisions") {
        SECTION("with reserved words") {
            // Setup with reserved words and with latin english charset
            std::unordered_set<std::string> illegals = {"B"};
            PharmML::SymbolNamer namer(illegals);
            namer.addCharSet(latin_english);

            // Tests
            REQUIRE(namer.getNameString(symbolFromString("A")) == "A");
            REQUIRE(namer.getNameString(symbolFromString("B")) != "B");
        }
        SECTION("with earlier symbols") {
            // Very basic character set
            PharmML::SymbolNamer namer;
            namer.addCharSet(std::unordered_set<char32_t>{'A','B'});
            namer.addInitialCharSet(latin_english);

            // Tests
            std::vector<std::string> names = {"A","B","C","AA","BB","CC","A","B","C","AA","BB","CC"};
            std::unordered_set<std::string> gen_names;
            for (PharmML::Symbol *symbol : symbolsFromStrings(names)) {
                std::string name = namer.getNameString(symbol);
                INFO(symbol->getSymbId() << " => " << name);
                REQUIRE(gen_names.count(name) == 0);
                gen_names.insert(name);
            }
        }
    }

    SECTION("Iterations") {
        SECTION("with digits") {
            // Basic character set
            PharmML::SymbolNamer namer;
            namer.addCharSet(latin_upper);
            namer.addInitialCharSet(latin_upper);
            namer.addCharSet(std::unordered_set<char32_t>{'0','1','2','3'});
            namer.addCharSet(std::unordered_set<char32_t>{'_'});

            // Test first symbol addition
            PharmML::Symbol *first = symbolFromString("A");
            REQUIRE(namer.getNameString(first) == "A");

            // Test numeral collision avoidance with subsequent symbols
            std::vector<std::string> exp_names = {"A_","A_2","A_3","A_10","A_11","A_12","A_13","A_20"};
            for (std::string exp_name : exp_names) {
                REQUIRE(namer.getNameString(symbolFromString("A")) == exp_name);
            }

            // Test that first symbol got remembered
            REQUIRE(namer.getNameString(first) == "A");
        }

        SECTION("with latin upper") {
            // Only latin upper character set
            PharmML::SymbolNamer namer;
            namer.addCharSet(latin_upper);

            // Test first symbol addition
            PharmML::Symbol *first = symbolFromString("A");
            REQUIRE(namer.getNameString(first) == "A");

            // Test numeral collision avoidance with subsequent symbols
            std::vector<std::string> exp_names = {"AC","AD","AE","AF"};
            for (std::string exp_name : exp_names) {
                REQUIRE(namer.getNameString(symbolFromString("A")) == exp_name);
            }

            // Test that first symbol got remembered
            REQUIRE(namer.getNameString(first) == "A");
        }
    }

    SECTION("Case restrictions") {
        // Only lower case
        PharmML::SymbolNamer lower_namer, upper_namer;
        lower_namer.addCharSet(latin_lower);
        upper_namer.addCharSet(latin_upper);

        // Tests
        std::vector<std::string> names = {"cl","cL","Cl","CL","ClEaRaNcE","cLeArAnCe","clearance","CLEARANCE"};
        std::unordered_set<std::string> lower_names, upper_names;
        for (PharmML::Symbol *symbol : symbolsFromStrings(names)) {
            std::string lo_name = lower_namer.getNameString(symbol);
            INFO(symbol->getSymbId() << " => " << lo_name);
            std::string up_name = upper_namer.getNameString(symbol);
            INFO(symbol->getSymbId() << " => " << up_name);
            REQUIRE(lower_names.count(lo_name) == 0);
            REQUIRE(upper_names.count(up_name) == 0);
            lower_names.insert(lo_name);
            upper_names.insert(up_name);
        }

        // Test that all symbols are there
        REQUIRE(lower_names.size() == names.size());
        REQUIRE(upper_names.size() == names.size());
    }

    SECTION("Character set") {
        SECTION("restrict initial character") {
            // Stringent restrictions on first character
            PharmML::SymbolNamer namer;
            namer.addCharSet(latin_english);
            std::unordered_set<char32_t> init_chars = {'A','0'};
            namer.addInitialCharSet(init_chars);

            // Generate random test strings
            std::vector<char32_t> legals(latin_english.begin(), latin_english.end());
            std::vector<std::string> names;
            for (uint i = 0; i < 10; i++) {
                names.push_back(genRandomString(10, legals));
            }

            // Tests
            std::unordered_set<std::string> gen_names;
            for (PharmML::Symbol *symbol : symbolsFromStrings(names)) {
                std::string name = namer.getNameString(symbol);
                INFO(symbol->getSymbId() << " => " << name);
                REQUIRE(init_chars.count(name.at(0)) == 1);
                REQUIRE(gen_names.count(name) == 0);
                gen_names.insert(name);
            }

            // Test that all symbols are there
            REQUIRE(gen_names.size() == names.size());
        }

        SECTION("restrict all characters") {
            // Stringent restrictions on all characters
            PharmML::SymbolNamer namer;
            std::unordered_set<char32_t> legal_chars = {'A','a','B','b','C','c','D','d','E','e','F','f'};
            namer.addCharSet(legal_chars);

            // Generate random test strings
            std::vector<char32_t> sampling(latin_english.begin(), latin_english.end());
            std::vector<std::string> names;
            for (uint i = 0; i < 10; i++) {
                names.push_back(genRandomString(5, sampling));
            }

            // Tests
            std::unordered_set<std::string> gen_names;
            for (PharmML::Symbol *symbol : symbolsFromStrings(names)) {
                std::string name = namer.getNameString(symbol);
                INFO(symbol->getSymbId() << " => " << name);
                for (auto it = name.begin(); it != name.end(); ++it) {
                    REQUIRE(legal_chars.count(*it) == 1);
                }
                REQUIRE(gen_names.count(name) == 0);
                gen_names.insert(name);
            }

            // Test that all symbols are there
            REQUIRE(gen_names.size() == names.size());
        }

        SECTION("swedish extensions") {
            // English/swedish test
            PharmML::SymbolNamer eng_namer;
            PharmML::SymbolNamer swe_namer;
            eng_namer.addCharSet(latin_english);
            swe_namer.addCharSet(latin_english);
            swe_namer.addCharSet(latin_swedish_ext);

            // Test strings
            std::vector<std::string> names = {"Hallå!","örn","dåligt_namn","fadäs"};

            // Tests
            std::unordered_set<std::string> eng_names, swe_names;
            for (PharmML::Symbol *symbol : symbolsFromStrings(names)) {
                // English
                std::string eng_name = eng_namer.getNameString(symbol);
                INFO(symbol->getSymbId() << " => " << eng_name << " (english)");
                for (auto it = eng_name.begin(); it != eng_name.end(); ++it) {
                    REQUIRE(latin_english.count(*it) == 1);
                }
                REQUIRE(eng_names.count(eng_name) == 0);
                eng_names.insert(eng_name);

                // Swedish
                std::string swe_name = swe_namer.getNameString(symbol);
                INFO(symbol->getSymbId() << " => " << swe_name << " (swedish)");
                REQUIRE(swe_names.count(swe_name) == 0);
                swe_names.insert(swe_name);

                // Are they the same? They shouldn't be.
                CHECK(eng_name != swe_name);
            }
        }
    }

    SECTION("Length restriction") {
        // Stringent restrictions
        std::unordered_set<std::string> illegals = {"if","then","else","NOT","OR","AND"};
        PharmML::SymbolNamer namer(illegals);
        std::unordered_set<char32_t> legal_chars = {'A','a','D','d','E','e','F','f','I','i','N','n','O','o'};
        namer.addCharSet(legal_chars);
        uint max_length = 4;
        namer.setMaximumLength(max_length);

        // Set test strings
        std::vector<std::string> names(illegals.begin(), illegals.end());
        names.insert(names.end(), illegals.begin(), illegals.end());

        // Tests
        std::unordered_set<std::string> gen_names;
        for (PharmML::Symbol *symbol : symbolsFromStrings(names)) {
            std::string name = namer.getNameString(symbol);
            INFO(symbol->getSymbId() << " => " << name);
            REQUIRE(gen_names.count(name) == 0);
            REQUIRE(name.length() <= max_length);
            gen_names.insert(name);
        }

        // Test that all symbols are there
        REQUIRE(gen_names.size() == names.size());
    }
}
