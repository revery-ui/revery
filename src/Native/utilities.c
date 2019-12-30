#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/threads.h>
#include <caml/alloc.h>

#include "caml_values.h"

/* Sourced from Brisk's `BriskCocoa`
    Thank you @wokalski!
*/
void revery_caml_call_n(value f, int argCount, value *args) {
    caml_c_thread_register();
    caml_acquire_runtime_system();
    caml_callbackN(f, argCount, args);
    caml_release_runtime_system();
}

void revery_caml_call(value f) {
    value args[] = {Val_unit};
    revery_caml_call_n(f, 1, args);
}