#include <glib/gi18n.h>
#include <gtk/gtk.h>

// The callback to g_signal_connect MUST be an `activate` function
static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *dialog;

  GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
  dialog = gtk_dialog_new_with_buttons("Revery Dialog", NULL, flags, _("_OK"),
                                       GTK_RESPONSE_ACCEPT, _("_Cancel"),
                                       GTK_RESPONSE_REJECT, NULL);

  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void revery_alert_gtk(void *pWin, const char *szMessage) {
  /*
   * TODO: figure out how to convert the pointer from an X11 window handle
   * to a GTK window, see (for inspiration):
   * https://gist.github.com/mmozeiko/2401933b1fa89e5d5bd238b33eab0465
   */
  GtkApplication *app;
  app = gtk_application_new("org.gtk.revery", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  g_application_run(G_APPLICATION(app), 0, 0);
  g_object_unref(app);
}
