#include "catch.hpp"

#include <unordered_map>

#include <PharmML/PharmML.h>
#include <visitors/AstParenthesizer.h>

using namespace pharmmlcpp;

// Assert that no ScalarInt have parentheses in Binop's
void assertScalarIntNoParentheses(std::vector<Binop *> binops) {
    for (auto binop : binops) {
        ScalarInt *sl = static_cast<ScalarInt *>( binop->getLeft() );
        ScalarInt *sr = static_cast<ScalarInt *>( binop->getRight() );
        CHECK(sl->hasParentheses() == false);
        CHECK(sr->hasParentheses() == false);
    }
}

// Assert that no ScalarReal have parentheses in Binop's
void assertScalarRealNoParentheses(std::vector<Binop *> binops) {
    for (auto binop : binops) {
        ScalarReal *sl = static_cast<ScalarReal *>( binop->getLeft() );
        ScalarReal *sr = static_cast<ScalarReal *>( binop->getRight() );
        CHECK(sl->hasParentheses() == false);
        CHECK(sr->hasParentheses() == false);
    }
}

TEST_CASE("AstParenthesizer", "[AstParenthesizer]") {
    PharmML model("testfiles/ast_parenthesizer.xml");
    std::unordered_map<std::string, AstNode *> vars;
    for (auto var : model.getModelDefinition()->getStructuralModel()->getVariables()) {
       vars[var->getSymbId()] = var->getAssignment();
    }
    AstParenthesizer ap;

    // No (scalar) root nodes should require parentheses
    SECTION ("Simple scalars") {
        SymbRef symbref("symbref");
        ScalarInt pos_int(2);
        ScalarInt neg_int(-2);
        ScalarReal pos_real(2);
        ScalarReal neg_real(-2);

        symbref.accept(&ap);
        pos_int.accept(&ap);
        neg_int.accept(&ap);
        pos_real.accept(&ap);
        neg_real.accept(&ap);

        CHECK(symbref.hasParentheses() == false);
        CHECK(pos_int.hasParentheses() == false);
        CHECK(neg_int.hasParentheses() == false);
        CHECK(pos_real.hasParentheses() == false);
        CHECK(neg_real.hasParentheses() == false);
    }

    // Component binop tests (expressions named after the digits they contain)
    SECTION ("Mixed binops") {
        SECTION ("expr ((1+2)+(3+4))+(5+6)") { // = 1+2+3+4+5+6 = 21
            auto var = vars["add"];
            var->accept(&ap);

            // binops
            Binop *e123456 = static_cast<Binop *>( var );
            Binop *e1234 = static_cast<Binop *>( e123456->getLeft() );
            Binop *e12 = static_cast<Binop *>( e1234->getLeft() );
            Binop *e34 = static_cast<Binop *>( e1234->getRight() );
            Binop *e56 = static_cast<Binop *>( e123456->getRight() );

            CHECK(e123456->hasParentheses() == false);
            CHECK(e1234->hasParentheses() == false);
            CHECK(e12->hasParentheses() == false);
            CHECK(e34->hasParentheses() == false);
            CHECK(e56->hasParentheses() == false);

            // scalars
            std::vector<Binop *> scalar_binops = {e12, e34, e56};
            assertScalarIntNoParentheses(scalar_binops);
        }
        SECTION ("expr ((1-2)+(3-4))-((5+6)-(7+8))") { // = 1-2+3-4-(5+6-(7+8)) = 2
            auto var = vars["add_min"];
            var->accept(&ap);

            // binops
            Binop *e12345678 = static_cast<Binop *>( var );
            Binop *e1234 = static_cast<Binop *>( e12345678->getLeft() );
            Binop *e12 = static_cast<Binop *>( e1234->getLeft() );
            Binop *e34 = static_cast<Binop *>( e1234->getRight() );
            Binop *e5678 = static_cast<Binop *>( e12345678->getRight() );
            Binop *e56 = static_cast<Binop *>( e5678->getLeft() );
            Binop *e78 = static_cast<Binop *>( e5678->getRight() );

            CHECK(e12345678->hasParentheses() == false);
            CHECK(e1234->hasParentheses() == false);
            CHECK(e12->hasParentheses() == false);
            CHECK(e34->hasParentheses() == false);
            CHECK(e5678->hasParentheses() == true);
            CHECK(e56->hasParentheses() == false);
            CHECK(e78->hasParentheses() == true);

            // scalars
            std::vector<Binop *> scalar_binops = {e12, e34, e56, e78};
            assertScalarIntNoParentheses(scalar_binops);
        }
        SECTION ("expr (1*2)+(3/4)") { // = 1*2+3/4 = 2.75
            auto var = vars["mul_add_div"];
            var->accept(&ap);

            // binops
            Binop *e1234 = static_cast<Binop *>( var );
            Binop *e12 = static_cast<Binop *>( e1234->getLeft() );
            Binop *e34 = static_cast<Binop *>( e1234->getRight() );

            CHECK(e1234->hasParentheses() == false);
            CHECK(e12->hasParentheses() == false);
            CHECK(e34->hasParentheses() == false);

            // scalars
            std::vector<Binop *> scalar_binops = {e12, e34};
            assertScalarIntNoParentheses(scalar_binops);
        }
        SECTION ("expr ((1.1^1.2)^1.3)^(1.4^1.5)") { // = ((1.1^1.2)^1.3)^1.4^1.5 ~= 1.28
            auto var = vars["exp"];
            var->accept(&ap);

            // binops
            Binop *e12345 = static_cast<Binop *>( var );
            Binop *e123 = static_cast<Binop *>( e12345->getLeft() );
            Binop *e12 = static_cast<Binop *>( e123->getLeft() );
            Binop *e45 = static_cast<Binop *>( e12345->getRight() );

            CHECK(e12345->hasParentheses() == false);
            CHECK(e123->hasParentheses() == true);
            CHECK(e12->hasParentheses() == true);
            CHECK(e45->hasParentheses() == false);

            // scalars
            std::vector<Binop *> scalar_binops = {e12, e45};
            assertScalarRealNoParentheses(scalar_binops);
            ScalarReal *s3 = static_cast<ScalarReal *>( e123->getRight() );
            CHECK(s3->hasParentheses() == false);
        }
    }

    // Component uniop tests (expressions named after the digits they contain)
    SECTION ("Uniop minus") {
        SECTION ("expr (((-1)+(-2))-((-3)-(-4)))+((-5)-(-6))") { // = -1+(-2)-(-3-(-4))+(-5)-(-6) = -3
            auto var = vars["unimin"];
            var->accept(&ap);

            // binops/uniops
            Binop *b_123456 = static_cast<Binop *>( var );
            Binop *b_1234 = static_cast<Binop *>( b_123456->getLeft() );
            Binop *b_12 = static_cast<Binop *>( b_1234->getLeft() );
            Uniop *u_1 = static_cast<Uniop *>( b_12->getLeft() );
            Uniop *u_2 = static_cast<Uniop *>( b_12->getRight() );
            Binop *b_34 = static_cast<Binop *>( b_1234->getRight() );
            Uniop *u_3 = static_cast<Uniop *>( b_34->getLeft() );
            Uniop *u_4 = static_cast<Uniop *>( b_34->getRight() );
            Binop *b_56 = static_cast<Binop *>( b_123456->getRight() );
            Uniop *u_5 = static_cast<Uniop *>( b_56->getLeft() );
            Uniop *u_6 = static_cast<Uniop *>( b_56->getRight() );

            CHECK(b_123456->hasParentheses() == false);
            CHECK(b_1234->hasParentheses() == false);
            CHECK(b_12->hasParentheses() == false);
            CHECK(u_1->hasParentheses() == false);
            CHECK(u_2->hasParentheses() == true);
            CHECK(b_34->hasParentheses() == true);
            CHECK(u_3->hasParentheses() == false);
            CHECK(u_4->hasParentheses() == true);
            CHECK(b_56->hasParentheses() == false);
            CHECK(u_5->hasParentheses() == true);
            CHECK(u_6->hasParentheses() == true);

            // scalars
            std::vector<Uniop *> uniops = {u_1, u_2, u_3, u_4, u_5, u_6};
            for (auto uniop : uniops) {
                ScalarInt *sint = static_cast<ScalarInt *>( uniop->getChild() );
                CHECK(sint->hasParentheses() == false);
            }
        }
    }

    // Logical operator tests (expressions named after the digits they contain)
    SECTION ("Logical operators") {
        SECTION ("((!((1==1)>(!F)))<((2>=3)<=(4>5)))||((T||F)&&(6!=7))") { // = "(! (1==1) > (!F)) < ((2>=3) <= (4>5)) || (T||F) && 6!=7"
            auto var = vars["rand_logic"];
            var->accept(&ap);

            // binops/uniops
            Binop *b11F2345TF67 = static_cast<Binop *>( var );
            Binop *b11F2345 = static_cast<Binop *>( b11F2345TF67->getLeft() );
            Uniop *u11F = static_cast<Uniop *>( b11F2345->getLeft() );
            Binop *b11F = static_cast<Binop *>( u11F->getChild() );
            Binop *b11 = static_cast<Binop *>( b11F->getLeft() );
            Uniop *uF = static_cast<Uniop *>( b11F->getRight() );
            Binop *b2345 = static_cast<Binop *>( b11F2345->getRight() );
            Binop *b23 = static_cast<Binop *>( b2345->getLeft() );
            Binop *b45 = static_cast<Binop *>( b2345->getRight() );
            Binop *bTF67 = static_cast<Binop *>( b11F2345TF67->getRight() );
            Binop *bTF = static_cast<Binop *>( bTF67->getLeft() );
            Binop *b67 = static_cast<Binop *>( bTF67->getRight() );

            CHECK(b11F2345TF67->hasParentheses() == false);
            CHECK(b11F2345->hasParentheses() == false);
            CHECK(u11F->hasParentheses() == true);
            CHECK(b11F->hasParentheses() == false);
            CHECK(b11->hasParentheses() == true);
            CHECK(uF->hasParentheses() == true);
            CHECK(b2345->hasParentheses() == true);
            CHECK(b23->hasParentheses() == true);
            CHECK(b45->hasParentheses() == true);
            CHECK(bTF67->hasParentheses() == false);
            CHECK(bTF->hasParentheses() == true);
            CHECK(b67->hasParentheses() == false);

            // scalars
            ScalarInt *s1_1 = static_cast<ScalarInt *>( b11->getLeft() );
            ScalarInt *s1_2 = static_cast<ScalarInt *>( b11->getRight() );
            ScalarBool *sF_1 = static_cast<ScalarBool *>( uF->getChild() );
            ScalarInt *s2 = static_cast<ScalarInt *>( b23->getLeft() );
            ScalarInt *s3 = static_cast<ScalarInt *>( b23->getRight() );
            ScalarInt *s4 = static_cast<ScalarInt *>( b45->getLeft() );
            ScalarInt *s5 = static_cast<ScalarInt *>( b45->getRight() );
            ScalarBool *sT = static_cast<ScalarBool *>( bTF->getLeft() );
            ScalarBool *sF_2 = static_cast<ScalarBool *>( bTF->getRight() );
            ScalarInt *s6 = static_cast<ScalarInt *>( b67->getLeft() );
            ScalarInt *s7 = static_cast<ScalarInt *>( b67->getRight() );

            CHECK(s1_1->hasParentheses() == false);
            CHECK(s1_2->hasParentheses() == false);
            CHECK(sF_1->hasParentheses() == false);
            CHECK(s2->hasParentheses() == false);
            CHECK(s3->hasParentheses() == false);
            CHECK(s4->hasParentheses() == false);
            CHECK(s5->hasParentheses() == false);
            CHECK(sT->hasParentheses() == false);
            CHECK(sF_2->hasParentheses() == false);
            CHECK(s6->hasParentheses() == false);
            CHECK(s7->hasParentheses() == false);
        }
    }

    // Functions (in R)
    SECTION ("Function operators (self-enclosed)") {
        SECTION ("(-b+sqrt((b^2)-(4*(a*c))))/(2*a) (quadratic formula 1)") { // = (-b + sqrt(b^2 - 4*a*c))/(2*a)
            auto var = vars["quadratic_formula_1"];
            var->accept(&ap);

            // binops/uniops
            Binop *b_bb24ac2a = static_cast<Binop *>( var );
            Binop *b_bb24ac = static_cast<Binop *>( b_bb24ac2a->getLeft() );
            Uniop *u_b = static_cast<Uniop *>( b_bb24ac->getLeft() );
            Uniop *u_b24ac = static_cast<Uniop *>( b_bb24ac->getRight() );
            Binop *b_b24ac = static_cast<Binop *>( u_b24ac->getChild() );
            Binop *b_b2 = static_cast<Binop *>( b_b24ac->getLeft() );
            Binop *b_4ac = static_cast<Binop *>( b_b24ac->getRight() );
            Binop *b_ac = static_cast<Binop *>( b_4ac->getRight() );
            Binop *b_2a = static_cast<Binop *>( b_bb24ac2a->getRight() );

            CHECK(b_bb24ac2a->hasParentheses() == false);
            CHECK(b_bb24ac->hasParentheses() == true);
            CHECK(u_b->hasParentheses() == false);
            CHECK(u_b24ac->hasParentheses() == false);
            CHECK(b_b24ac->hasParentheses() == false);
            CHECK(b_b2->hasParentheses() == false);
            CHECK(b_4ac->hasParentheses() == false);
            CHECK(b_ac->hasParentheses() == false);
            CHECK(b_2a->hasParentheses() == true);

            // scalars
            SymbRef *s_b = static_cast<SymbRef *>( u_b->getChild() );
            ScalarInt *s_4 = static_cast<ScalarInt *>( b_4ac->getLeft() );
            SymbRef *b_a = static_cast<SymbRef *>( b_ac->getLeft() );
            SymbRef *b_c = static_cast<SymbRef *>( b_ac->getRight() );
            ScalarInt *s_2 = static_cast<ScalarInt *>( b_2a->getLeft() );
            SymbRef *s_a = static_cast<SymbRef *>( b_2a->getRight() );

            CHECK(s_b->hasParentheses() == false);
            CHECK(s_4->hasParentheses() == false);
            CHECK(b_a->hasParentheses() == false);
            CHECK(b_c->hasParentheses() == false);
            CHECK(s_2->hasParentheses() == false);
            CHECK(s_a->hasParentheses() == false);
        }
    }
}
