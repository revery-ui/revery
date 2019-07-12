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

external yield: unit => unit = "caml_thread_yield";

let getExecutingDirectory = () =>
  if (!isNative) {
    "";
  } else {
    let dir =
      switch (
        String.rindex_opt(Sys.executable_name, '/'),
        String.rindex_opt(Sys.executable_name, '\\'),
      ) {
      | (Some(v1), Some(v2)) =>
        String.sub(Sys.executable_name, 0, max(v1, v2))
      | (None, Some(v)) => String.sub(Sys.executable_name, 0, v)
      | (Some(v), None) => String.sub(Sys.executable_name, 0, v)
      | _ => Sys.executable_name
      };

    /* Check if there is a trailing slash. If not, we need to add one. */

    let len = String.length(dir);
    let ret =
      switch (dir.[len - 1]) {
      | '/' => dir
      | '\\' => dir
      | _ => dir ++ Filename.dir_sep
      };

    ret;
  };

let executingDirectory = getExecutingDirectory();

let getWorkingDirectory = () => Sys.getcwd();

type os =
  | Windows
  | Mac
  | Linux
  | Browser
  | Unknown;

let os = {
  webGL
    ? Browser
    : (
      switch (Sys.os_type) {
      | "Win32" => Windows
      | _ =>
        let ic = Unix.open_process_in("uname");
        let uname = input_line(ic);
        let _ = close_in(ic);
        switch (uname) {
        | "Darwin" => Mac
        | "Linux" => Linux
        | _ => Unknown
        };
      }
    );
};
