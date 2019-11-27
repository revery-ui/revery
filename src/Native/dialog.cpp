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

CAMLprim value revery_alertOpenFiles(value vStartDirectory, value vFileTypes, value vAllowMultipleFiles, value vTitle, value vUnit) {
  CAMLparam5(vStartDirectory, vFileTypes, vAllowMultipleFiles, vTitle, vUnit);

  char *startDirectory = NULL;

  // Initialize an array of filetypes
  char **fileTypes = NULL;
  int fileTypesSize = 0;

  // title from OCaml -> C
  char *title = NULL;
  if (vTitle != Val_none)
    title = String_val(Some_val(vTitle));

  int allowMultipleFiles = Bool_val(vAllowMultipleFiles);


  if (vFileTypes != Val_none) {
    CAMLlocal1(camlArr);
    camlArr = Some_val(vFileTypes);
    fileTypesSize = Wosize_val(camlArr);

    // Allocate space for an array
    fileTypes = (char**) malloc(sizeof(char*) * fileTypesSize);

    // Populate the array with the CAML array;
    for (int i = 0; i < fileTypesSize; i++) {
      char* str = String_val(Field(camlArr, i));
      fileTypes[i] = str;
    }
  }

  if (vStartDirectory != Val_none) {
    startDirectory = String_val(Some_val(vStartDirectory));
  }


  char** fileList;

#ifdef __APPLE__
  fileList = revery_open_files_cocoa(startDirectory, fileTypes, fileTypesSize, allowMultipleFiles, title);
#endif

  if (fileList) {
    CAMLlocal1(camlArr);

    int len = -1;
    while (fileList[++len] != NULL) {}
    camlArr = caml_alloc(len, 0);

    for (int i = 0; i < len; i++) {
      Store_field(camlArr, i, caml_copy_string(fileList[i]));
    }

    free(fileList);

    CAMLreturn(Val_some(camlArr));
  } else {
    CAMLreturn(Val_none);
  }
}
}