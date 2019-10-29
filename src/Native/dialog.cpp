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
#elif __linux__
  return Val_true;
#else
  return Val_false;
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
#elif __linux__
  revery_alert_gtk(pWin, szMessage);
#else
  // No-op
#endif
  CAMLreturn(Val_unit);
}

CAMLprim value revery_openItem(value vItem) {
  CAMLparam1(vItem);

  const char *szItem = String_val(vItem);
  #ifdef WIN32
    // TODO
  #elif __APPLE__
    revery_openItem_cocoa(szItem);
  #elif __linux__
    // TODO
  #else
    // No-op
  #endif
  CAMLreturn(Val_unit);
}

}
