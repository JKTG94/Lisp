//
// Created by Jonathan Deaton on 3/29/18.
//

#ifndef LISP_PARSE_TEST_H
#define LISP_PARSE_TEST_H

#include <interpreter.h>
#include <stdbool.h>


/**
 * Function: test_single_parse
 * ---------------------------
 * Tests to see if a single parsing works correctly
 * @param expr: The expression to parse and then unparse
 * @param expected: The expected result of parsing and then un-parsing the expression
 * @return: True if the result of parsing and un-parsing the expression is equal
 * to the expected expression
 */
bool test_single_parse(const_expression expr, const_expression expected);

/**
 * Function: parser_test
 * ---------------------
 * Tests the functionality of the parser and un-parser.
 * @return: The number of tests that failed
 */
int test_parser();

#endif //LISP_PARSE_TEST_H
