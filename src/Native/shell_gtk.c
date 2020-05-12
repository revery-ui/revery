#ifdef __linux__
#include <gtk/gtk.h>

int revery_openURL_gtk(const char *url_string) {
    return (int)gtk_show_uri_on_window(
               NULL, // We don't have a parent window (and we don't need one)
               url_string,
               gtk_get_current_event_time(),
               NULL
           );
}

int revery_openFile_gtk(const char *path_string) {
    GString *url = g_string_new(path_string);
    url = g_string_prepend(url, "file://");
    return revery_openURL_gtk(g_string_free(url, FALSE));
}

#endif
