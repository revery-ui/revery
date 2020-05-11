#ifdef __linux__
#include <gtk/gtk.h>

int revery_openItemAtURL_gtk(const char *url_string) {
    GString *url = g_string_new(url_string);
    if (*url_string == '/') {
        url = g_string_prepend(url, "file://");
    }

    int success = (int)gtk_show_uri_on_window(
                      NULL, // We don't have a parent window (and we don't need one)
                      g_string_free(url, FALSE),
                      gtk_get_current_event_time(),
                      NULL
                  );
    return success;
}

#endif
