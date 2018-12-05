let isNative =
  switch (Sys.backend_type) {
  | Native => true
  | Bytecode => true
  | _ => false
  };

let webGL = !isNative;

let sleep = (t: Time.t) => {
  /* No-op in JS */
  if (isNative) {
    Unix.sleepf(Time.to_float_seconds(t)); 
  }
}
