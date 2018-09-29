
type byteArray = Bigarray.Array1.t(int, Bigarray.int8_unsigned_elt, Bigarray.c_layout);

exception FileOpenException(string);

type successFunction = (byteArray) => unit;
type failureFunction = (string) => unit;

external open_sync_raw: (string, successFunction, failureFunction) => unit = "caml_open_sync_raw";

let (p, r) = Lwt.task();

let someTestCallback = (s) => {
    print_endline ("callback from thread: " ++ s);
    Lwt.wakeup_later(r, "yo!")
};
Callback.register("test_callback", someTestCallback);

let getPromise = () => p;

external caml_test_thread : unit => unit = "caml_test_thread";

let openAsync = (p) => {
    let (promise, resolver) = Lwt.task();

    let success = (ba) => Lwt.wakeup_later(resolver, ba);
    let failure = (message) => Lwt.wakeup_later_exn(resolver, FileOpenException(message));

    open_sync_raw(p, success, failure);
    promise
};
