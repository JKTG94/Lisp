/*
 * File: list.h
 * ------------
 * Exports the interface to the list data structure along with basic operations
 * such as copying and and disposal
 */

#ifndef _LIST_H_INCLUDED
#define _LIST_H_INCLUDED

#include <stdbool.h>

// The different types of lists
enum type {atom_obj, list_obj, primitive_obj, closure_obj, integer_obj, float_obj};

typedef const char* atom_t;

typedef struct {
  enum type objtype;
} obj;

typedef struct {
  obj* car;
  obj* cdr;
} list_t;

typedef obj*(*primitive_t)(const obj*, obj*);

/**
 * Function: new_list
 * ------------------
 * Returns a list object in dynamically allocated memory
 * @return: A pointer to a new list object in dynamically allocated memory
 */
obj* new_list();

/**
 * Function: new_atom
 * ------------------
 * Create a new atom object
 * @param name: The name to store in the atom object
 * @return: The new atom object wrapping the raw atom
 */
obj* new_atom(atom_t name);

/**
 * Function: new_primitive
 * -----------------------
 * Create a new primitive object
 * @param primitive: The primitive to wrap in an object
 * @return; The new primitive object wrapping the raw primitive instruction pointer
 */
obj* new_primitive(primitive_t primitive);

/**
 * Function: copy
 * --------------
 * Copies an object, returning a new one, leaving the old one untouched
 * @param o: An object to copy
 * @return: A copy of the object
 */
obj* copy(const obj* o);

/**
 * Function: dispose
 * -----------------
 * Free the dynamically allocated memory used to store the lisp object.
 * @param o: Pointer to the lisp object to dispose of
 */
void dispose(obj* o);

/**
 * Function: dispose_recursive
 * ---------------------------
 * Free the allocated memory used to store this lisp object, recursing
 * on any child lisp objects in the case that the object is of the list type.
 * @param o: Pointer to the lisp object to dispose of recursively
 */
void dispose_recursive(obj *o);

/**
 * Function: get_list
 * ------------------
 * Gets a pointer to the list
 * @param o: Pointer to a lisp data structure
 * @return: A pointer to that list data structure's list
 */
list_t* list_of(const obj *o);

/**
 * Function: get_atom
 * ------------------
 * Gets a pointer to the atom
 * @param o: Pointer to a lisp data structure
 * @return: A pointer to the atom in the object
 */
atom_t atom_of(const obj *o);

/**
 * Function: get_primitive
 * -----------------------
 * Gets a pointer to a function pointer to the primitive function for a primitive object list
 * @param o: Pointer to a lisp data structure
 * @return: Function pointer to primitive function
 */
primitive_t* primitive_of(const obj *o);

/**
 * Function: is_empty
 * ------------------
 * @param o: A lisp object to determine if it is the empty list
 * @return: True if the object is the empty list, false otherwise
 */
bool is_empty(const obj* o);

/**
 * Function: deep_compare
 * ----------------------
 * Deep comparison of two lisp objects
 * @param x: The first object to compare
 * @param y: The second object to compare
 * @return: True if the two objects are identical, false otherwise
 */
bool deep_compare(obj* x, obj* y);

/**
 * Function: ith
 * -------------
 * Get the i'th element of a list
 * @param o: The list to get the i'th element of
 * @param i: The index (starting at 0) of the element to get
 * @return: Pointer to the element of the list at index i, or NULL if there is no element there
 */
obj* ith(const obj* o, int i);

/**
 * Function: get_contents
 * ----------------------
 * Utility function for getting the contents of the object that exists just to the right of
 * the object type enum.
 * @param o: Object to get the contents of
 * @return: Pointer to the memory containing the contents of the object
 */
void* get_contents(const obj *o);

#endif // _LIST_H_INCLUDED
