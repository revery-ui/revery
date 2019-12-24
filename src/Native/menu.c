#include <caml/mlvalues.h>

#ifdef WIN32
#include "ReveryWin32.h"
#elif __APPLE__
#include "ReveryCocoa.h"
#else
#include "ReveryGtk.h"
#endif

CAMLprim value revery_menuSupported() {
#ifdef WIN32
    return Val_false;
#elif __APPLE__
    return Val_false;
#elif __linux__
    return Val_false;
#else
    return Val_false;
#endif
}
