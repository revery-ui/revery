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

type os =
  | Windows
  | Mac
  | Linux
  | Unknown;

let os = {
  let ic = Unix.open_process_in("uname");
  let uname = input_line(ic);
  let _ = close_in(ic);
  switch (Sys.os_type) {
  | "Win32" => Windows
  | _ =>
    switch (uname) {
    | "Darwin" => Mac
    | "Linux" => Linux
    | _ => Unknown
    }
  };
};