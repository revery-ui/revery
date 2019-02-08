// Provides: revery_example_notify_changed
// Requires: caml_to_js_string
function revery_example_notify_changed(src) {
    var window = joo_global_object.window;
    if (window && window["__revery_playground_example_notify_changed"]) {
        window["__revery_playground_example_notify_changed"](caml_to_js_string(src));
    }
}
