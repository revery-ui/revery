#include "config.h"
#ifdef USE_GTK
#include <gtk/gtk.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/threads.h>

#include "utilities.h"

GtkApplication *revery_gtkApp;
value *camlRenderLoop;

void revery_initializeGtk() {
    revery_gtkApp = gtk_application_new("io.reveryui", G_APPLICATION_FLAGS_NONE);
}

gboolean revery_invokeRenderLoop_gtk(gpointer user_data) {
    (void)user_data;
    value vRet = caml_callback(*camlRenderLoop, Val_unit);
    return Val_bool(vRet);
}

CAMLprim value revery_startGtkEventLoop(value camlCallback) {
    CAMLparam1(camlCallback);
    camlRenderLoop = malloc(sizeof(long));
    *camlRenderLoop = camlCallback;

    caml_register_generational_global_root(camlRenderLoop);

    g_timeout_add(0, revery_invokeRenderLoop_gtk, NULL);
    int status = g_application_run(G_APPLICATION(revery_gtkApp), 0, NULL);

    CAMLreturn(Val_int(status));
}
#endif
