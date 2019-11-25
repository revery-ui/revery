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

#define Val_none Val_int(0)
#define Some_val(v) Field(v,0)

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
  printf("WARNING - Not implemented: alert");
#endif
  return Val_unit;
}

CAMLprim value revery_alertOpenFiles(value vStartDirectory, value vFileTypes, value vTitle) {
  CAMLparam3(vStartDirectory, vFileTypes, vTitle);

  char *startDirectory = NULL;

  char *title = String_val(vTitle);

  if (vStartDirectory != Val_none) {
    startDirectory = String_val(Some_val(vStartDirectory));
  }

#ifdef __APPLE__
  char** fileList = revery_open_files_cocoa(startDirectory, NULL, title);
#endif

  return Val_unit;
}
}