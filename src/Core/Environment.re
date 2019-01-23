let isNative =
  switch (Sys.backend_type) {
  | Native => true
  | Bytecode => true
  | _ => false
  };

let webGL = !isNative;

let sleep = (t: Time.t) =>
  /* No-op in JS */
  if (isNative) {
    Unix.sleepf(Time.to_float_seconds(t));
  };

let getExecutingDirectory = () =>
  isNative ? Filename.dirname(Sys.argv[0]) ++ Filename.dir_sep : "";

let getWorkingDirectory = () => Sys.getcwd();

let getAssetPath = p => {
  let ret =
    isNative
      ? {
        let isAbsolute = p |> Fpath.v |> Fpath.normalize |> Fpath.is_abs;

        isAbsolute ? p : getExecutingDirectory() ++ p;
      }
      : p;
  ret;
};
