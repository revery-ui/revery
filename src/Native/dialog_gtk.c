#ifdef __linux__
#include <gtk/gtk.h>

// The callback to g_signal_connect MUST be an `activate` function
static void activate(GtkApplication *app, const char *user_data) {
  GtkWidget *dialog;

  GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
  dialog = gtk_message_dialog_new(NULL, flags, GTK_MESSAGE_INFO,
                                  GTK_BUTTONS_CLOSE, user_data);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void revery_alert_gtk(void *pWin, const char *szMessage) {
  /*
   * TODO:
   * 1. figure out how to convert the pointer from an X11 window handle
   * to a GTK window, see (for inspiration):
   * https://gist.github.com/mmozeiko/2401933b1fa89e5d5bd238b33eab0465
   *
   * 2. Get reference to revery application, is there an existing
   * gtk application reference when a glfw window is created that can be reused?
   */
  GtkApplication *app;
  app = gtk_application_new("org.gtk.revery", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), (gpointer)szMessage);
  /* argv the final argument to run can be set to NULL in which case argc should
   * be set to 0 */
  g_application_run(G_APPLICATION(app), 0, NULL);
  g_object_unref(app);
}
#endif
