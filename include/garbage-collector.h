/**
 * File: allocated-list.h
 * ----------------------
 *
 */

#ifndef _LISP_GARBAGE_COLLECTOR_H_INCLUDED
#define _LISP_GARBAGE_COLLECTOR_H_INCLUDED

#include "lisp-objects.h"

/**
 * Function: init_allocated
 * ------------------------
 * Initializes a CVector of pointers to allocated objects. Note that this list
 * should not contain the environment, or parsed lisp objects. This list should
 * only contain lisp objects that needed to be created during calls to eval.
 * Call this before any calls to eval are made. Calling eval without first calling this
 * method results in undefined behavior.
 */
void init_allocated();

/**
 *  Function: add_allocated
 * -----------------------
 * Adds a lisp object that was created during evaluation and needs to be
 * freed. This method should be called by any function internal to
 * eval that allocated lisp objects in dynamically allocated memory.
 * @param o: A pointer to a lisp object that needs to be free'd
 */
void add_allocated(const obj* o);

/**
 * Function: add_allocated_recursive
 * ---------------------------------
 * Add an object to the allocated list recursively
 * @param o: The object to add recursively
 */
void add_allocated_recursive(const obj* o);

/**
 * Function: clear_allocated
 * -------------------------
 * Frees all of the allocated lisp objects in the allocated list. Suggested usage is to
 * call this function after each call to eval, after the object returned from
 * eval has been completely processed (e.g. copied into environment, serialized, etc...).
 */
void clear_allocated();

/**
 * Function: dispose_allocated
 * ---------------------------
 * Disposes of the CVector of allocated objects. Call this method after
 * all calls to eval are completed to free the memory used to store the
 * CVector of allocated objects.
 */
void dispose_allocated();


#endif //_LISP_GARBAGE_COLLECTOR_H_INCLUDED