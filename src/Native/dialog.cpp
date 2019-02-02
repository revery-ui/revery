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
CAMLprim value revery_alertSupported() {
#ifdef WIN32
  return Val_true;
#elif __APPLE__
  return Val_true;
#else
  return Val_true;
#endif
}

CAMLprim value revery_alert(value vWindow, value vMessage) {
  CAMLparam2(vWindow, vMessage);
  const char *szMessage = String_val(vMessage);
  void *pWin = (void *)vWindow;

#ifdef WIN32
  revery_alert_win32(pWin, szMessage);
#elif __APPLE__
  revery_alert_cocoa(pWin, szMessage);
#else
  revery_alert_gtk(pWin, szMessage);
#endif
  return Val_unit;
}
}
