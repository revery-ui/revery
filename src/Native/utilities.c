#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/threads.h>
#include <caml/alloc.h>

#include "caml_values.h"
#include <pthread.h>

/* Sourced from Brisk's `BriskCocoa`
    Thank you @wokalski!
*/
void revery_caml_call_n(value f, int argCount, value *args) {
    caml_c_thread_register();
    caml_callbackN(f, argCount, args);
}

void revery_caml_call(value f) {
    value args[] = {Val_unit};
    revery_caml_call_n(f, 1, args);
}

void revery_caml_call_no_lock_n(value f, int argCount, value *args) {
    caml_callbackN(f, argCount, args);
}

void revery_caml_call_no_lock(value f) {
    printf("%s: f: %ld\n", __func__, f);
    value args[] = {Val_unit};
    revery_caml_call_no_lock_n(f, 1, args);
}

void revery_caml_register_global(value *f) {
    caml_register_generational_global_root(f);
}

void revery_caml_unregister_global(value *f) {
    caml_remove_global_root(f);
}
