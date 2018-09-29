
type byteArray = Bigarray.Array1.t(int, Bigarray.int8_unsigned_elt, Bigarray.c_layout);

exception FileOpenException(string);

type successFunction = (byteArray) => unit;
type failureFunction = (string) => unit;

let openAsync: (string) => Lwt.t(byteArray);

let caml_test_thread: unit => unit;

let getPromise: unit => Lwt.t(string);
