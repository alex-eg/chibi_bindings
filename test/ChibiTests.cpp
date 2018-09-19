#include "gtest/gtest.h"

#include <Chibi.hpp>

#include <iostream>

TEST(ChibiTests, PrintException) {
    Chibi chibi;

    sexp outputPort = sexp_open_output_string(chibi.context);

    sexp exp = chibi.eval_string("(error \"an error\")");
    EXPECT_TRUE(sexp_exceptionp(exp));
    chibi.print_exception(exp, outputPort);

    ASSERT_EQ(std::string(sexp_string_data(sexp_get_output_string(chibi.context, outputPort))), "ERROR: an error\n");
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
