#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#ifdef WIN32
#include "ReveryWin32.h"
#elif __APPLE__
#include "ReveryCocoa.h"
#else
#include "ReveryGtk.h"
#endif

extern "C" {
CAMLprim value revery_menuSupported() {
#ifdef WIN32
  return Val_true;
#elif __APPLE__
  return Val_false;
#elif __linux__
  return Val_false;
#else
  return Val_false;
#endif
}
}
