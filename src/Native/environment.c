#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <stdio.h>
#include <string.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#include "config.h"

CAMLprim value revery_getOperatingSystem() {
  CAMLparam0();
  CAMLreturn(caml_copy_string(PLATFORM_NAME));
}
