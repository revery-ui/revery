#include "config.h"
#ifdef USE_GTK

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#include <gtk/gtk.h>

CAMLprim value revery_gtkEventsPending() {
    CAMLparam0();
    gboolean gEventsPending = gtk_events_pending();
    CAMLreturn(Val_bool(gEventsPending));
}

CAMLprim value revery_gtkMainIteration() {
    CAMLparam0();
    gboolean gMainIteration = gtk_main_iteration();
    CAMLreturn(Val_bool(gMainIteration));
}

#endif
