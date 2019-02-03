#include <glib/gi18n.h>
#include <gtk/gtk.h>

static void show_dialog(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *dialog;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "revery gtk");

  GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
  dialog = gtk_dialog_new_with_buttons("Revery Dialog", GTK_WINDOW(window),
                                       flags, _("_OK"), GTK_RESPONSE_ACCEPT,
                                       _("_Cancel"), GTK_RESPONSE_REJECT, NULL);

  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_show_all(dialog);
  gtk_widget_destroy(dialog);
}

void revery_alert_gtk(void *pWin, const char *szMessage) {
  GtkApplication *app;
  app = gtk_application_new("org.gtk.revery", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "show_dialog", G_CALLBACK(show_dialog), NULL);
  g_application_run(G_APPLICATION(app), 0, 0);
  g_object_unref(app);
}
