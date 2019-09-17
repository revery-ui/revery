#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#ifdef WIN32
#include <Windows.h>
#include <winuser.h>
#include "shellscalingapi.h"
#endif

CAMLprim value revery_monitor_getScaleFactor(value vWindow) {
  CAMLparam1(vWindow);

#ifdef WIN32
  HWND hwnd = (HWND)vWindow;
  HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL);

  DEVICE_SCALE_FACTOR scaleFactor = SCALE_100_PERCENT;

  if (GetScaleFromMonitor(hmon, &scaleFactor) == S_OK) {
    printf ("Scale factor: %d\n", scaleFactor); 
  } else {
    printf("FAILED!\n");
  }

#else
  CAMLreturn(caml_copy_double(1.0));
#endif
}
