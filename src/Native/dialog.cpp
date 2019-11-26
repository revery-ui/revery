#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <string.h>


#ifdef WIN32
#include "ReveryWin32.h"
#elif __APPLE__
#include "ReveryCocoa.h"
#else
#include "ReveryGtk.h"
#endif

#define Val_none Val_int(0)
static value Val_some(value v) {
  CAMLparam1(v);
  CAMLlocal1(some);
  some = caml_alloc(1, 0);
  Store_field(some, 0, v);
  CAMLreturn(some);
}

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
  char** fileList;
#ifdef __APPLE__
  fileList = revery_open_files_cocoa(startDirectory, NULL, title);
#endif

  if (fileList) {
    CAMLlocal2(camlArr, mlData);

    int len = sizeof(fileList) / sizeof(char*);

    camlArr = caml_alloc(len, 0);


    for (int i = 0; i < len; i++) {
      int strl = strlen(fileList[i]);

      mlData = caml_alloc_string(strl);

      memcpy(String_val(mlData), fileList[i], strl);

      Store_field(camlArr, i, mlData);
    }

    CAMLreturn(Val_some(camlArr));
  } else {
    CAMLreturn(Val_none);
  }
}
}