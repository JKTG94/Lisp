/*
 * File: repl.h
 * ------------
 * Presents the interface to the Read-Eval-Print Loop
 */

#ifndef _LISP_INTERPRETER_H_INCLUDED
#define _LISP_INTERPRETER_H_INCLUDED

#include "parser.h"
#include <stdio.h>

typedef struct LispInterpreterImpl LispInterpreter;

/**
 * Function: repl_init
 * -------------------
 * Initializes the read eval print loop by creating
 * the global environment and the vector of allocated
 * objects.
 */
LispInterpreter * interpreter_init();

/**
 * Function: repl_run_program
 * --------------------------
 * Reads in expressions from a file, and runs them
 * in the environment initialized in repl_init
 * @param program_file: The file containing the program to run
 */
void interpret_program(LispInterpreter *interpreter, const char *program_file);

/**
 * Function: repl_run
 * ------------------
 * Run the interactive REPL prompt
 */
void interpret_fd(LispInterpreter *interpreter, FILE *fd_in, FILE *fd_out);

/**
 * Function: repl_eval
 * -------------------
 * Evaluate a single expression in the global environment
 * @param expr: The expression to evaluate
 * @return: The an expression representing the result of evaluating
 * the passed expression, in dynamically allocated memory that must
 * be freed
 */
expression interpret_expression(LispInterpreter *interpreter, const_expression expr);

/**
 * Function: repl_dispose
 * ----------------------
 * Dispose of the global environment and vector of allocated objects.
 */
void interpreter_dispose(LispInterpreter *interpreter);

#endif //_LISP_INTERPRETER_H_INCLUDED