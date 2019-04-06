// Provides: caml_thread_create
function caml_thread_create() { }

// Provides: caml_thread_initialize
function caml_thread_initialize() { }

// Provides: caml_thread_yield
function caml_thread_yield() { }

// Provides: caml_mutex_new
function caml_mutex_new() { }

// Provides: lwt_unix_iov_max
function lwt_unix_iov_max() { }

// Provides: lwt_unix_init_notification
function lwt_unix_init_notification() { }

// Provides: lwt_unix_init_signals
function lwt_unix_init_signals() { }

// Provides: lwt_unix_set_signal
function lwt_unix_set_signal() { }

// Provides: lwt_unix_system_byte_order
function lwt_unix_system_byte_order() { }

// Provides: lwt_unix_get_page_size
function lwt_unix_get_page_size() { }

// Provides: caml_open_sync_raw
// Requires: caml_ba_create_from, caml_js_from_string, caml_js_to_string
function caml_open_sync_raw(fileName, onSuccess, onFailure) {
    var f = caml_js_from_string(fileName);
    var data = new joo_global_object.Uint8Array([17, 211, 2]);

    var result = null;

    function onSuccess () {
        var res = new joo_global_object.Uint8Array(oReq.response);
        var ba = caml_ba_create_from(res, null, 0, 0, 0, [res.length])
        onSuccess(ba);
    };

    function onFailure () {
        onFailure(caml_js_to_string("failed to load file: " + f));   
    };

    var oReq = new XMLHttpRequest();
    oReq.addEventListener("load", onSuccess, onFailure);
    oReq.responseType = "arraybuffer"
    oReq.open("GET", fileName, true);
    oReq.send();
}
