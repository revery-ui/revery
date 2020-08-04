#pragma once

void revery_caml_call_n(long f, int numArgs, long *args);
void revery_caml_call(long f);
void revery_caml_call_no_lock_n(long f, int numArgs, long *args);
void revery_caml_call_no_lock(long f);
void revery_caml_register_global(long *f);
void revery_caml_unregister_global(long *f);
