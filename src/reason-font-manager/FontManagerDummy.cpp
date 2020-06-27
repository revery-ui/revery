#include "config.h"
#ifdef USE_DUMMY

#include "FontDescriptor.h"

extern "C" FontDescriptor *findFont() {
  return NULL;
}

#endif
