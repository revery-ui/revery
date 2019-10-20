type t =
  Bigarray.Array1.t(int, Bigarray.int16_unsigned_elt, Bigarray.c_layout);

let of_array = array =>
  Bigarray.Array1.of_array(Bigarray.Int16_unsigned, Bigarray.C_layout, array);
