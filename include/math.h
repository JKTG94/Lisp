/*
 * File: math.h
 * ------------
 * Presents the interface of the math library
 */

#ifndef _LISP_MATH_H_INCLUDED
#define _LISP_MATH_H_INCLUDED

#include "list.h"

/**
 * Function: add_math_library
 * --------------------------
 * Get the math library environment
 * @return: The math library environment
 */
obj* get_math_library();

/**
 * Function: new_int
 * -----------------
 * Creates a new integer object wrapping a raw integer value.
 * Note: This object will not be automatically added to the "allocated" list
 * @param value: The integer value to wrap in an object
 * @return: The object wrapping a copy of the integer value in dynamically allocated memory
 */
obj* new_int(int value);

/**
 * Function: new_float
 * -------------------
 * Creates a new float object wrapping a raw integer value.
 * Note: This object will not be automatically added to the "allocated" list
 * @param value: The float value to wrap in an object
 * @return: The object wrapping a copy of the floating point value
 */
obj* new_float(float value);

/**
 * Function: get_int
 * -----------------
 * Get the integer value from an object wrapping a number
 * @param o: The object that is wrapping the integer or the float
 * @return: A copy of the value stored in the object as an integer
 */
int get_int(const obj* o);

/**
 * Function: get_float
 * -------------------
 * Get the floating point value from an object wrapping a number
 * @param o: The object wrapping the integer or floating point value
 * @return: A copy of the value stored in the object as a floating point
 */
float get_float(const obj* o);

/**
 * Primitive: plus
 * ---------------
 * Primitive function for adding two numbers
 * @param o: The unevaluated arguments to add
 * @param env: The environment to evaluate the addition
 * @return: A newly allocated number/float object containing the result of the addition
 */
obj* plus(const obj* o, obj* env);

/**
 * Primitive: subtract
 * -------------------
 * Primitive function for subtracting two numbers
 * @param o: The unevaluated arguments to subtract
 * @param env: The environment to evaluate the subtraction
 * @return: A newly allocated number/float object containing the result of the subtraction
 */
obj* subtract(const obj* o, obj* env);

/**
 * Primitive: multiply
 * -------------------
 * Primitive function for multiplying two numbers
 * @param o: The unevaluated arguments to multiply
 * @param env: The environment to evaluate the multiplication
 * @return: A newly allocated number/float object containing the result of the multiplication
 */
obj* multiply(const obj* o, obj* env);

/**
 * Primitive: divide
 * -----------------
 * Primitive function for dividing two numbers
 * @param o: The unevaluated arguments to divide
 * @param env: The environment to evaluate the division
 * @return: A newly allocated number/float object containing the result of the division
 */
obj* divide(const obj* o, obj* env);

/**
 * Primitive: mod
 * --------------
 * Primitive function for taking modulus of two numbers
 * @param o: The unevaluated arguments to use to take the modulus
 * @param env: The environment to evaluate the modulus
 * @return: A newly allocated number/float object containing the result of the modulus
 */
obj* mod(const obj* o, obj* env);

#endif // _LISP_MATH_H_INCLUDED
