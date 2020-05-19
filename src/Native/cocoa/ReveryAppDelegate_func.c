#ifdef __APPLE__
#include "ReveryAppDelegate_func.h"

#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/threads.h>
#include <caml/alloc.h>

#include "caml_values.h"

#include "utilities.h"

void revery_appDelegate_openFile(const char *path) {
    static const value *dispatchFileOpen = NULL;
    if (dispatchFileOpen == NULL) {
        dispatchFileOpen = caml_named_value("revery_dispatchFileOpen");
    }
    // Call only if the value was gotten
    if (dispatchFileOpen != NULL) {
        caml_c_thread_register();
        caml_acquire_runtime_system();
        caml_callback(*dispatchFileOpen, caml_copy_string(path));
        caml_release_runtime_system();
    }
}

#endif
