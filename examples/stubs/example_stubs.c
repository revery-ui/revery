#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

CAMLprim value revery_example_notify_changed(value vExample) {
  CAMLparam1(vExample);
  const char *szExampleSource = String_val(vExample);

  printf("Switched to example: %s\n", szExampleSource);
  return Val_unit;
}
