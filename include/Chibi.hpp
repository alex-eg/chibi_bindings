#pragma once

#include <optional>
#include <vector>

#include <chibi/eval.h>

class SExp;

class Chibi {
public:
    Chibi();

    /** Evaluates a string as a scheme expression and returns the result. */
    SExp eval_string(const std::string &str);

    template <typename... Args>
    /** Evaluates several strings in order and returns the results.

        This might be useful when one needs to first import something, as imports take effect starting from the next expression.
    */
    std::vector<SExp> eval_strings(const Args &...strs);

    /** Registers a function pointer or a lambda to be callable from scheme.

        @arg name name, under which this function will be known in scheme
        @arg fnc the function pointer to use
     */
    template <typename... Args>
    void register_function(const std::string &name, sexp (*fnc)(sexp, sexp, long, Args...));


    // Functions to create scheme values


    /** Look up a value in the environment and return it, otherwise return \p dflt. */
    SExp env_ref(const std::string &name, sexp dflt = SEXP_FALSE);

    /** Make, or "intern", a symbol from a given string.  */
    SExp make_symbol(const std::string &symbol);

    /** Create a list from expressions provided, making it a proper linked list with a nil at the end */
    template <typename Elem, typename... Elems>
    SExp make_list(Elem elem, Elems... elems);
    SExp make_list();

    /** Creates a scheme string from a C++ string. Unlike sexp_make_string, this function fills the string with actual content. */
    SExp make_string(const std::string &str);

    /** Create an scheme integer, heap allocating as a bignum if needed. */
    SExp make_integer(sexp_sint_t num);

    /** Wraps a sexp into a SExp class */
    SExp make_SExp(const sexp &exp);

    ~Chibi();

    /** Current context used in this instance. */
    sexp context;

private:
    /** Current environment used in the context */
    sexp env;
};

#include "Chibi_impl.hpp"
