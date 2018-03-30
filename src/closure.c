/*
 * File: closures.c
 * ----------------
 * Implements closure related functionality
 */

#include <closure.h>
#include <lisp-objects.h>
#include <garbage-collector.h>
#include <list.h>
#include <environment.h>
#include <evaluator.h>
#include <stack-trace.h>

#include <stdlib.h>

// Static function declarations
static void get_captured_vars(obj **capturedp, const obj *params, const obj *procedure, const obj *env);

obj *make_closure(const obj *lambda, obj *env, GarbageCollector *gc) {
  if (!CHECK_NARGS_MIN(list_of(lambda)->cdr, 1)) return NULL;
  if (!CHECK_NARGS_MAX(list_of(lambda)->cdr, 2)) return NULL;

  obj* params = ith(lambda, 1);
  if (!is_list(params)) return LOG_ERROR("Lambda parameters are not a list");
  FOR_LIST(params, var) {
    if (var == NULL) continue;
    if (is_t(var)) return LOG_ERROR("Truth atom can't be parameter");
    if (is_empty(var)) return LOG_ERROR("Empty list can't be a parameter");
    if (!is_atom(var)) return LOG_ERROR("Parameter was not an atom");
  }

  params = copy_recursive(params); // Params are well-formed, make a copy for saving.
  obj* procedure = copy_recursive(ith(lambda, 2));

  obj* captured = NULL;
  get_captured_vars(&captured, params, procedure, env);

  obj* closure = new_closure_set(params, procedure, captured);
  gc_add_recursive(gc, closure);
  return closure;
}

obj *closure_partial_application(const obj *closure, const obj *args, obj **envp, GarbageCollector *gc) {
  if (closure == NULL) return NULL;

  int nargs = list_length(args);

  obj* params = copy_recursive(sublist(closure_of(closure)->parameters, nargs));
  obj* procedure = copy_recursive(closure_of(closure)->procedure);

  obj* new_bindings = associate(closure_of(closure)->parameters, args, envp, gc);
  obj* captured = join_lists(new_bindings, copy_recursive(closure_of(closure)->captured));

  obj* new_closure = new_closure_set(params, procedure, captured);
  gc_add_recursive(gc, new_closure);
  return new_closure;
}

obj *new_closure_set(obj *params, obj *procedure, obj *captured) {
  obj* o = new_closure();
  closure_of(o)->parameters = params;
  closure_of(o)->procedure = procedure;
  closure_of(o)->captured = captured;
  closure_of(o)->nargs = is_empty(params) ? 0 : list_length(params);
  return o;
}

obj* copy_closure_recursive(const obj* closure) {
  closure_t* c = closure_of(closure);
  obj* params = copy_recursive(c->parameters);
  obj* proc = copy_recursive(c->procedure);
  obj* capt = copy_recursive(c->captured);
  return new_closure_set(params, proc, capt);
}

obj *associate(obj *names, const obj *args, obj **envp, GarbageCollector *gc) {
  if (!is_list(names) || !is_list(args)) return NULL;

  obj* value = eval(list_of(args)->car, envp, gc);
  obj* pair = make_pair(list_of(names)->car, value, true);
  obj* cdr = associate(list_of(names)->cdr, list_of(args)->cdr, envp, gc);
  return new_list_set(pair, cdr);
}

/**
 * Function: get_captured_vars
 * ---------------------------
 * Creates a captured variable list by searching for variable names that exist in both the procedure
 * and the environment. Creates a list of key-value pairs extracted (copied) from the environment.
 * Variable names in the parameter list will not be captured.
 * @param capturedp: Pointer to where the captured list reference should be stored
 * @param params: Parameters to the lambda function (these will not be captured
 * @param procedure: Procedure body of the lambda function to search for variables to bind in
 * @param env: Environment to search for values to capture
 */
static void get_captured_vars(obj **capturedp, const obj *params, const obj *procedure, const obj *env) {
  if (procedure == NULL) return;

  if (is_list(procedure)) { // depth-first search
    get_captured_vars(capturedp, params, list_of(procedure)->car, env);
    get_captured_vars(capturedp, params, list_of(procedure)->cdr, env);

  } else if (is_atom(procedure)) {
    if (lookup_pair(procedure, *capturedp)) return; // Already captured
    if (list_contains(params, procedure)) return; // Don't capture parameters (those get bound at apply-time)

    obj* matching_pair = lookup_pair(procedure, env);
    if (!matching_pair) return; // No value to be captured
    *capturedp = new_list_set(copy_recursive(matching_pair), *capturedp); // Prepend to capture list
  }
}
