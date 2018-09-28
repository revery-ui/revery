
type byteArray = Bigarray.Array1.t(int, Bigarray.int8_unsigned_elt, Bigarray.c_layout);

external open_sync_raw: (string) => byteArray = "caml_open_sync_raw";


let openSync = (p) => {
    open_sync_raw(p);
    /* let ret = [|1, 2, 3|]; */
    /* Bigarray.Array1.of_array(Bigarray.Int8_unsigned, Bigarray.C_layout, ret); */
};
