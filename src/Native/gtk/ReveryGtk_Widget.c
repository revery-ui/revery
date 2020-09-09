
#include "config.h"
#ifdef USE_GTK

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>

#include "utilities.h"

static void revery_gtkRealize(GtkWidget *gWidget, gpointer data) {
    gtk_widget_set_window(gWidget, (GdkWindow *)data);
}

CAMLprim value revery_createGtkWidgetFromXWindow(value vXWindow) {
    CAMLparam1(vXWindow);

    Window xWindow = (Window)vXWindow;

    GdkDisplay *gDisplay = gdk_display_get_default();
    XMapRaised(GDK_DISPLAY_XDISPLAY(gDisplay), xWindow);

    GdkWindow *gWindow =
        gdk_x11_window_foreign_new_for_display(gDisplay, xWindow);

    GtkWidget *gWidget = gtk_widget_new(GTK_TYPE_WINDOW, NULL);
    g_signal_connect(gWidget, "realize", G_CALLBACK(revery_gtkRealize), gWindow);
    gtk_widget_set_has_window(gWidget, TRUE);
    gtk_widget_realize(gWidget);

    value camlWidget = revery_wrapPointer(gWidget);

    CAMLreturn(camlWidget);
}

CAMLprim value revery_gtkWidgetShowAll(value vWidget) {
    CAMLparam1(vWidget);

    GtkWidget *gWidget = (GtkWidget *)revery_unwrapPointer(vWidget);
    gtk_widget_show_all(gWidget);

    CAMLreturn(Val_unit);
}

CAMLprim value revery_gtkWidgetGetPath(value vWidget) {
    CAMLparam1(vWidget);
    CAMLlocal1(vPathStr);

    GtkWidget *gWidget = (GtkWidget *)revery_unwrapPointer(vWidget);

    GtkWidgetPath *gWidgetPath = gtk_widget_get_path(gWidget);
    char *pathStr = gtk_widget_path_to_string(gWidgetPath);

    vPathStr = caml_copy_string(pathStr);
    free(pathStr);

    CAMLreturn(vPathStr);
}

CAMLprim value revery_gtkWidgetSetOpacity(value vWidget, value vOpacity) {
    CAMLparam2(vWidget, vOpacity);

    GtkWidget *gWidget = (GtkWidget *)revery_unwrapPointer(vWidget);
    double opacity = Double_val(vOpacity);

    gtk_widget_set_opacity(gWidget, opacity);

    CAMLreturn(Val_unit);
}

CAMLprim value revery_gtkWidgetDestroy(value vWidget) {
    CAMLparam1(vWidget);

    GtkWidget *gWidget = (GtkWidget *)revery_unwrapPointer(vWidget);

    gtk_widget_destroy(gWidget);

    CAMLreturn(Val_unit);
}

CAMLprim value revery_gtkWidgetGetOpacity(value vWidget) {
    CAMLparam1(vWidget);

    GtkWidget *gWidget = (GtkWidget *)revery_unwrapPointer(vWidget);

    double opacity = gtk_widget_get_opacity(gWidget);

    CAMLreturn(caml_copy_double(opacity));
}

CAMLprim value revery_gtkWidgetGetDepth(value vWidget) {
    CAMLparam1(vWidget);

    GtkWidget *gWidget = (GtkWidget *)revery_unwrapPointer(vWidget);

    int depth;
    for (depth = 0; gWidget != NULL; depth++) {
        gWidget = gtk_widget_get_parent(gWidget);
    }

    CAMLreturn(Val_int(depth));
}

#endif
