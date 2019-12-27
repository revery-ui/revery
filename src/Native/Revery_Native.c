#include <stdio.h>

#include "caml_tools.h"
#include <string.h>

#define UNUSED(x) (void)(x)

#ifdef WIN32
#include "ReveryWin32.h"
#elif __APPLE__
#include "ReveryCocoa.h"
#else
#include "ReveryGtk.h"
#endif

CAMLprim value revery_initialize() {
#ifdef __APPLE__
  revery_cocoaSetAppDelegate();
#endif
  return Val_unit;
}
