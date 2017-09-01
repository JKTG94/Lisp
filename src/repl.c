/*
 * File: repl.c
 * ------------
 * Presents the implementation of the Read-Eval-Print Loop
 * for Lisp
 */

#include "repl.h"
#include <string.h>
#include <stdio.h>
#include <lisp.h>

#define BUFSIZE 1024
#define UNPARSE_BUFF 16
#define PROMPT "> "
#define REPROMPT ">> "

// Static function declarations
static expression_t getExpression();
static bool isBalanced(expression_t e);
static bool isValid(expression_t e);
static obj* parse(expression_t e, size_t* numParsed);
static expression_t unparse(obj* o);
static size_t atomSize(expression_t e);
static bool isWhiteSpace(char character);

// Get expression_t from stdin, turn it into a list, return the list
obj* read() {
  expression_t input = getExpression();
  if (input == NULL) return NULL;
  obj* o = parse((expression_t) input, NULL);
  free(input);
  return o;
};

// Stringifies the lisp data structure, prints it to stdout
void print(obj* o) {
  if (o == NULL) {
    perror("Error\n");
    return;
  }

  expression_t result = unparse(o);

  if (result) printf("%s", (char*) result);
  else perror("Error\n");
  free(result);
};

// Self explanatory
void repl() {
  while (true) print(eval(read()));
};

// Static functions

/**
 * Function: getExpression
 * -----------------------
 * Gets an expression from the user
 * @return : The expression in a dynamically allocated location
 */
static expression_t getExpression() {
  printf(PROMPT);

  char buff[BUFSIZE];
  int inputSize = scanf("%s", buff);
  int totalSize = inputSize;
  expression_t e = malloc(sizeof(char) * (inputSize + 1));
  if (e == NULL) return NULL;

  strcpy(buff, e);

  bool valid;
  while ((valid = isValid(e)) && !isBalanced(e)) {
    printf(REPROMPT);
    inputSize = scanf("%s", buff);
    e = realloc(e, (size_t) totalSize + inputSize);
    if (e == NULL) return NULL;

    strcpy(e + totalSize + 1, buff);
    totalSize += inputSize;
  }
  if (!valid) return NULL;

  return e;
}

/**
 * Function: unbalanced
 * ------------------------------
 * Checks if all of the open parenthesis in the expression
 * are balanced by a closing parentheses.
 * @param e : A lisp expression
 * @return : True if there are at least as many
 */
static bool isBalanced(expression_t e) {
  int net = 0;
  for (size_t i = 0; i < strlen(e); i++) {
    if (e[i] == '(') net++;
    if (e[i] == ')') net--;
  }
  return net == 0;
}

/**
 * Function: isValid
 * -----------------
 * Checks if the expression is valid
 * @param e : The expression to check
 * @return : True if the expression is valid, false otherwise
 */
static bool isValid(expression_t e) {
  int net = 0;
  for (size_t i = 0; i < strlen(e); i++) {
    if (e[i] == '(') net++;
    if (e[i] == ')') net--;
    if (net < 0) return false;
  }
  return net == 0;
}

/**
 * Function: parse
 * ---------------
 * Converts a lisp expression into a lisp data structure
 * @param input : The lisp expression to objectify
 * @return : Pointer to the lisp data structure
 */
static obj* parse(expression_t e, size_t* numParsedP) {
  size_t i;
  for (i = 0; i < strlen(e); i++)
    if (!isWhiteSpace(e[i])) break;
  if (i == strlen(e)) return NULL; // only whitespace
  if (e[0] == ')') return NULL; // End of list or error

  obj* o = calloc(1, sizeof(obj));

  if (e[0] != '(') {
    o->objtype = atom_obj; // its an atom
    size_t size = atomSize(e);
    o->p = malloc(size + 1);
    if (o->p == NULL) return NULL;
    strcpy(e, o->p);
    if (numParsedP != NULL) *numParsedP = size;
  } else {
    o->objtype = list_obj; // its a list
    o->p = calloc(1, sizeof(list_t));
    list_t* l = o->p;

    size_t carExpressionSize;
    l->car = parse(e + 1, &carExpressionSize);

    size_t cdrExpressionSize;
    l->cdr = parse(e + 1 + carExpressionSize, &cdrExpressionSize);

    if (numParsedP != NULL) *numParsedP = 1 + carExpressionSize + cdrExpressionSize;
  }
  return o;
}

/**
 * Function: unparse
 * -----------------
 * Stringifies a lisp data structure. Will return a pointer to dynamically
 * allocated memory that must be freed.
 * @param o : A lisp object
 * @return : The string representation of the lisp data structure
 */
static expression_t unparse(obj* o) {
  if (o == NULL) return NULL;

  if (o->objtype == atom_obj) {
    expression_t e = malloc(strlen(o->p) + 1);
    return strcpy(e, o->p);
  }

  if (o->objtype == list_obj) {
    expression_t e = malloc(UNPARSE_BUFF);
    if (e == NULL) return NULL;
    e[0] = '(';

    list_t* l = o->p;
    expression_t carExp = unparse(l->car);
    if (carExp == NULL) {
      e[1] = ')';
      return e;
    }

    size_t carExpSize = strlen(carExp);

    // 4: open paren, space, close paren, null terminator
    if (1 + carExpSize + 3 > UNPARSE_BUFF) {
      e = realloc(e, 4 + carExpSize);
      if (e == NULL) return NULL;
    }
    strcpy(e + 1, carExp);
    free(carExp);
    e[1 + carExpSize] = ' ';

    expression_t cdrExp = unparse(l->cdr);
    if (cdrExp == NULL) {
      e[1 + carExpSize] = ')';
      return e;
    }
    size_t cdrExpSize = strlen(cdrExp);

    if (1 + carExpSize + 1 + cdrExpSize + 2 > 4 + carExpSize) {
      e = realloc(e, 4 + carExpSize + cdrExpSize);
      if (e == NULL) return NULL;
    }
    strcpy(e + 2 + carExpSize, cdrExp);
    free(cdrExp);

    strcpy(e + 1 + carExpSize + 1 + cdrExpSize, ")");
    return e;
  }
}

/**
 * Function: atomSize
 * ------------------------
 * Finds the size of the atom pointed to in the expression
 * @param e : An expression that represents an atom
 * @return : The number of characters in that atom
 */
static size_t atomSize(expression_t e) {
  size_t i;
  for(i = 0; i < strlen(e); i++) {
    if (isWhiteSpace(e[i]) || e[i] == '(' || e[i] == ')') return i;
  }
  return i;
}

/**
 * Function: isWhiteSpace
 * ----------------------
 * Checks if a single character is a whitespace character
 * @param character : The character to check
 * @return : True if that character is whitespace, false otherwise
 */
static const char* kWhitespace = " \t\n";
static bool isWhiteSpace(char character) {
  return strchr(kWhitespace, character) == NULL;
}