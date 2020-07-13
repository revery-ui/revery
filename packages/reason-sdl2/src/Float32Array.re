type t = Bigarray.Array1.t(float, Bigarray.float32_elt, Bigarray.c_layout);

let of_array = array =>
  Bigarray.Array1.of_array(Bigarray.Float32, Bigarray.C_layout, array);
