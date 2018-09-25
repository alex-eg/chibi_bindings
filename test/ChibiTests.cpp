#include "gtest/gtest.h"

#include <Chibi.hpp>

#include <iostream>

TEST(ChibiTests, PrintException) {
    Chibi chibi;

    sexp outputPort = sexp_open_output_string(chibi.context);

    sexp exp = chibi.eval_string("(error \"an error\")");
    EXPECT_TRUE(sexp_exceptionp(exp));
    chibi.print_exception(exp, outputPort);

    EXPECT_EQ(std::string(sexp_string_data(sexp_get_output_string(chibi.context, outputPort))), "ERROR: an error\n");
}

TEST(ChibiTests, SexpToString) {
    Chibi chibi;

    std::string str = "'(1 2 3)";
    sexp lst = chibi.eval_string(str);
    EXPECT_EQ(chibi.sexp_to_string(lst), "(1 2 3)");
}

TEST(ChibiTests, EvalString) {
    Chibi chibi;

    sexp one_sexp = chibi.eval_string("1");

    EXPECT_TRUE(sexp_fixnump(one_sexp));
    EXPECT_EQ(one_sexp, SEXP_ONE);
}

TEST(ChibiTests, CallCFunction) {
    Chibi chibi;

    auto f = +[](sexp ctx, sexp self, long n, sexp arg) { return arg; };
    chibi.register_function("f", f);
    sexp res = chibi.eval_string("(f 10)");

    EXPECT_EQ(sexp_unbox_fixnum(res), 10);
}

TEST(ChibiTests, MakeList) {
    Chibi chibi;

    sexp res = chibi.make_list(SEXP_ONE, SEXP_TWO, SEXP_THREE);

    sexp lst = chibi.eval_string("'(1 2 3)");
    sexp lst_wrong = chibi.eval_string("'(1 2 3 4)");

    EXPECT_TRUE(
        sexp_unbox_boolean(sexp_equalp(chibi.context, res, lst))
    );
    EXPECT_FALSE(
        sexp_unbox_boolean(sexp_equalp(chibi.context, res, lst_wrong))
    );
}

TEST(ChibiTests, MakeString) {
    Chibi chibi;

    sexp res = chibi.make_string("test");
    EXPECT_EQ(std::string(sexp_string_data(res)), "test");
}
