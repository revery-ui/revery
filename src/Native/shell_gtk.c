#include "config.h"
#ifdef USE_GTK
#include <gtk/gtk.h>

/*  gtk_show_uri is technically deprecated, but it has the most support
 and we aren't attaching to a window or screen */
int revery_openURL_gtk(const char *url_string) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    return (int)gtk_show_uri(
               NULL, // We don't have a parent screen (and we don't need one)
               url_string,
               gtk_get_current_event_time(),
               NULL
           );
#pragma GCC diagnostic pop
}

int revery_openFile_gtk(const char *path_string) {
    GString *url = g_string_new(path_string);
    url = g_string_prepend(url, "file://");
    return revery_openURL_gtk(g_string_free(url, FALSE));
}

#endif
