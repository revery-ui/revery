#ifdef __linux__
#include <gtk/gtk.h>
#include <string.h>

// The callback to g_signal_connect MUST be an `activate` function
static void activate(GtkApplication *app, const char *user_data) {
    (void)app;
    GtkWidget *dialog;

    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_message_dialog_new(NULL, flags, GTK_MESSAGE_INFO,
                                    GTK_BUTTONS_CLOSE, "%s", user_data);
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
     * gtk application reference when a glfw window is created that can be
     * reused?
     */
    (void)pWin;
    GtkApplication *app;
    app = gtk_application_new("org.gtk.revery", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate),
                     (gpointer)szMessage);
    /* argv the final argument to run can be set to NULL in which case argc
     * should be set to 0 */
    g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
}

struct FileChooserOptions {
    const char *startDir;
    char **fileTypes;
    int fileTypesSize;
    int allowMultiple;
    int canChooseFiles;
    int canChooseDirectories;
    int showHidden;
    const char *buttonText;
    const char *title;
    const char **result;
};

void activate_filechooser(GtkApplication *app, struct FileChooserOptions *options) {
    (void)app;
    GtkFileChooserAction action = options->canChooseDirectories ? GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER : GTK_FILE_CHOOSER_ACTION_OPEN;
    gint result;
    const char *okButtonText = (options->buttonText ? options->buttonText : "Open");
    const char *dialogTitle = (options->title ? options->title : "Open File(s) and/or Folder(s)");

    GtkWidget *dialog = gtk_file_chooser_dialog_new(
                            dialogTitle, NULL, action, "Cancel", GTK_RESPONSE_CANCEL, okButtonText,
                            GTK_RESPONSE_ACCEPT, NULL);
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);

    gtk_file_chooser_set_show_hidden(chooser, options->showHidden);
    gtk_file_chooser_set_select_multiple(chooser, options->allowMultiple);

    if (options->fileTypes) {
        char *wildcard = "*.";
        char *name = "";
        GtkFileFilter *filter = gtk_file_filter_new();
        for (int i = 0; i < options->fileTypesSize; i++) {
            char *fileType = options->fileTypes[i];
            char *pattern = malloc(strlen(wildcard) + strlen(fileType) + 1);
            strcpy(pattern, wildcard);
            strcat(pattern, fileType);
            name = g_strjoin(" ", name, pattern, NULL);
            gtk_file_filter_add_pattern(filter, pattern);
        }
        gtk_file_filter_set_name(filter, name);
        gtk_file_chooser_add_filter(chooser, filter);
    }

    result = gtk_dialog_run(GTK_DIALOG(dialog));

    if (result == GTK_RESPONSE_ACCEPT) {
        GSList *filenames = gtk_file_chooser_get_filenames(chooser);
        int size = g_slist_length(filenames);
        options->result = malloc((size + 1) * sizeof(char *));
        for (int i = 0; i < size; i++) {
            options->result[i] = (char *) g_slist_nth_data(filenames, i);
        }
        options->result[size] = NULL;
    } else {
        options->result = NULL;
    }

    gtk_widget_destroy(dialog);
}

const char **revery_open_files_gtk(const char *startDir, char *fileTypes[],
                                   int fileTypesSize, int allowMultiple,
                                   int canChooseFiles, int canChooseDirectories,
                                   int showHidden, const char *buttonText,
                                   const char *title) {
    struct FileChooserOptions options = {
        startDir,
        fileTypes,
        fileTypesSize,
        allowMultiple,
        canChooseFiles,
        canChooseDirectories,
        showHidden,
        buttonText,
        title,
        NULL
    };

    GtkApplication *app;
    app = gtk_application_new("org.gtk.revery", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate_filechooser),
                     (gpointer)&options);
    /* argv the final argument to run can be set to NULL in which case argc
     * should be set to 0 */
    g_application_run(G_APPLICATION(app), 0, NULL);


    g_object_unref(app);
    return options.result;
}
#endif
